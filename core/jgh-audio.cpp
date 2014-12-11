//-----------------------------------------------------------------------------
// name: jgh-audio.cpp
// desc: audio stuffs
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------
#include "jgh-audio.h"
#include "jgh-globals.h"
#include "jgh-sim.h"
#include "y-fft.h"
#include "jgh-me.h"
#include "y-waveform.h"
#include "y-fluidsynth.h"
#include <iostream>
#include "x-fun.h"
using namespace std;

    
JGHSynth *g_synth;  

double g_now;
double g_nextTime;
double g_currBeat = Globals::samplesPerBeatDivisor;

vector<Track*>g_tracks;

XMutex g_mutex;


Track *getCurrentTrack()
{
    return g_tracks[Globals::currentTrack % Globals::numberOfTracks];
}

unsigned short  getCurrentDrum()
{
    switch(Globals::currentTrack % Globals::numberOfTracks)
    {
        case JGH_KICK_DRUM:
        {
            return 35;
            break;
        }
        case JGH_CLAP:
        {
            return 39;
            break;
        }
        case JGH_HIHAT:
        {
            return 42;
            break;
        }
        case JGH_OPEN_HI:
        {
            return 46;
            break;
        }
        case JGH_COWBELL:
        {
            return 56;
            break;
        }
        case JGH_METRONOME:
        {
            return 75;
            break;
        }
        case JGH_LOW_TOM:
        {
            return 45;
            break;
        } 
        case JGH_SHAKER:
        {
            return 70;
            break;
        } 
        case JGH_HIGH_TOM:
        {
            return 38;
            break;
        } 
        case JGH_MID_TOM:
        {
            return 64;
            break;
        }
        default:
        {
            return 0;
        }

    }
};

void addNote()
{
    g_mutex.acquire();
    JGHNoteEvent * h = new JGHNoteEvent();
    h -> pitch = getCurrentDrum();
    h -> velocity = 1;
    
    if(Globals::isRecording)
    {
        Track *currTrack = getCurrentTrack();
        currTrack->addNote(h,currTrack ->nearestBeatDivision());
    }else
    {
        g_synth->playNotes(h);
    }
    g_mutex.release();
}
    
JGHNoteEvent *connectNotes(vector<JGHNoteEvent *> notes)
{
    if(notes.size() == 0) return NULL;
  
    JGHNoteEvent *currentNote = new JGHNoteEvent(notes[0]);
    JGHNoteEvent *firstNote = currentNote;
    for(int i = 1; i < notes.size() ; i++)
    {
        JGHNoteEvent *nextNote = new JGHNoteEvent(notes[i]);
        currentNote -> simultaneous = nextNote;
        currentNote = nextNote;
    }


    return firstNote;
}

JGHNoteEvent *g_metronomeNote;

//-----------------------------------------------------------------------------
// doTheBeat?
//-----------------------------------------------------------------------------
void doBeat(){
     g_mutex.acquire();
     vector<JGHNoteEvent *> notes; 
    for(int i = 0; i < g_tracks.size(); i++)
    {
        JGHNoteEvent *note = g_tracks[i] -> getNextNote();
        if(note)notes.push_back(note);
    }
    if(Globals::isMetronomeOn && Globals::currentBeatDivisorIndex == 0)
    {
        if(Globals::currentBeatIndex == 0)
        {
            g_metronomeNote -> velocity = 1;
        }else
        {
            g_metronomeNote -> velocity = .4;
        }
        notes.push_back(g_metronomeNote);
    }
    JGHNoteEvent *combinedNotes = connectNotes(notes);
    if (combinedNotes != NULL)
    { 
    
        g_synth-> playNotes(combinedNotes);
        SAFE_DELETE(combinedNotes);
    }

    g_mutex.release();
    calculateBeat();
}

//-----------------------------------------------------------------------------
// name: audio_callback
// desc: audio callback
//-----------------------------------------------------------------------------
static void audio_callback( SAMPLE * buffer, unsigned int numFrames, void * userData )
{


    //calculate beat
    if((int)g_currBeat > (int)Globals::samplesPerBeatDivisor)
    {
        g_currBeat = (int)Globals::samplesPerBeatDivisor % (int)(g_currBeat - Globals::samplesPerBeatDivisor);
        doBeat();
    }

    g_now += numFrames;
    g_currBeat += numFrames;
    
    // sum
    SAMPLE sum = 0;


    // num channels
    unsigned int channels = Globals::lastAudioBufferChannels;
    
    // zero out
    memset( Globals::lastAudioBuffer, 0,
           sizeof(SAMPLE)*Globals::lastAudioBufferFrames*channels );
    memset( Globals::lastAudioBufferMono, 0,
           sizeof(SAMPLE)*Globals::lastAudioBufferFrames );
    
    // copy to global buffer
    memcpy( Globals::lastAudioBuffer, buffer,
           sizeof(SAMPLE)*numFrames*channels );
    
    // copy to mono buffer
    for( int i = 0; i < numFrames; i++ )
    {
        // zero out
        sum = 0;
        // loop over channels
        for( int j = 0; j < channels; j++ )
        {
            // sum
            sum += buffer[i*channels + j];
        }
        // set
        Globals::lastAudioBufferMono[i] = sum / channels;
    }
    
    // window it
    for( int i = 0; i < numFrames; i++ )
    {
        // multiply
        Globals::lastAudioBufferMono[i] *= Globals::audioBufferWindow[i];
    }

    g_synth ->synthesize2(buffer, numFrames);

}

//setBPM
void setBPM(unsigned int BPM)
{
    Globals::BPM = BPM;
    Globals::samplesPerBeatDivisor = Globals::samplesPerMinute / (double)Globals::BPM/ (double)Globals::beatDivisor;
}

//-----------------------------------------------------------------------------
// name: jgh_audio_init()
// desc: initialize audio system
//-----------------------------------------------------------------------------
bool jgh_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels )
{
    // initialize
    if( !XAudioIO::init( 0, 0, srate, frameSize, channels, audio_callback, NULL ) )
    {
        // done
        return false;
    }

    //set BPM
    setBPM(DEFAULT_BPM);

    // allocate
    Globals::lastAudioBuffer = new SAMPLE[frameSize*channels];
    // allocate mono buffer
    Globals::lastAudioBufferMono = new SAMPLE[frameSize];
    // allocate window buffer
    Globals::audioBufferWindow = new SAMPLE[frameSize];
    // set frame size (could have changed in XAudioIO::init())
    Globals::lastAudioBufferFrames = frameSize;
    // set num channels
    Globals::lastAudioBufferChannels = channels;
    
    // compute the window
    hanning( Globals::audioBufferWindow, frameSize );
  
    g_synth = new JGHSynth();
    g_synth->init( srate,frameSize, 32, channels );
    g_synth->loadFont( "data/sfonts/TR-808_Drums.sf2", "" );
   
    for(int i = 0; i < Globals::numberOfTracks; i++)
    {
        Track *track = new Track(4);
        g_tracks.push_back(track);
    }

    g_metronomeNote = new JGHNoteEvent();
    g_metronomeNote-> pitch = 75;
    g_metronomeNote -> velocity = .3;
    g_metronomeNote -> duration = 1;
    
    return true;
}
Track* getTrack(unsigned int trackNumber)
{
    return g_tracks[trackNumber];
}



//-----------------------------------------------------------------------------
// name: vq_audio_start()
// desc: start audio system
//-----------------------------------------------------------------------------
bool jgh_audio_start()
{
    // start the audio
    if( !XAudioIO::start() )
    {
        // done
        return false;
    }
    
    return true;
}
