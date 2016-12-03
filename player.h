#include "ioManager.h"
#include "twowaysprite.h"
#include "bulletPool.h"
#include "health.h"

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
	void reset();
	void setStatus(Status s);
	void getHurt();
    void addScore();
    unsigned int getScore() const { return score; }
	
    bool hit(const Drawable *d);

	protected:
	Status status;
	Health health;
	BulletPool &bulletPool;
  	CollisionStrategy *strategy;
	unsigned int speedX;
	unsigned int speedY;
	unsigned int score;
	unsigned int bonus;

	Player(const Player&);
	Player& operator=(const Player&);
};