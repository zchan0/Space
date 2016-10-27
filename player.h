#include "ioManager.h"
#include "twowaysprite.h"

class Player: public TwowaySprite
{
	public:

		enum Status
		{
			UP = 0,
			DOWN,
			LEFT,
			RIGHT,
			UPLEFT,
			UPRIGHT,
			DOWNLEFT,
			DOWNRIGHT,
			STAND	
		};

		Player(const std::string&);

		virtual ~Player();
		virtual void draw() const;
  	virtual void update(Uint32 ticks);

		void setStatus(Status s);
	
	protected:
		Status status;
		unsigned int speedX;
		unsigned int speedY;

		Player(const Player&);
		Player& operator=(const Player&);
};