#include "stdafx.h"

class FPS
{ 
public:
	FPS() : fFrame(0), fFps(0) {}
	const unsigned int getFPS() const { return fFps; }

private:
	unsigned int fFrame;
	unsigned int fFps;
	sf::Clock fClock;



public:
	void update()
	{
		if(fClock.getElapsedTime().asSeconds() >= 1.f)
		{
			fFps = fFrame;
			fFrame = 0;
			fClock.restart();
		}
 
		++fFrame;
	}
};