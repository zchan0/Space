#include <string>
#include "ioManager.h"
#include "frame.h"
#include "viewport.h"

class World {
public:
  World(const std::string& name, int fact);
  // ~World() { ; } // The frame will be deleted by the FrameFactory
  void update();
  void draw() const;

  int getFactor() const;

private:
  const IOManager& io;
  Frame* const frame;
  int factor;
  unsigned frameWidth;
  unsigned worldWidth;
  float viewX;
  float viewY;
  const Viewport & view;
  World(const World&);
  World& operator=(const World&);
};

class WorldFactorCompare {
public:
  bool operator()(const World *lhs, const World *rhs) const {
    return lhs -> getFactor() > rhs -> getFactor();  
  }
};