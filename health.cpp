#include "health.h"
#include "gamedata.h"

Health::Health(const std::string & name) :
  screen(IOManager::getInstance().getScreen()),
  start(Vector2f(Gamedata::getInstance().getXmlInt(name+"/x"), 
      Gamedata::getInstance().getXmlInt(name+"/y"))),
  totalLength(Gamedata::getInstance().getXmlInt(name+"/length")),
  currentLength(totalLength),
  thick(Gamedata::getInstance().getXmlFloat(name+"/thick")),
  increments(Gamedata::getInstance().getXmlFloat(name+"/increment")),
  interval(Gamedata::getInstance().getXmlFloat(name+"/interval")),
  deltaTime(0),
  RED( SDL_MapRGB(screen->format, 0xff, 0x00, 0x00) ),
  GRAY( SDL_MapRGB(screen->format, 0xce, 0xb4, 0xb4) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
  color(RED),
  hurt(Gamedata::getInstance().getXmlInt(name+"/hurt"))
{
}

Health::Health(int sx, int sy, int tl, int cl,
               float t, int inc, Uint32 c, float sp):
  screen(IOManager::getInstance().getScreen()),
  start(Vector2f(sx, sy)), 
  totalLength(tl), 
  currentLength(cl), 
  thick(t), 
  increments(inc),
  interval(sp),
  deltaTime(SDL_GetTicks()),
  RED( SDL_MapRGB(screen->format, 0xff, 0x00, 0x00) ),
  GRAY( SDL_MapRGB(screen->format, 0xff, 0xff, 0xff) ),
  BLACK( SDL_MapRGB(screen->format, 0x00, 0x00, 0x00) ),
    color(c),
  hurt(40)
{
}

void Health::drawBox() const {
  Draw_AALine(screen, start[0], start[1], 
                      start[0]+totalLength, start[1], 
                      thick, GRAY);
  // Two Horizontal lines
  Draw_AALine(screen, start[0], start[1]-8, 
                      start[0]+totalLength, start[1]-8, 
                      1.0, BLACK);
  Draw_AALine(screen, start[0], start[1]+8, 
                      start[0]+totalLength, start[1]+8, 
                      1.0, BLACK);
  // Two Vertical lines
  Draw_AALine(screen, start[0]-1, start[1]-8, 
                      start[0]-1, start[1]+8, 
                      2.0, BLACK);
  Draw_AALine(screen, start[0]+totalLength+1, start[1]-8, 
                      start[0]+totalLength+1, start[1]+8, 
                      2.0, BLACK);
}

void Health::draw() const {
  drawBox();
  Draw_AALine(screen, start[0], start[1], 
                      start[0]+currentLength, start[1], 
                      thick, color);
}

void Health::update(Uint32 ticks) {
  deltaTime += ticks;
  if ( currentLength == 0 && deltaTime > interval ) {
    deltaTime = 0;
    //reset();
  }
}

void Health::updateValue(int val) {
  currentLength += val;
  if(currentLength < 0) currentLength = 0;
  if(currentLength > totalLength) currentLength = totalLength;
}

void Health::getHurt()
{
    currentLength -= hurt;
    if(currentLength < 0) currentLength = 0;
    if(currentLength > totalLength) currentLength = totalLength;
}

