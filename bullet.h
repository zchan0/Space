#include "drawable.h"
#include "collisionStrategy.h"

class Bullet: Drawable
{
	public:
		Bullet(const std::string &name, const Vector2f &pos, const Vector2f &vel);
		Bullet(const Bullet&);

		virtual ~Bullet() {}
		virtual void draw() const;
		virtual void update(Uint32 ticks);
  	virtual bool collidedWith(const Drawable *d) const;
		virtual const Frame *getFrame() const {
			return frame;
		}

		bool tooFar() const;
		void reset(const Vector2f &pos, const Vector2f &vel);

	private:
		const Frame *frame;
  	CollisionStrategy *strategy;
		int frameWidth, frameHeight;
		int worldWidth, worldHeight;

		Bullet operator=(const Bullet&);
};