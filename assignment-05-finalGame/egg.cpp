/*
* University of Mary Washington
* Jacques Troussard
* Professor Polack
* CPSC 440
* 07/25/2017
*
* Assignment 5
* egg.cpp
*
* Purpose: egg class file
*/
#include "egg.h"

Egg::Egg() {
	image = NULL;
}

void Egg::initEgg(int spawnX, int spawnY, int playerSpeed) {
	collected = false;

	x = spawnX;
	y = spawnY;
	int eggPosition = 1;
	angle = 0;

	frameHeight = frameWidth = 30;

	cout << "Loading Egg Object ..." << endl;
	image = al_load_bitmap("assets/sprites/egg.png");
	if (image != NULL) {
		cout << " COMPLETE." << endl;
	}
	else {
		cout << " *** ERROR LOADING EGG OBJECT ***" << endl;
	}
	speed = playerSpeed;
	safe = false;
}

void Egg::updateEgg(Player &p) {
	if (collected) {
		angle += 0.2;
		int dir = p.getAnimationDirection();
		int spacer = eggPosition * 25;
		if (dir == 0 || dir == 1) {
			y = p.getY() + (p.getfh() / 2);
			if (dir == 0) {
				x = p.getX() + spacer;
			}
			else if (dir == 1) {
				x = p.getX() - spacer;
			}
		}
		else if (dir == 2 || dir == 3) {
			x = p.getX() + (p.getfw() / 2);
			if (dir == 2) {
				y = p.getY() + (p.getfh() / 2) + spacer + 8;
			}
			else if (dir == 3) {
				y = p.getY() - spacer + (p.getfh() / 2);
			}
		}
	}
}

void Egg::drawEgg(int xoffset, int yoffset) {
	if (!collected) {
		//cout << x << " " << y << endl;
		al_draw_scaled_bitmap(image, 0,0, frameWidth, frameHeight, x-xoffset,y-yoffset, frameWidth * 2, frameHeight * 2, 0);
	}
	else if (collected && !safe){
		al_draw_rotated_bitmap(image, frameWidth / 2, frameHeight / 2, x - xoffset, y - yoffset, angle, 0);
	}
}

bool Egg::collideWithPlayer(Player &p, Egg eggs[], int size){
	int grace = 2;
	bool result = false;
	if (!collected)
	{
		if (x + frameWidth - grace > p.getX() &&
			p.getX() + p.getfw() - grace > x &&
			y + frameHeight - grace > p.getY() &&
			p.getY() + p.getfh() - grace > y){
			collected = true;
			y = p.getY();
			x = p.getX();
			for (int i = 0; i < size; i++)
			{
				if (eggs[i].isCollected() && !eggs[i].safe) {
					eggPosition++;
				}
				p.setNumEggs(eggPosition);
			}
			return true;
		}
	}
	return result;
}

void Egg::destroy() {
	al_destroy_bitmap(image);
}

bool Egg::isCollected(){
	return collected;
}

void Egg::collectEgg(){
	if (!collected) {
		collected = true;
	}
}

void Egg::setSafe(){
	safe = true;
}

bool Egg::isSafe() {
	return safe;
}