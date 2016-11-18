#include <iostream>
#include "twowaysprite.h"
#include "gamedata.h"
#include "frameFactory.h"
#include "explodingSprite.h"

void TwowaySprite::advanceFrame(Uint32 ticks) {
  timeSinceLastFrame += ticks;
  if (timeSinceLastFrame > frameInterval) {
    if (velocityX() == 0) {
      direction = lastDirection;
    } else {
      direction = velocityX() > 0 ? RIGHT : LEFT;
      lastDirection = direction;
    }
    timeSinceLastFrame = 0;
  }
}

TwowaySprite::TwowaySprite( const std::string& name) :
  Drawable(name, 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  explosion(NULL),
  frames( FrameFactory::getInstance().getFrames(name) ),
  worldWidth( Gamedata::getInstance().getXmlInt("world/width") ),
  worldHeight( Gamedata::getInstance().getXmlInt("world/height") ),
 
  direction(RIGHT),
  lastDirection(RIGHT),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval") ),
  timeSinceLastFrame(0),
  frameWidth(frames[0]->getWidth()),
  frameHeight(frames[0]->getHeight())
{ }

TwowaySprite::TwowaySprite(const TwowaySprite& s) :
  Drawable(s), 
  explosion(s.explosion),
  frames(s.frames),
  worldWidth( s.worldWidth ),
  worldHeight( s.worldHeight ),
  direction( s.direction ),
  lastDirection( s.lastDirection ),

  numberOfFrames( s.numberOfFrames ),
  frameInterval( s.frameInterval ),
  timeSinceLastFrame( s.timeSinceLastFrame ),
  frameWidth( s.frameWidth ),
  frameHeight( s.frameHeight )
  { }

TwowaySprite::~TwowaySprite() {
  if (explosion) delete explosion;
}

void TwowaySprite::draw() const {
  if (explosion) {
    explosion -> draw();
    return;
  }

  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frames[direction]->draw(x, y);
}

void TwowaySprite::update(Uint32 ticks) { 
  if (explosion) {
    explosion -> update(ticks); 
    if (explosion -> chunkCount() == 0) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
}

void TwowaySprite::explode() {
  if (explosion) return;
  Sprite sprite( getName(), getPosition(), getVelocity(), getFrame() );
  explosion = new ExplodingSprite(sprite); 
}
