#include <vector>
#include <SDL.h>
#include "ioManager.h"
#include "world.h"
#include "viewport.h"
#include "clock.h"
#include "hud.h"
#include "player.h"
#include "scaledSprite.h"

class Manager {
public:
  Manager ();
  ~Manager ();
  void play();
  void switchSprite();

private:
  const bool env;
  const IOManager& io;
  Clock& clock;

  SDL_Surface * const screen;
  SDL_Surface * const yellowstarSurface;
  Viewport& viewport;

  std::vector<World*> worlds;
  std::vector<Drawable*> sprites;
  std::vector<ScaledSprite*> stars;

  int currentSprite;
  bool makeVideo;
  int frameCount;
  const std::string username;
  const std::string title;
  const int frameMax;

  Player *player;
  Hud &hud;
  
  void draw() const;
  void update();
  void makeStars();
  void printStars();

  Manager(const Manager&);
  Manager& operator=(const Manager&);
  void makeFrame();
};
