/*
* @Author: zhengcc
* @Date:   2016-11-18 15:35:20
* @Last Modified by:   zhengcc
* @Last Modified time: 2016-11-18 22:33:15
*/

#include "bullet.h"
#include "viewport.h"
#include "gamedata.h"
#include "frameFactory.h"

Bullet::Bullet(const std::string &name, const Vector2f &pos, const Vector2f &vel):
	Drawable(name, pos, vel),
	frame(FrameFactory::getInstance().getFrame(name)),
	frameWidth(frame -> getWidth()),
	frameHeight(frame -> getHeight()),
	worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
	worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{}

Bullet::Bullet(const Bullet& s): 
	Drawable(s),
	frame(s.frame),
	frameWidth(s.frameWidth),
	frameHeight(s.frameHeight),
	worldWidth(s.worldWidth),
	worldHeight(s.worldHeight)
{}

void Bullet::draw() const 
{
	Uint32 x = static_cast<Uint32>(X());
	Uint32 y = static_cast<Uint32>(Y());
	frame -> draw(x, y);
}

void Bullet::update(Uint32 ticks)
{
	Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
	setPosition(getPosition() + incr);
}

bool Bullet::tooFar() const
{
	return (Viewport::getInstance().getPosition())[0] + Gamedata::getInstance().getXmlInt("view/width") < X();
}

void Bullet::reset(const Vector2f &pos, const Vector2f &vel)
{
	setPosition(pos);
	setVelocity(vel);	
}


