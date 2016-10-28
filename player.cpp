#include <iostream>

#include "player.h"
#include "gamedata.h"

Player::Player(const std::string &name):
	TwowaySprite(name),
	status(STAND),
	speedX(abs(Gamedata::getInstance().getXmlInt(name + "/speedX"))),
	speedY(abs(Gamedata::getInstance().getXmlInt(name + "/speedY")))
{}

Player::~Player()
{}

void Player::draw() const
{
	TwowaySprite::draw();
}

void Player::update(Uint32 ticks)
{
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
}

void Player::setStatus(Status s)
{
	status = s;
	switch(status) {
		case UP:
			velocityX(0);
			velocityY(-abs(speedY));
			break;
		case DOWN:
			velocityX(0);
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
		case UPLEFT:
			velocityX(-abs(speedX));
			velocityY(-abs(speedY));
			break;
		case UPRIGHT:
			velocityX(speedX);
			velocityY(-abs(speedY));
			break;
		case DOWNLEFT:
			velocityX(-abs(speedX));
			velocityY(speedY);
			break;
		case DOWNRIGHT:
			velocityX(speedX);
			velocityY(speedY);
			break;
		case STAND:
		default:
			velocityX(0);
			velocityY(0);
			break;
	}
}
