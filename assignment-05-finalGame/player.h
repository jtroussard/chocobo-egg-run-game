#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
using namespace std;

class Player
{
	friend int collided(int x, int y);
	friend bool tileValueHeal(int x, int y);
	friend bool tileValueLava(int x, int y);
	friend bool tileValueNest(int x, int y);
public:
	Player();
	~Player();
	void initPlayer(int width, int height);
	void updatePlayer(int width, int height, int dir); //dir 1 = right, 0 = left
	void drawPlayer(int xoffset, int yoffset, int numEggs, bool egg);
	bool collidePlayer();
	float getX() { return x; }
	float getY() { return y; }
	int getWidth() { return frameWidth; }
	int getHeight() { return frameHeight; }
	bool collisionHealBlock();
	bool collisionLavaBlock();
	bool collisionNestBlock();
	void setX(int incomingX);
	void setY(int incomingY);
	void dmgPlayer();
	int getHealth();
	int getMaxHealth();
	int getScore();
	int getfw();
	int getfh();
	int getSpeed();
	int getOldX();
	int getOldY();
	int getAnimationDirection();
	int getNumEggs();
	void healPlayer(bool nest);
	void setNumEggs(int eggs);
	void incrementScore();
	void addToScore(int points);

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
	int health; // how much health player has
	int score; // running score
	int maxHealth; // max hp for player
	int speed;
	int numEggs;
	int dmgDelay;
	int dmgCount;

	bool egg; // does player have an egg

	ALLEGRO_BITMAP *image;
	ALLEGRO_SAMPLE *hurt;
	ALLEGRO_SAMPLE *heal;
	ALLEGRO_SAMPLE *coin;
};

#endif