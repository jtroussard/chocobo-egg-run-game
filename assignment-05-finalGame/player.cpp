/*
* University of Mary Washington
* Jacques Troussard
* Professor Polack
* CPSC 440
* 07/25/2017
*
* Assignment 5
* player.cpp
*
* Purpose: player class file
*/

#include "player.h"

Player::Player()
{
	image = NULL;
	hurt = NULL;
	heal = NULL;
	coin = NULL;
}

Player::~Player()
{
	al_destroy_bitmap(image);
}

void Player::initPlayer(int width, int height)
{
	score = 0;
	health = 50;
	maxHealth = 50;
	x = 224;
	y = 1312;
	speed = 3;

	maxFrame = 2;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 24;
	frameHeight = 32;
	animationColumns = 2;
	animationDirection = 1;

	dmgDelay = 10;
	dmgCount = 0;

	cout << "Loading Player Image ..." << endl;
	image = al_load_bitmap("assets/sprites/chocoboSpriteSheet.png");
	if (image != NULL) {
		cout << " COMPLETE." << endl;
	}
	else {
		cout << " *** ERROR LOADING PLAYER OBJECT ***" << endl;
	}
	cout << "Loading Player Sounds ..." << endl;
	hurt = al_load_sample("assets/sounds/playerHit.wav");
	if (hurt != NULL) {
		cout << " COMPLETE." << endl;
	}
	else {
		cout << "*** ERROR LOADING PLAYER SOUNDS ***" << endl;
	}
	heal = al_load_sample("assets/sounds/heal.wav");
	if (hurt != NULL) {
		cout << " COMPLETE." << endl;
	}
	else {
		cout << "*** ERROR LOADING PLAYER SOUNDS ***" << endl;
	}
	coin = al_load_sample("assets/sounds/coin.wav");
	if (coin != NULL) {
		cout << " COMPLETE." << endl;
	}
	else {
		cout << "*** ERROR LOADING PLAYER SOUNDS ***" << endl;
	}
}

void Player::updatePlayer(int width, int height, int dir)
{
	//cout << "player: " << x << " " << y << endl;

	if (dir == 1){ //right key
		animationDirection = 1;
		prevPosX = x;
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
		prevPosX = x;
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
		prevPosY = y;
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
		prevPosY = y;
		y += speed;
		if (++frameCount > frameDelay) {
			frameCount = 0;
			if (++curFrame > maxFrame) {
				curFrame = 0;
			}
		}

	}
	else //represent that they hit the space bar and that mean direction = 0
		animationDirection = dir;

	//check for collided with foreground tiles
	if (animationDirection == 0)
	{
		if (collided(x, y + frameHeight)) { //collision detection to the left
			x = prevPosX;
			y = prevPosY;
		}

	}
	else if (animationDirection == 1)
	{
		if (collided(x + frameWidth, y + frameHeight)) { //collision detection to the right
			x = prevPosX;
			y = prevPosY;
		}
	}
	else if (animationDirection == 2) { // check for collision when moving up
		if (collided(x, y + frameHeight)) {
			x = prevPosX;
			y = prevPosY;
		}
	}
	else if (animationDirection == 3) {
		if (collided(x, y + frameHeight)) {
			x = prevPosX;
			y = prevPosY;
		}
	}
}

bool Player::collisionHealBlock()
{
	switch (animationDirection)
	{
	case 0: // detection when facing to the left
		if (tileValueHeal(x + 5, y + frameHeight)) {
			return true;
		}
		else {
			return false;
		}
		break;
	case 1: // detection when facing to the right
		if (tileValueHeal(x + frameWidth - 5, y + frameHeight)) {
			return true;
		}
		else {
			return false;
		}
		break;
	case 2: // detection when facing upwards
		if (tileValueHeal(x + (frameWidth / 2), y + frameHeight + 5)) {
			return true;
		}
		else {
			return false;
		}
		break;
	case 3:
		if (tileValueHeal(x + (frameWidth / 2), y + frameHeight + 5)) {
			return true;
		}
		else {
			return false;
		}
		break;
	default:
		return false;
		break;
	}
}

bool Player::collisionLavaBlock()
{
	switch (animationDirection)
	{
	case 0: // detection when facing to the left
		if (tileValueLava(x + 5, y + frameHeight)) {
			return true;
		}
		else {
			return false;
		}
		break;
	case 1: // detection when facing to the right
		if (tileValueLava(x + frameWidth - 5, y + frameHeight)) {
			return true;
		}
		else {
			return false;
		}
		break;
	case 2: // detection when facing upwards
		if (tileValueLava(x + (frameWidth / 2), y + frameHeight + 5)) {
			return true;
		}
		else {
			return false;
		}
		break;
	case 3:
		if (tileValueLava(x + (frameWidth / 2), y + frameHeight + 5)) {
			return true;
		}
		else {
			return false;
		}
		break;
	default:
		return false;
		break;
	}
	
}

bool Player::collisionNestBlock()
{
	switch (animationDirection)
	{
	case 0: // detection when facing to the left
		if (tileValueNest(x + 5, y + frameHeight)) {
			return true;
		}
		else {
			return false;
		}
		break;
	case 1: // detection when facing to the right
		if (tileValueNest(x + frameWidth - 5, y + frameHeight)) {
			return true;
		}
		else {
			return false;
		}
		break;
	case 2: // detection when facing upwards
		if (tileValueNest(x + (frameWidth / 2), y + frameHeight + 5)) {
			return true;
		}
		else {
			return false;
		}
		break;
	case 3:
		if (tileValueNest(x + (frameWidth / 2), y + frameHeight + 5)) {
			return true;
		}
		else {
			return false;
		}
		break;
	default:
		return false;
		break;
	}
}

void Player::drawPlayer(int xoffset, int yoffset, int numEggs, bool egg)
{
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;
	if (animationDirection < 2) {
		fy = 64; // left to right
	}
	else if (animationDirection == 2) {
		fy = 32; // up
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

	// add egg


	//cout << "inputs = " << fx << " " << fy << " " << frameWidth << " " << frameHeight << " " << x << " " << y << endl;
	//cout << "animation direction = " << animationDirection << "  fx: " << fx << "  fy: " << fy << endl;
	//cout << "player x: " << x << "  y: " << y << endl;
}

void Player::setX(int incomingX) {
	x = incomingX;
}

void Player::setY(int incomingY) {
	y = incomingY;
}

void Player::addToScore(int addPoints) {
	score += addPoints * 10;
	int count = addPoints;
	for (int i = 0; i < count; i++) {
		al_play_sample(coin, 0.25, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	}
}

void Player::dmgPlayer() {
	if (health > 0){
		health--;
		al_play_sample(hurt, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	}
}

int Player::getHealth() {
	return health;
}

int Player::getMaxHealth() {
	return maxHealth;
}

int Player::getScore() {
	return score;
}

int Player::getfw() {
	return frameWidth;
}

int Player::getfh() {
	return frameHeight;
}

int Player::getSpeed() {
	return speed;
}

int Player::getOldX(){
	return prevPosX;
}

int Player::getOldY(){
	return prevPosY;
}

int Player::getAnimationDirection(){
	return animationDirection;
}

int Player::getNumEggs(){
	return numEggs;
}

void Player::healPlayer(bool nest){
	if (health < maxHealth) {
		if (nest){
			health = maxHealth;
		}
		else { 
			health++;
			al_play_sample(heal, .5, 0.0, .50, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
	}

}

void Player::incrementScore(){
	score++;
	al_play_sample(coin, 0.25, 0.0, .50, ALLEGRO_PLAYMODE_ONCE, NULL);
}

void Player::setNumEggs(int eggs) {
	numEggs = eggs;
}