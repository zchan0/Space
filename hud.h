#include "ioManager.h"

class Hud
{
	public:
		static Hud& getInstance();
		void draw() const;
		void update(Uint32);
		void toggle();

	private:
		const IOManager &io;
		SDL_Surface *screen;
		bool showHud;
		double elapsedTime;
		unsigned showTime;
		unsigned startX;
		unsigned startY;
		unsigned width;
		unsigned height;
		unsigned thick;

		Hud(const std::string &name);
		Hud(const Hud&);
		Hud& operator=(const Hud&);
  	void drawBox() const;
};