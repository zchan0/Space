#include "SDL/SDL_rotozoom.h"
#include "drawable.h"
#include "frame.h"
#include "ioManager.h"
#include "viewport.h"

Frame::Frame( SDL_Surface* surf ) : 
  screen(IOManager::getInstance().getScreen()),
  surface( surf ),
  width(surf->w), 
  height(surf->h),
  spriteSourceX(0),
  spriteSourceY(0)
{ }

Frame::Frame( const Frame& frame ) :
  screen(frame.screen),
  surface(frame.surface), 
  width(surface->w), 
  height(surface->h),
  spriteSourceX(frame.spriteSourceX),
  spriteSourceY(frame.spriteSourceY)
{ }

Frame::Frame( SDL_Surface* spr, Uint16 sprite_width, Uint16 sprite_height,
              Sint16 src_x, Sint16 src_y) :
  screen(IOManager::getInstance().getScreen()),
  surface(spr), 
  width(sprite_width), 
  height(sprite_height),
  spriteSourceX(src_x), 
  spriteSourceY(src_y) 
{ }

Frame& Frame::operator=(const Frame& rhs) {
  surface = rhs.surface;
  screen = rhs.screen;
  width = surface->w;
  height = surface->h;
  spriteSourceX = rhs.spriteSourceX;
  spriteSourceY = rhs.spriteSourceY;
  return *this;
}

void Frame::draw(Sint16 x, Sint16 y) const {
  SDL_Rect src = { 0, 0, width, height };    
  x -= Viewport::getInstance().X();
  y -= Viewport::getInstance().Y();
  SDL_Rect dest = {x, y, width, height };
  SDL_BlitSurface(surface, &src, screen, &dest);
}

void Frame::draw(Sint16 sx, Sint16 sy, Sint16 dx, Sint16 dy) const {
  SDL_Rect src = { sx, sy, width, height };    
  SDL_Rect dest = {dx, dy, width, height };
  SDL_BlitSurface(surface, &src, screen, &dest);
}

void Frame::draw(Sint16 x, Sint16 y, double angle) const {
  SDL_Surface* tmp = rotozoomSurface(surface, angle, 1, 1);
  Sint16 zero = 0;
  Uint16 width = tmp->w;
  Uint16 height = tmp->h;
  SDL_Rect src = { zero, zero, width, height };    
  x -= Viewport::getInstance().X();
  y -= Viewport::getInstance().Y();
  SDL_Rect dest = {x, y, 0, 0 };
  SDL_BlitSurface(tmp, &src, screen, &dest);
  SDL_FreeSurface( tmp );
}

