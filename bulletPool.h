#include <list>
#include "bullet.h"

class BulletPool
{
	public:
		static BulletPool& getInstance();

		const std::string bulletName() const {
			return name;
		}

		void draw() const;
		void update(Uint32 ticks);
		void shoot(const Vector2f &pos, const Vector2f &vel);

	private:
		std::string name;
		float frameInterval;
		float timeSinceLastFrame;
	  std::list<Bullet> bulletList;
	  std::list<Bullet> freeList;

		BulletPool();
		BulletPool(const BulletPool&);
		BulletPool &operator=(const BulletPool&);
		~BulletPool() {}
	
};
