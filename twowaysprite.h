#include <string>
#include <vector>
#include "drawable.h"
#include "explodingSprite.h"


class TwowaySprite : public Drawable {
public:

  enum Direction {
    RIGHT = 0,
    LEFT
  };

  TwowaySprite(const std::string&);
  TwowaySprite(const TwowaySprite&);

  virtual ~TwowaySprite(); 
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[direction]; 
  }
  virtual void explode();

  Direction currentDirection() const {
    return direction;
  }

protected:
  ExplodingSprite* explosion;

  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  Direction direction;
  Direction lastDirection; // keep last direction frame
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);

  TwowaySprite& operator=(const TwowaySprite&); // Item 6
};
