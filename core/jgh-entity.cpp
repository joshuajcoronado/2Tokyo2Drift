//-----------------------------------------------------------------------------
// name: jgh-entity.cpp
// desc: entities for bokeh visualization
//
// author: Joshua J Coronado (jjcorona@ccrma.stanford.edu)
//   date: 2013
//-----------------------------------------------------------------------------
#include "jgh-entity.h"
#include "jgh-globals.h"
#include "x-fun.h"
#include "jgh-audio.h"
#include "y-entity.h"
using namespace std;


// texture coordinates
static const GLshort g_coord[ ] = { 0, 0, 1, 0, 0, 1, 1, 1 };



//-------------------------------------------------------------------------------
// name: update()
// desc: ...
//-------------------------------------------------------------------------------
void JGHTeapot::update( YTimeInterval dt )
{

    if(Globals::onBeat){

    	this->col = Vector3D(XFun::rand2f(0, 1), XFun::rand2f(0, 1), XFun::rand2f(0, 1));
        Globals::onBeat = FALSE;
    }
    
}

// vertices
static GLfloat g_irisVerts[] = 
{
    -.1f, -.1f, 0.04f,
    .14f, .3f, -0.02f,
    -.1f, 0.8f, -0.06f
};

// normals
static const GLfloat g_normals[] = {
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1
};
void JGHIris::update(YTimeInterval dt)
{
        
}

void JGHIris::render()
{
    m_numBlades = connectedTrack -> beatLength * Globals::beatDivisor;

      // push
    glPushMatrix();
    
    // enable
    glEnable( GL_DEPTH_TEST );
    
    // enable
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
    
    // vertex
    glVertexPointer( 3, GL_FLOAT, 0, g_irisVerts );
    // normal
    glNormalPointer( GL_FLOAT, 0, g_normals );
    
    // find angle between each adjacent piece
    GLfloat angle = 360.0f / m_numBlades;
    
    // position
    GLfloat pos = (1 - m_position.value);
    
    // rotate the whole thing
    glRotatef( pos * 240, 0, 0, 1 );
    // set the blade length
    // g_irisVerts[7] = .5 + pos / 3.0f;
    // set the blade width
    g_irisVerts[3] = .14f * 36 / m_numBlades + pos*.2;
    
    // color
    glColor4f( col.x, col.y, col.z, 1 );
    // enable lighting
    glEnable( GL_LIGHTING );
    // loop overs
    for( int i = 0; i < m_numBlades; i++ )
    {
        // push
        glPushMatrix();
        // rotate
        glRotatef( -angle * i, 0, 0, 1 );
        // translate from center
        glTranslatef( .8f, 0.0f, 0.0f );
        // rotate
        glRotatef( pos * 90, 0, 0, 1 );

        if(connectedTrack -> getNextNote() ==NULL)
        {
            if(i == (int)  connectedTrack-> currentBeatIndex())
            {   
                glColor4f( 1, 1, 1, 1 );
            }else
            {
                glColor4f( col.x, col.y, col.z, 0 );
            }
        }else
        {
            if(i == (int)  connectedTrack-> currentBeatIndex())
            {   
                    glColor4f( col.x, col.y, col.z, 0 );
            }else
            {
                glColor4f( 1, 1, 1, 1 );
        
            }
        }
        // triangle strip
        glDrawArrays( GL_TRIANGLE_STRIP, 0, 3 );
        // pop
        glPopMatrix();
    }
    
    // outline color
    glColor4f( 0, 0, 0, 1 );
    // translate a bit
    glTranslatef( 0, 0, .001 );
    // linewidth
    glLineWidth( m_outlineWidth );
    // no lighting
    glDisable( GL_LIGHTING );
    // no normal
    glDisableClientState( GL_NORMAL_ARRAY );
    // // second pass for outline
    if(getCurrentTrack() == connectedTrack)
    {
         for( int i = 0; i < m_numBlades; i++ )
    {
        // push
        glPushMatrix();
        // rotate
        glRotatef( -angle * i, 0, 0, 1 );
        // translate from center
        glTranslatef( .8f, 0.0f, 0.0f );
        // rotate
        glRotatef( pos * 90, 0, 0, 1 );

        // triangle strip
        glDrawArrays( GL_LINE_LOOP, 0, 3 );
        // pop
        glPopMatrix();
    }
    

    }
   
    // disable
    glDisableClientState( GL_VERTEX_ARRAY );
    
    // pop
    glPopMatrix();
}


//-------------------------------------------------------------------------------
// name: render()
// desc: ...
//-------------------------------------------------------------------------------
void JGHTeapot::render()
{
    // enable lighting
    glEnable( GL_LIGHTING );
    // set color
    glColor4f( col.x, col.y, col.z, alpha );
    // render stuff
    glutSolidTeapot( 1.0 );
    // disable lighting
    glDisable( GL_LIGHTING );
}
