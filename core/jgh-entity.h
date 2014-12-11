//-----------------------------------------------------------------------------
// name: jgh-entity.h
// desc: entities for visquin visualization
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#ifndef __JGH_ENTITY_H__
#define __JGH_ENTITY_H__

#include "y-entity.h"
#include "jgh-me.h"
#include "x-buffer.h"
#include <vector>


//-----------------------------------------------------------------------------
// name: class jghTeapot
// desc: for testing
//-----------------------------------------------------------------------------
class JGHTeapot : public YEntity
{
public:
    // update
    void update( YTimeInterval dt );
    // render
    void render();
};



class JGHIris: public YIris
{
public:
	
	Track *connectedTrack;

public:
	void update(YTimeInterval dt);
	void render();

};

#endif














