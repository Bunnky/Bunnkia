#pragma once



class MiniMap
{
public:

	MiniMap();
	~MiniMap();

	short int X, Y;
	bool Docked;
    bool Size;

    bool On;

	void createBack();
	void draw();
	void drawFull();
};


