#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>

#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"

class ScaledSpriteCompare {
public:
  bool operator()(const ScaledSprite *lhs, const ScaledSprite *rhs) const {
    return lhs -> getScale() < rhs -> getScale();
  }
};

Manager::~Manager() { 
  // These deletions eliminate "definitely lost" and
  // "still reachable"s in Valgrind.
  
  SDL_FreeSurface(yellowstarSurface);

  for (unsigned int i = 0; i < worlds.size(); ++i) {
    delete worlds[i];
  }

  for (unsigned i = 0; i < sprites.size(); ++i) {
    delete sprites[i];
  }

  for (unsigned int i = 0; i < stars.size(); ++i) {
    delete stars[i];
  }

  stars.clear();
  worlds.clear();
  sprites.clear();
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  yellowstarSurface( io.loadAndSet(Gamedata::getInstance().getXmlStr("yellowstar/file"), Gamedata::getInstance().getXmlBool("yellowstar/transparency")) ),
  viewport( Viewport::getInstance() ),
  worlds(),
  sprites(),
  stars(),
  currentSprite(0),
  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") ),
  player( new Player("crab") ),
  hud( Hud::getInstance() )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);

  makeStars();
  // printStars();

  worlds.push_back( new World("back", Gamedata::getInstance().getXmlInt("back/factor")) ); 
  worlds.push_back( new World("mountain1", Gamedata::getInstance().getXmlInt("mountain1/factor")) ); 
  worlds.push_back( new World("mountain2", Gamedata::getInstance().getXmlInt("mountain2/factor")) ); 
  std::sort(worlds.begin(), worlds.end(), WorldFactorCompare());

  sprites.push_back( player );
  sprites.push_back( new MultiSprite("flyingbee") );
  sprites.push_back( new MultiSprite("runningboy") );
  sprites.push_back( new MultiSprite("runningman") );
  sprites.push_back( new MultiSprite("flyingbird") );
  sprites.push_back( new TwowaySprite("flappybird") );
  sprites.push_back( new TwowaySprite("pinkbird") );

  viewport.setObjectToTrack(sprites[currentSprite]);
}

void Manager::draw() const {
  
  //  to create depth, draw order matters
  //  create count = worlds.size() scale ranges
  float scale; 
  float minScale = stars[0] -> minScale();
  float maxScale = stars[0] -> maxScale();
  float stepScale = (maxScale - minScale) / worlds.size();

  for (unsigned int i = 0; i < worlds.size(); ++i) {
    worlds[i] -> draw();

    for (unsigned int j = 0; j < stars.size(); ++j) {
      scale = stars[j] -> getScale();
      if (scale > minScale + i * stepScale && scale < minScale + (i + 1) * stepScale ) 
        stars[j] -> draw();
    }
  }

  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->draw();
  }

  // io.printMessageAt(title, 3, 80);
  
  hud.draw();
  viewport.draw();

  SDL_Flip(screen);
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

void Manager::makeStars() {
  unsigned int numberOfStars = Gamedata::getInstance().getXmlInt("numberOfStars");
  stars.reserve(numberOfStars);
  for (unsigned int i = 0; i < numberOfStars; ++i) {
    stars.push_back( new ScaledSprite("yellowstar", yellowstarSurface) );
  }
  std::sort(stars.begin(), stars.end(), ScaledSpriteCompare());
}

void Manager::printStars() {
  for (unsigned int i = 0; i < stars.size(); ++i) {
    std::cout << stars[i] -> getScale() << std::endl;
  }
}

void Manager::update() {
  ++clock;
  Uint32 ticks = clock.getElapsedTicks();

  static unsigned int lastSeconds = clock.getSeconds();
  if ( clock.getSeconds() - lastSeconds == 5 ) {
    lastSeconds = clock.getSeconds();
    // switchSprite();
  }

  for (unsigned int i = 0; i < sprites.size(); ++i) {
    sprites[i]->update(ticks);
  }

  for (unsigned int i = 0; i < stars.size(); ++i) {
    stars[i] -> update(ticks);
  }

  if ( makeVideo && frameCount < frameMax ) {
    makeFrame();
  }

  for (unsigned int i = 0; i < worlds.size(); ++i) {
    worlds[i] -> update();
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
        // player's key
        if (keystate[SDLK_a]) {
          player -> setStatus(Player::LEFT);
        }
        if (keystate[SDLK_d]) {
          player -> setStatus(Player::RIGHT);
        }
        if (keystate[SDLK_w]) {
          player -> setStatus(Player::UP); 
        }
        if (keystate[SDLK_s]) {
          player -> setStatus(Player::DOWN);
        }
        if (keystate[SDLK_a] && keystate[SDLK_w]) {
          player -> setStatus(Player::UPLEFT);
        }
        if (keystate[SDLK_a] && keystate[SDLK_s]) {
          player -> setStatus(Player::DOWNLEFT);
        }
        if (keystate[SDLK_d] && keystate[SDLK_w]) {
          player -> setStatus(Player::UPRIGHT);
        }
        if (keystate[SDLK_d] && keystate[SDLK_s]) {
          player -> setStatus(Player::DOWNRIGHT);
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
