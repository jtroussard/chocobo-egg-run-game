/*
* University of Mary Washington
* Jacques Troussard
* Professor Polack
* CPSC 440
* 07/25/2017
*
* Assignment 5
* egg.h
*
* Purpose: egg header file
*/
#ifndef EGG_H
#define EGG_H

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "player.h"
using namespace std;

class Egg
{
	friend int collided(int x, int y);
public:
	Egg();
	void initEgg(int spawnX, int spawnY, int playerSpeed);
	void updateEgg(Player &p); //dir 1 = right, 0 = left
	void drawEgg(int xoffset, int yoffset);
	bool collideWithPlayer(Player &p, Egg eggs[], int size);
	float getX() { return x; }
	float getY() { return y; }
	int getWidth() { return frameWidth; }
	int getHeight() { return frameHeight; }
	void setX(int incomingX);
	void setY(int incomingY);
	void destroy();
	bool isCollected();
	void collectEgg();
	void setSafe();
	bool isSafe();

private:
	float x;
	float y;

	int frameWidth;
	int frameHeight;
	int speed;
	int eggPosition;
	float angle;

	bool collected;
	bool safe;
	
	ALLEGRO_BITMAP *image;
};

#endif