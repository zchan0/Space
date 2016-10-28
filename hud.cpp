#include <iostream>

#include "hud.h"
#include "clock.h"
#include "aaline.h"
#include "gamedata.h"

Hud& Hud::getInstance()
{
	static Hud instance("hud");
	return instance;
}

Hud::Hud(const std::string &name):
	io(IOManager::getInstance()),
	screen(io.getScreen()),
	showHud(true),
	elapsedTime(0.0),
	showTime(Gamedata::getInstance().getXmlInt(name + "/showTime")),
	startX(Gamedata::getInstance().getXmlInt(name + "/startX")),
	startY(Gamedata::getInstance().getXmlInt(name + "/startY")),
	width(Gamedata::getInstance().getXmlInt(name + "/width")),
	height(Gamedata::getInstance().getXmlInt(name + "/height")),
	thick(Gamedata::getInstance().getXmlFloat(name + "/thick"))
{}

void Hud::drawBox() const
{
	const Uint32 borderColor = SDL_MapRGB(screen -> format, 255, 0, 0);
	// Horizontal lines
	Draw_AALine(screen, startX, startY, 
											startX + width, startY,  
                      thick, borderColor);
	Draw_AALine(screen, startX, startY + height, 
											startX + width, startY + height,  
                      thick, borderColor);
  
  // Vertical lines
  Draw_AALine(screen, startX, startY, 
											startX, startY + height,  
                      thick, borderColor);
  Draw_AALine(screen, startX + width, startY, 
											startX + width, startY + height,  
                      thick, borderColor);
  // Fill
  Draw_AALine(screen, startX, startY + height / 2, 
                      startX + width, startY + height / 2, 
                      height, 0xff, 0xff, 0xff, 0x99);
}

void Hud::draw() const
{
	if (showHud) {
		drawBox();
  	// Print infomation
  	io.printMessageValueAt("Seconds: ", Clock::getInstance().getSeconds(), 30, 30);
    io.printMessageValueAt("Fps: ", Clock::getInstance().getFps(), 30, 50);
    io.printMessageAt("W - move up",    30, 90);
    io.printMessageAt("S - move down",  30, 110);
    io.printMessageAt("A - move left",  30, 130);
  	io.printMessageAt("D - move right", 30, 150);
    io.printMessageAt("l - slow; p - pasue", 30, 190);	
	} else {
    io.printMessageAt("F1 Help", 30, 40);
	}
}

void Hud::update(Uint32 ticks)
{
	elapsedTime += ticks;
	if (showHud && elapsedTime > showTime) 
		toggle();
}

void Hud::toggle()
{
	showHud = !showHud;
	elapsedTime = 0.0;
}




