#include <iostream>
#include <cmath>
#include "SDL/SDL_rotozoom.h"
#include "scaledSprite.h"
#include "gamedata.h"
#include "explodingSprite.h"
#include "viewport.h"

ScaledSprite::ScaledSprite(const std::string& name, 
                           SDL_Surface* surface) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/loc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/loc/y")), 
           Vector2f((rand()%2?1:-1)*getRandom(
                    Gamedata::getInstance().getXmlInt(name+"/cushion"),
                    Gamedata::getInstance().getXmlInt(name+"/speed/x")),
                    (rand()%2?1:-1)*getRandom(
                    Gamedata::getInstance().getXmlInt(name+"/cushion"),
                    Gamedata::getInstance().getXmlInt(name+"/speed/y"))
                   )
  ), 
  scale(getRandFloat(Gamedata::getInstance().getXmlFloat(name+"/scale/min"),
                     Gamedata::getInstance().getXmlFloat(name+"/scale/max"))
  ),
  scaledSurface( rotozoomSurface(surface, 0, scale, SMOOTHING_ON) ),
  explosion( NULL ),
  frame( new Frame(scaledSurface, scaledSurface->w, scaledSurface->h,
                   Gamedata::getInstance().getXmlInt(name+"/src/x"), 
                   Gamedata::getInstance().getXmlInt(name+"/src/y"))
  ),
  frameWidth(frame->getWidth()),
  frameHeight(frame->getHeight()),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

ScaledSprite::ScaledSprite(const ScaledSprite& s) :
  Drawable(s.getName(), s.getPosition(), s.getVelocity()), 
  scale(s.scale),
  scaledSurface(s.scaledSurface),
  explosion( s.explosion ),
  frame(s.frame),
  frameWidth(s.frameWidth),
  frameHeight(s.frameHeight),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight)
{ }

ScaledSprite& ScaledSprite::operator=(const ScaledSprite& rhs) {
  setName( rhs.getName() );
  setPosition(rhs.getPosition());
  setVelocity(rhs.getVelocity());
  scale = rhs.scale;
  scaledSurface = rhs.scaledSurface;
  frame = rhs.frame;
  frameWidth = rhs.frameWidth;
  frameHeight = rhs.frameHeight;
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

ScaledSprite::~ScaledSprite() {
  SDL_FreeSurface( scaledSurface );
  delete frame;
  if (explosion) delete explosion;
}

void ScaledSprite::draw() const { 
  if (explosion) {
    explosion -> draw();
    return;
  }

  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  frame->draw(x, y); 
}

unsigned ScaledSprite::getPixel(Uint32 i, Uint32 j) const { 
  Uint32 x = static_cast<Uint32>(X());
  Uint32 y = static_cast<Uint32>(Y());
  x = i - x;
  y = j - y;
  Uint32 *pixels = static_cast<Uint32 *>(frame->getSurface()->pixels);
  return pixels[ ( y * frame->getWidth() ) + x ];
}

float ScaledSprite::minScale() const {
  return Gamedata::getInstance().getXmlFloat(getName() + "/scale/min");
}

float ScaledSprite::maxScale() const {
  return Gamedata::getInstance().getXmlFloat(getName() + "/scale/max");
}

void ScaledSprite::update(Uint32 ticks) { 
  if (explosion) {
    explosion -> update(ticks);
    if (explosion -> chunkCount() == 0) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

  Vector2f incr = getVelocity() * scale * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( Y() < 0) {
    velocityY( abs( velocityY() ) );
  }
  if ( Y() > worldHeight-frameHeight) {
    setPosition( Vector2f(rand() % worldWidth, 0.0) );
    velocityY( -abs( velocityY() ) );
  }

  if ( X() < 0) {
    setPosition( Vector2f(rand() % worldWidth, 0.0) );
    velocityX( abs( velocityX() ) );
  }
  if ( X() > worldWidth-frameWidth) {
    velocityX( -abs( velocityX() ) );
  }  
}

void ScaledSprite::explode() {
  if (explosion) return;
  Sprite sprite(getName(), getPosition(), getVelocity(), getFrame());
  explosion = new ExplodingSprite(sprite); 
}

bool ScaledSprite::offscreen() const {
  Vector2f viewPos = Viewport::getInstance().getPosition();
  int viewWidth  = Gamedata::getInstance().getXmlInt("view/width");
  int viewHeight = Gamedata::getInstance().getXmlInt("view/height");
  
  if (X() < viewPos[0] || X() > viewPos[0] + viewWidth || 
      Y() < 0 || Y() > viewHeight) {
    return true;
  }

  return false;
}

