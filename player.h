#include "ioManager.h"
#include "twowaysprite.h"
#include "bulletPool.h"

class Player: public TwowaySprite
{
	public:

		enum Status
		{
			UP = 0,
			UPLEFT,
			UPRIGHT,
			DOWN,
			DOWNLEFT,
			DOWNRIGHT,
			LEFT,
			RIGHT,
			STAND	
		};

		Player(const std::string&);

		virtual ~Player();
		virtual void draw() const;
  	virtual void update(Uint32 ticks);
  	virtual bool collidedWith(const Drawable *d) const;

		void shoot();
		void setStatus(Status s);
	
    bool hit(const Drawable *d);

	protected:
		Status status;
		BulletPool &bulletPool;
  	CollisionStrategy *strategy;
		unsigned int speedX;
		unsigned int speedY;

		Player(const Player&);
		Player& operator=(const Player&);
};