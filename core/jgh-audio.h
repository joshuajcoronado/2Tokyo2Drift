//-----------------------------------------------------------------------------
// name: jgh-audio.h
// desc: audio stuff
//
// author: Joshua J Coronado(jjcorona@ccrma.stanford.edu)
//   date: 2014
//-----------------------------------------------------------------------------
#ifndef __JGH_AUDIO_H__
#define __JGH_AUDIO_H__
#include "jgh-me.h"

// init audio
bool jgh_audio_init( unsigned int srate, unsigned int frameSize, unsigned channels );
// start audio
bool jgh_audio_start();

//getCurrentTrack();
Track* getCurrentTrack();
//get synth
JGHSynth *getSynth();
Track* getTrack(unsigned int trackNumber);

//addNote
void addNote();
#endif
