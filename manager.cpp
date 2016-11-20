#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>

#include "manager.h"
#include "sprite.h"
#include "gamedata.h"

class ScaledSpriteCompare {
public:
  bool operator()(const ScaledSprite *lhs, const ScaledSprite *rhs) const {
    return lhs -> getScale() < rhs -> getScale();
  }
};

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  
  SDL_FreeSurface(asteroidSurface);

  for (unsigned int i = 0; i < worlds.size(); ++i) {
    delete worlds[i];
  }

  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }

  for (unsigned int i = 0; i < asteroids.size(); ++i) {
    delete asteroids[i];
  }

  asteroids.clear();
  worlds.clear();
  sprites.clear();
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  asteroidSurface( io.loadAndSet(Gamedata::getInstance().getXmlStr("asteroid/file"), Gamedata::getInstance().getXmlBool("asteroid/transparency")) ),
  viewport( Viewport::getInstance() ),
  worlds(),
  sprites(),
  asteroids(),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  player( new Player("ship") ),
  hud( Hud::getInstance() )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);

  makeAsteroids();
  // printAsteroids();

  worlds.push_back( new World("back1", Gamedata::getInstance().getXmlInt("back1/factor")) ); 
  worlds.push_back( new World("back2", Gamedata::getInstance().getXmlInt("back2/factor")) ); 
  worlds.push_back( new World("back3", Gamedata::getInstance().getXmlInt("back3/factor")) ); 
  std::sort(worlds.begin(), worlds.end(), WorldFactorCompare());

  player -> setStatus(Player::STAND);
  sprites.push_back( player );

  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::makeFrame() {
  std::stringstream strm;
  strm << "frames/" << username<< '.' 
       << std::setfill('0') << std::setw(4) 
       << frameCount++ << ".bmp";
  std::string filename( strm.str() );
  std::cout << "Making frame: " << filename << std::endl;
  SDL_SaveBMP(screen, filename.c_str());
}

void Manager::switchSprite() {
  currentSprite = (currentSprite+1) % sprites.size();
  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::makeAsteroids() {
  unsigned int numberOfAsteroids = Gamedata::getInstance().getXmlInt("numberOfAsteroids");
  asteroids.reserve(numberOfAsteroids);
  for (unsigned int i = 0; i < numberOfAsteroids; ++i) {
    asteroids.push_back( new ScaledSprite("asteroid", asteroidSurface) );
  }
  std::sort(asteroids.begin(), asteroids.end(), ScaledSpriteCompare());
}

void Manager::printAsteroids() {
  for (unsigned int i = 0; i < asteroids.size(); ++i) {
    std::cout << asteroids[i] -> getScale() << std::endl;
  }
}

void Manager::checkForCollisions() const {
  std::vector<ScaledSprite *>::const_iterator ptr = asteroids.begin(); 
  while(ptr != asteroids.end()) {
    // ignore those which not on screen
    if ((*ptr) -> offscreen()) {
      ++ptr;
      continue;
    }
    
    // player got hurt
    if (player -> collidedWith(*ptr)) {
      // player -> explode();
    }
    
    // player shoot ateriod
    if (player -> hit(*ptr)) {
      (*ptr) -> explode();
      return; // one bullet hit one ateriod
    }
    
    ++ptr;
  }
}

void Manager::draw() const {
  //  to create depth, draw order matters
  //  create count = worlds.size() scale ranges
  float scale; 
  float minScale = asteroids[0] -> minScale();
  float maxScale = asteroids[0] -> maxScale();
  float stepScale = (maxScale - minScale) / worlds.size();

  for (unsigned int i = 0; i < worlds.size(); ++i) {
    worlds[i] -> draw();

    for (unsigned int j = 0; j < asteroids.size(); ++j) {
      scale = asteroids[j] -> getScale();
      if (scale > minScale + i * stepScale && scale < minScale + (i + 1) * stepScale ) {
        asteroids[j] -> draw();
      } 
    }
  }

  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }

  io.printMessageAt(title, 30, 650);

  checkForCollisions();
  
  hud.draw();
  viewport.draw();

  SDL_Flip(screen);
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();

  // update sprites
  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }
  for (unsigned int i = 0; i < asteroids.size(); ++i) {
    asteroids[i] -> update(ticks);
  }
  for (unsigned int i = 0; i < worlds.size(); ++i) {
    worlds[i] -> update();
  }

  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }

  hud.update(ticks);
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { 
        done = true; break; 
      }

      // No key input, set player to STAND status
      if (event.type == SDL_KEYUP) {
        player -> setStatus(Player::STAND);
      }

      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_t] ) {
          switchSprite();
        }
        if ( keystate[SDLK_l] ) {
          clock.toggleSloMo();
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDLK_r] ) {
          player -> reset();
        }
        // player's key
        // direction
        if (keystate[SDLK_a]) {
          player -> setStatus(Player::LEFT);
        }
        if (keystate[SDLK_d]) {
          player -> setStatus(Player::RIGHT);
        }
        if (keystate[SDLK_a] && keystate[SDLK_d]) {
          player -> setStatus(Player::STAND);
        }

        if (keystate[SDLK_w]) {
          player -> setStatus(Player::UP); 
        }
        if (keystate[SDLK_w] && keystate[SDLK_a]) {
          player -> setStatus(Player::UPLEFT);
        }
        if (keystate[SDLK_w] && keystate[SDLK_d]) {
          player -> setStatus(Player::UPRIGHT);
        }
        if (keystate[SDLK_s]) {
          player -> setStatus(Player::DOWN);
        }
        if (keystate[SDLK_s] && keystate[SDLK_a]) {
          player -> setStatus(Player::DOWNLEFT);
        }
        if (keystate[SDLK_s] && keystate[SDLK_d]) {
          player -> setStatus(Player::DOWNRIGHT);
        }
        if (keystate[SDLK_w] && keystate[SDLK_s]) {
          player -> setStatus(Player::STAND);
        }
        // action
        if (keystate[SDLK_SPACE]) {
          player -> setStatus(Player::STAND);
          player -> shoot(); 
        }
        // F key
        if (keystate[SDLK_F1]) {
          hud.toggle();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
      }
    }
    draw();
    update();
  }
}
