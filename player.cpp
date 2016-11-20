#include <iostream>

#include "player.h"
#include "gamedata.h"

Player::Player(const std::string &name):
	TwowaySprite(name),
	status(STAND),
	bulletPool(BulletPool::getInstance()),
	strategy(new PerPixelCollisionStrategy),
	speedX(abs(Gamedata::getInstance().getXmlInt(name + "/speedX"))),
	speedY(abs(Gamedata::getInstance().getXmlInt(name + "/speedY")))
{}

Player::~Player()
{
	delete strategy;
}

void Player::draw() const
{
	if (explosion) {
		explosion -> draw();
		return;
	}

	TwowaySprite::draw();
	bulletPool.draw();
}

void Player::update(Uint32 ticks)
{
	if (explosion) {
		explosion -> update(ticks);
		if (explosion -> chunkCount() == 0) {
			delete explosion;
			explosion = NULL;
		}
		return;
	}

	TwowaySprite::advanceFrame(ticks);	

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);
  
  if (Y() < 0) {
    velocityY(0);
    Y(0);
  }

  if (Y() > worldHeight - frameHeight) {
    velocityY(0);
    Y(worldHeight - frameHeight);
  }
  
  if (X() < 0) {
    velocityX(0);
    X(0);
  }

  if ( X() > worldWidth - frameWidth) {
    velocityX(0);
    X(worldWidth - frameWidth);
	}

	bulletPool.update(ticks);
}

void Player::setStatus(Status s)
{
	status = s;
	switch(status) {
		case UP:
			velocityX(0);
			velocityY(-abs(speedY));
			break;
		case UPLEFT:
			velocityX(-abs(speedX));
			velocityY(-abs(speedY));
			break;
		case UPRIGHT:
			velocityX(abs(speedX));
			velocityY(-abs(speedY));
			break;
		case DOWN:
			velocityX(0);
			velocityY(speedY);
			break;
		case DOWNLEFT:
			velocityX(-abs(speedX));
			velocityY(speedY);
			break;
		case DOWNRIGHT:
			velocityX(abs(speedX));
			velocityY(speedY);
			break;
		case LEFT:
			velocityX(-abs(speedX));
			velocityY(0);
			break;
		case RIGHT:
			velocityX(speedX);
			velocityY(0);
			break;
		case STAND: 
		default:
			velocityX(0);
			velocityY(0);
			break;
	}
}

void Player::reset()
{
	setStatus(STAND);
	setPosition(Vector2f(Gamedata::getInstance().getXmlInt(getName() + "/startLoc/x"), 
											 Gamedata::getInstance().getXmlInt(getName() + "/startLoc/y")));
}

void Player::shoot()
{
	bool towardLeft = currentDirection() == TwowaySprite::LEFT;
	float x = X() + (towardLeft ? 0 : 1) * frameWidth;
	float y = Y() + frameHeight / 2;
	float vx = (towardLeft ? -1.0 : 1.0) * Gamedata::getInstance().getXmlInt(bulletPool.bulletName() + "/speedX");
	bulletPool.shoot(Vector2f(x, y), Vector2f(vx, 0));
}

bool Player::collidedWith(const Drawable *d) const
{
	return strategy -> execute(*this, *d);
}

bool Player::hit(const Drawable *d)
{
	return bulletPool.collidedWith(d);	
}
