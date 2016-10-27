#include <string>
#include <vector>
#include "drawable.h"

enum Direction {
  RIGHT = 0,
  LEFT  = 1
};

class TwowaySprite : public Drawable {
public:
  TwowaySprite(const std::string&);
  TwowaySprite(const TwowaySprite&);
  virtual ~TwowaySprite() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual const Frame* getFrame() const { 
    return frames[direction]; 
  }

protected:
  const std::vector<Frame *> frames;
  int worldWidth;
  int worldHeight;

  Direction direction;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int frameWidth;
  int frameHeight;

  void advanceFrame(Uint32 ticks);
};
