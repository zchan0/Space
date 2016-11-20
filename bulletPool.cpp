/*
* @Author: zhengcc
* @Date:   2016-11-18 15:35:35
* @Last Modified by:   zhengcc
* @Last Modified time: 2016-11-19 21:32:31
*/

#include "bulletPool.h"
#include "gamedata.h"

BulletPool& BulletPool::getInstance()
{
	static BulletPool instance;
	return instance;
} 

BulletPool::BulletPool(): 
	name("Bullet"),
	frameInterval(Gamedata::getInstance().getXmlInt(name + "/frameInterval")),
	timeSinceLastFrame(0),
	bulletList(),
	freeList()
{}

void BulletPool::draw() const
{
	std::list<Bullet>::const_iterator ptr = bulletList.begin();
	while(ptr != bulletList.end()) {
		ptr -> draw();
		++ptr;
	}
}

void BulletPool::update(Uint32 ticks)
{
	timeSinceLastFrame += ticks;

	std::list<Bullet>::iterator ptr = bulletList.begin();
	while(ptr != bulletList.end()) {
		ptr -> update(ticks);
		if (ptr -> tooFar()) {
			freeList.push_back((*ptr));
			ptr = bulletList.erase(ptr);
		} else {
			++ptr;
		}
	}
}

void BulletPool::shoot(const Vector2f &pos, const Vector2f &vel)
{
	if (timeSinceLastFrame > frameInterval) {
		if (freeList.empty()) {
			Bullet bullet(name, pos, vel);
			bulletList.push_back(bullet);
		} else {
			freeList.begin() -> reset(pos, vel);
			bulletList.push_back(*freeList.begin());
			freeList.erase(freeList.begin());
		}
	}
	timeSinceLastFrame = 0;
}

bool BulletPool::collidedWith(const Drawable *d)
{
	std::list<Bullet>::iterator ptr = bulletList.begin();
	
	while(ptr != bulletList.end()) {
		if (ptr -> tooFar()) 
			continue;
		
		if (ptr -> collidedWith(d)) {
			freeList.push_back(*ptr);
			ptr = bulletList.erase(ptr);
			return true;
		} else {
			++ptr;
		} 
	}

	return false;
}

unsigned int BulletPool::bulletCount() const
{
	return bulletList.size();
}

unsigned int BulletPool::freeCount() const
{
	return freeList.size();
}


