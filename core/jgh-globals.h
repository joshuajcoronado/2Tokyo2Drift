//-----------------------------------------------------------------------------
// name: jgh-globals.h
// desc: global stuffs
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------
#ifndef __JGH_GLOBALS_H__
#define __JGH_GLOBALS_H__


#include "x-def.h"
#include "x-audio.h"
#include "x-gfx.h"
#include "x-vector3d.h"
#include "y-waveform.h"

// c++
#include <string>
#include <map>
#include <vector>
#include <utility>

// defines
#define JGH_SRATE        44100
#define JGH_FRAMESIZE    512
#define JGH_NUMCHANNELS  2
#define JGH_MAX_TEXTURES 32
#define DEFAULT_BPM      120


//-----------------------------------------------------------------------------
// name: enum JoshGoHomeTextureNames
// desc: texture names
//-----------------------------------------------------------------------------
enum JoshGoHomeTextureNames
{
    JGH_TEX_NONE = 0,
    JGH_TEX_FLARE_TNG_1,
    JGH_TEX_FLARE_TNG_2,
    JGH_TEX_FLARE_TNG_3,
    JGH_TEX_FLARE_TNG_4,
    JGH_TEX_FLARE_TNG_5
};


enum JoshGoHomeTrackNames
{   
    JGH_KICK_DRUM = 0,
    JGH_CLAP,
    JGH_HIHAT,
    JGH_OPEN_HI,
    JGH_COWBELL,
    JGH_METRONOME,
    JGH_LOW_TOM,
    JGH_SHAKER,
    JGH_HIGH_TOM,
    JGH_MID_TOM
};




// forward reference
class JGHSim;




//-----------------------------------------------------------------------------
// name: class Globals
// desc: the global class
//-----------------------------------------------------------------------------
class Globals
{
public:
    // top level root simulation
    static JGHSim * sim;
    
    // path
    static std::string path;
    // path to datapath
    static std::string relpath;
    // datapath
    static std::string datapath;
    // version
    static std::string version;

    // last audio buffer
    static SAMPLE * lastAudioBuffer;
    static SAMPLE * lastAudioBufferMono;
    static SAMPLE * audioBufferWindow;
    static unsigned int lastAudioBufferFrames;
    static unsigned int lastAudioBufferChannels;

    //beats per minute
    static unsigned int BPM;
    static unsigned int LOW_BPM;
    static unsigned int HIGH_BPM;
    static double samplesPerBeat;
    static double samplesPerMinute;
    static bool onBeat;
    static unsigned int beatDivisor;
    static double samplesPerBeatDivisor;
    static unsigned int currentBeatDivisorIndex;
    static unsigned int currentBeatIndex;
    static unsigned int beatsPerMeasure;
    static unsigned int currentBeat;
    static unsigned int currentTrack;
    static bool isMetronomeOn;


    //controls
    static bool isRecording;
   
    static unsigned int numberOfTracks;


    // waveform
    static YWaveform * waveform;

    // width and height of the window
    static GLsizei windowWidth;
    static GLsizei windowHeight;
    static GLsizei lastWindowWidth;
    static GLsizei lastWindowHeight;

    // graphics fullscreen
    static GLboolean fullscreen;
    // render waveform
    static GLboolean renderWaveform;
    // blend pane instead of clearing screen
    static GLboolean blendScreen;
    // blend screen parameters
    static Vector3D blendAlpha;
    static GLfloat blendRed;
    // fill mode
    static GLenum fillmode;
    // background color
    static iSlew3D bgColor;
    // view stuff
    static Vector3D viewRadius;
    static Vector3D viewEyeY;
    static Vector3D fov;
    
    // textures
    static GLuint textures[];
    
    // light 0 position
    static GLfloat light0_pos[4];
    // light 1 parameters
    static GLfloat light1_ambient[4];
    static GLfloat light1_diffuse[4];
    static GLfloat light1_specular[4];
    static GLfloat light1_pos[4];
    // line width
    static GLfloat linewidth;
    // do
    static GLboolean fog;
    static GLuint fog_mode[4];   // storage for three/four types of fog
    static GLuint fog_filter;    // which fog to use
    static GLfloat fog_density;  // fog density
    
    // colors
    static Vector3D ourWhite;
    static Vector3D ourRed;
    static Vector3D ourBlue;
    static Vector3D ourOrange;
    static Vector3D ourGreen;
    static Vector3D ourGray;
    static Vector3D ourYellow;
    static Vector3D ourSoftYellow;
    static Vector3D ourPurple;

};




#endif
