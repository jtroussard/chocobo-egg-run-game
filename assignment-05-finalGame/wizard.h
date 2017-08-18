/*
* University of Mary Washington
* Jacques Troussard
* Professor Polack
* CPSC 440
* 07/25/2017
*
* Assignment 5
* wizard.h
*
* Purpose: wizard/badguy header file
*/
#ifndef WIZARD_H
#define WIZARD_H

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "player.h"
using namespace std;

class Wizard
{
	friend int collided(int x, int y);
public:
	Wizard();
	void initWizard(string color, int spawnX, int spawnY, int level);
	void updateWizard(int px, int py, int w, int h); //dir 1 = right, 0 = left
	void drawWizard(int xoffset, int yoffset);
	bool collideWithPlayer(Player &p);
	float getX() { return x; }
	float getY() { return y; }
	int getWidth() { return frameWidth; }
	int getHeight() { return frameHeight; }
	void setX(int incomingX);
	void setY(int incomingY);
	void setLive(bool live);
	void testDraw(int x, int y);
	void destroy();
	void resetWizards();

private:
	float x;
	float y;
	float prevPosX; // hold previous x value for egg position
	float prevPosY; // hold previous y value for egg position

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationRows;
	int animationDirection;
	int badDirection; // used to prevent 'stupid' movement
	int speed;
	int updateCount;
	int updateDelay;
	int speedCounter;
	int stuckCounter;
	int dmgDelay;
	int dmgCounter;

	bool red;
	bool live;

	ALLEGRO_BITMAP *image;
};

#endif