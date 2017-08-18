/*
* University of Mary Washington
* Jacques Troussard
* Professor Polack
* CPSC 440
* 07/25/2017
*
* Assignment 5
* wizard.cpp
*
* Purpose: wizard/badguy class file
*/
#include "wizard.h"

Wizard::Wizard() {
	image = NULL;
}

void Wizard::initWizard(string color, int spawnX, int spawnY, int level) {
	live = false;
	maxFrame = 2;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 21;
	frameHeight = 24;
	animationColumns = 2;
	animationDirection = 1;
	updateCount = 0;
	dmgCounter = 0;
	dmgDelay = 12;

	x = spawnX;
	y = spawnY;

	if (color == "blue") {
		image = al_load_bitmap("assets/sprites/bluewizard.png");
		//image = al_load_bitmap("assets/sprites/egg.png");
		speed = level;
		updateDelay = 6;
	}
	else {
		image = al_load_bitmap("assets/sprites/redwizard.png");
		speed = level + 2;
		updateDelay = 18;
		red = true;
	}
}

void Wizard::updateWizard(int playerX, int playerY, int w, int h) {
	if (live) {
		int dir = animationDirection;
		if (updateCount == updateDelay) {
			if (stuckCounter == 30) {
				x = rand() % w;
				y = rand() % h;
				stuckCounter == 0;
			}
			updateCount = 0;
			int badLuck = 0;
			do {
				dir = rand() % 4;
				badLuck++;
				if (badLuck == 30) {
					x = rand() % w;
					y = rand() % h;
					stuckCounter == 0;
				}
			} while (dir == badDirection);

			bool follow = false;
			if (rand() % 3 == 0) {
				//cout << "follow --";
				int px = playerX;
				int py = playerY;
				if (dir < 2 && px > x && badDirection != 1) {
					dir = 1;
				}
				else if (dir < 2 && px < x && badDirection != 0) {
					dir = 0;
				}

				if (dir > 1 && py > y && badDirection != 3) {
					dir = 3;
				}
				else if (dir > 1 && py < y && badDirection != 2) {
					dir = 2;
				}
				//cout << "dir:" << dir << " - bad:" << badDirection << " - px/py:" << px << " " << py << " - x/y" << x << " " << y << endl;

			}
		}
		else {
			//cout << updateDelay << "    " << updateCount << endl;;
			updateCount++;
		}

		int oldx = x;
		int oldy = y;
		prevPosX = oldx;
		prevPosY = oldy;

		if (speedCounter == 0) {
			if (dir == 1){ //right key
				animationDirection = 1;
				x += speed;
				if (++frameCount > frameDelay)
				{
					frameCount = 0;
					if (++curFrame > maxFrame)
						curFrame = 1;
				}
			}
			else if (dir == 0){ //left key
				animationDirection = 0;
				x -= speed;
				if (++frameCount > frameDelay)
				{
					frameCount = 0;
					if (++curFrame > maxFrame)
						curFrame = 1;
				}
			}
			else if (dir == 2) { //update up
				animationDirection = 2;
				y -= speed;
				if (++frameCount > frameDelay){
					frameCount = 0;
					if (++curFrame > maxFrame) {
						curFrame = 0;
					}
				}
			}
			else if (dir == 3) {// update down
				animationDirection = 3;
				y += speed;
				if (++frameCount > frameDelay) {
					frameCount = 0;
					if (++curFrame > maxFrame) {
						curFrame = 0;
					}
				}
			}
			else{
				animationDirection = dir;
			}
		}
		else {
			speedCounter--;
		}

		//check for collided with foreground tiles
		badDirection = -1;
		if (animationDirection == 0)
		{
			if (collided(x, y + frameHeight)) { //collision detection to the left
				x = oldx;
				y = oldy;
				badDirection = animationDirection;
				stuckCounter++;
			}

		}
		else if (animationDirection == 1)
		{
			if (collided(x + frameWidth, y + frameHeight)) { //collision detection to the right
				x = oldx;
				y = oldy;
				badDirection = animationDirection;
				stuckCounter++;
			}
		}
		else if (animationDirection == 2) { // check for collision when moving up
			if (collided(x, y + frameHeight)) {
				x = oldx;
				y = oldy;
				badDirection = animationDirection;
				stuckCounter++;

			}
		}
		else if (animationDirection == 3) {
			if (collided(x, y + frameHeight)) {
				x = oldx;
				y = oldy;
				badDirection = animationDirection;
				stuckCounter++;

			}
		}
	}
else {
	x = x;
	y = y;
}
}

void Wizard::drawWizard(int xoffset, int yoffset) {
	if (live) {
	
		int fx = 0;//(curFrame % animationColumns) * frameWidth;
		int fy = (curFrame / animationColumns) * frameHeight;
		if (animationDirection < 2) {
			fy = 48; // left to right
		}
		else if (animationDirection == 2) {
			fy = 24; // up
		}
		else if (animationDirection == 3) {
			fy = 0; // down
		}

		if (animationDirection == 0) { // row 2 no modifications
			al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
		}
		else if (animationDirection == 1){ // draw row 2 pointing to the right
			al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, ALLEGRO_FLIP_HORIZONTAL);
		}
		else if (animationDirection == 2){ // draw row 1 no modifications
			al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
		}
		else if (animationDirection == 3){ // draw row 0 no modifications
			al_draw_bitmap_region(image, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
		}
	}
}

bool Wizard::collideWithPlayer(Player &p){
	int grace = 2;
	bool result = false;
	if (live)
	{
		if (x + frameWidth - grace > p.getX() &&
			p.getX() + p.getfw() - grace > x &&
			y + frameHeight - grace > p.getY() &&
			p.getY() + p.getfh() - grace > y){
			if (dmgCounter == dmgDelay){
				p.dmgPlayer();
				dmgCounter = 0;
			}
			dmgCounter++;
			speedCounter = 60;
			return true;
		}
	}
	return result;
}

void Wizard::setLive(bool status) {
	live = status;
}

void Wizard::testDraw(int a, int b) {
	al_draw_bitmap_region(image, 0, 0, frameWidth, frameHeight, x - a, y - b, 0);
}

void Wizard::destroy() {
	al_destroy_bitmap(image);
}

void Wizard::resetWizards() {
	live = false;
}