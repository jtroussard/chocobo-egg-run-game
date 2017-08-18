/*
* University of Mary Washington
* Jacques Troussard
* Professor Polack
* CPSC 440
* 07/25/2017
*
* Assignment 5
* main.cpp
*
* Purpose: main program driver
*/

#include <iostream>
#include <math.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\allegro_native_dialog.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include "mappy_A5.h"
#include "player.h"
#include "wizard.h"
#include "egg.h"
using namespace std;

// Variables -- global

// Main function definitions
int collided(int x, int y);
bool tileValueHeal(int x, int y);
bool tileValueLava(int x, int y);
bool tileValueNest(int x, int y);
void displayTime(int xPosPlayer, int yPosPlayer, ALLEGRO_FONT clockFont, int gameTime); // print timer remaining
void displayStatus(Player &p, int timeLeft, ALLEGRO_FONT statusFont, int spacer); // print game and player status
void loadNextMap(int level, Player &p); // load next map & set player position variables
bool loadNextStory(int &level, ALLEGRO_FONT levelFont, ALLEGRO_FONT storyFont, int lineSpace, int xOff, int yOff, int WIDTH, int HEIGHT, int storyFontSize, bool &done, bool &isPaused, ALLEGRO_BITMAP *img); // during pause sequence prompt the user for start and print new story to screen
void resetClock(time_t &startTime, time_t &gameTime, int &differenceInTime, time_t &currentTime, int &timeLeft); // reset timing variables
void wizardGenerator(int level, Wizard wizards[], int sizeArray, int w, int h);
void eggGenerator(Egg eggs[], int size, int w, int h, Player &p);
bool pass(Egg eggs[], int size);

int main(void) {
	cout << "Running main ..." << endl;
	// Variables -- constants
	cout << "Initializing constant variables ...";
	const int WIDTH = 800;
	const int HEIGHT = 600;
	const int FPS = 60;
	const int MAX_WIZARDS = 50;
	const int MAX_EGGS = 5;
	cout << " COMPLETE." << endl;


	// Variables -- standard
	cout << "Initializing standard variables ...";
	int mapW = 1560;
	int mapH = 1690;
	bool done = false;
	bool render = false;
	bool isPaused = true;
	bool gameFinished = false;
	bool won = false;
	bool lavaOn = false;
	bool healOn = false;
	bool musicOff = true;
	bool levelDone = false;

	bool keys[] = { false, false, false, false, false };
	enum KEYS{ UP, DOWN, LEFT, RIGHT, SPACE };

	time_t startTime = time(NULL);
	time_t gameTime = 0;
	time_t currentTime;
	int timebank = 120;
	int timeLeft;
	int differenceInTime;

	int lavaEventTime = 0;
	int lavaDmgCoef = 10; // lower value means strong lava

	int healEventTime = 0;
	int healCoef = 10;

	int lineSpace = 39;
	int storyFontSize = 24;
	int statusFontSize = 30;

	int level = 0;
	int enemies = 0;
	int xOff = 0;
	int yOff = 0;
	int totalEggsSaved = 0;

	int eggCounter = 0;

	Player p1;
	Wizard wizards[MAX_WIZARDS];
	Egg eggs[MAX_EGGS];
	cout << " COMPLETE." << endl;


	// Variables -- allegro
	cout << "Initializing standard variables ...";
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;

	cout << " COMPLETE." << endl;


	// Initalizations -- allegro library
	cout << "Initalizing allegro library ...";
	if (!al_init()) {
		al_show_native_message_box(al_get_current_display(), "Initialization Error!", "Failed to initialize allegro library.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	else {
		cout << " COMPLETE." << endl;
	}


	// Initalizations -- display
	cout << "Initializing display ...";
	display = al_create_display(WIDTH, HEIGHT);
	if (display == NULL) {
		al_show_native_message_box(al_get_current_display(), "Initialization Error!", "Failed to initialize display.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	else{
		cout << " COMPLETE." << endl;
	}


	// Initializations -- graphics
	cout << "Initializing graphics ...";
	al_init_image_addon();
	al_init_primitives_addon();
	ALLEGRO_BITMAP *winner;
	ALLEGRO_BITMAP *loser;
	ALLEGRO_BITMAP *directions;
	winner = al_load_bitmap("assets/cards/winner.png");
	loser = al_load_bitmap("assets/cards/loser.png");
	directions = al_load_bitmap("assets/cards/directions.png");
	cout << " COMPLETE." << endl;


	// Initializations -- audio
	cout << "Initializing audio ...";
	if (!al_install_audio()){
		al_show_native_message_box(al_get_current_display(), "Initialization Error!", "Failed to install audio.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	if (!al_init_acodec_addon()){
		al_show_native_message_box(al_get_current_display(), "Initialization Error!", "Failed to initialize audio.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	if (!al_reserve_samples(8)){
		al_show_native_message_box(al_get_current_display(), "Initialization Error!", "Failed to reserve samples.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	cout << " COMPLETE." << endl;

	cout << "Loading samples ...";
	ALLEGRO_SAMPLE *aud_level_00 = NULL;
	ALLEGRO_SAMPLE_ID id_aud_menu;
	ALLEGRO_SAMPLE *aud_level_01 = NULL;
	ALLEGRO_SAMPLE_ID id_aud_level01;
	ALLEGRO_SAMPLE *aud_level_02 = NULL;
	ALLEGRO_SAMPLE_ID id_aud_level02;
	ALLEGRO_SAMPLE *aud_level_03 = NULL;
	ALLEGRO_SAMPLE_ID id_aud_level03;

	aud_level_00 = al_load_sample("assets/musics/level-02-music.wav"); 
	aud_level_01 = al_load_sample("assets/musics/level-02-music.wav");
	aud_level_02 = al_load_sample("assets/musics/level-02-music.wav");
	aud_level_03 = al_load_sample("assets/musics/victory-00-music.wav");

	if (aud_level_00 == NULL){
		cout << " Error loading main music file." << endl;
		return -9;
	}
	else if (aud_level_01 == NULL) {
		return -99;
	}
	else if (aud_level_02 == NULL) {
		return -999;
	}
	else if (aud_level_03 == NULL) {
		return -9999;
	}
	else {
		cout << " COMPLETE." << endl;
	}

	// Allegro addons -- fonts
	cout << "Loading fonts ...";
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_FONT *clockFont = al_load_ttf_font("assets/fonts/OldLondon.ttf", 30, 0);
	ALLEGRO_FONT *levelFont = al_load_ttf_font("assets/fonts/storyFont.ttf", 75, 0);
	ALLEGRO_FONT *storyFont = al_load_ttf_font("assets/fonts/menuFont.ttf", storyFontSize, 0);
	ALLEGRO_FONT *statusFont = al_load_ttf_font("assets/fonts/statusFont.ttf", statusFontSize, 0);
	ALLEGRO_FONT *menuFont = al_load_ttf_font("assets/fonts/menuFont.ttf", statusFontSize, 0);
	ALLEGRO_FONT *menuFont1 = al_load_ttf_font("assets/fonts/menuFont.ttf", 80, 0);
	cout << " COMPLETE." << endl;


	// Allegro installations -- keyboard
	cout << "Installing keyboard ...";
	if (!al_install_keyboard()) {
		al_show_native_message_box(al_get_current_display(), "Keyboard installation Error!", "Failed to install keyboard.", 0, 0, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	else {
		cout << " COMPLETE." << endl;
	}

	// Finish event queue, register sources, and start timer
	cout << "Starting timer ...";
	timer = al_create_timer(1.0 / FPS);
	al_start_timer(timer);
	cout << " COMPLETE." << endl;

	cout << "Registering event sources ...";
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	cout << " COMPLETE." << endl;


	// Load Map
	cout << "Loading map ...";
	if (MapLoad("assets/maps/level_01.fmp", 1)) {
		return -5;
	}
	else {
		cout << " COMPLETE." << endl;
	}

	// Initalizations -- player
	cout << "Creating player object ...";
	p1.initPlayer(WIDTH, HEIGHT);

	MapInitAnims();
	bool ns = false;
	// Game loop
	cout << "Running game loop..." << endl;
	while (!done) {
		// If game is paused, check which map to load and set player spawn location
		if (isPaused) {
			level++;
			if (loadNextStory(level, *levelFont, *storyFont, lineSpace, xOff, yOff, WIDTH, HEIGHT, storyFontSize, done, isPaused, directions)){
				loadNextMap(level, p1); // load map and set spawn
				eggGenerator(eggs, MAX_EGGS, mapW, mapH, p1);
				resetClock(startTime, gameTime, differenceInTime, currentTime, timeLeft); // reset time
				eggCounter = 0; // reset egg counter
				levelDone = false;
				wizardGenerator(level, wizards, MAX_WIZARDS, mapW, mapH);
			}
		}



		if (musicOff) {
			al_play_sample(aud_level_01, 1.25, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &id_aud_level01);
			musicOff = false;
		}


		// Game clock management
		currentTime = time(&currentTime); // update current time
		differenceInTime = currentTime - startTime; // calculate how much time has passed in game
		if (differenceInTime >= 1) {
			startTime = currentTime;
			gameTime += differenceInTime;
			differenceInTime = 0;
		}

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			render = true;
			MapUpdateAnims();

			if (keys[UP])
				p1.updatePlayer(WIDTH, HEIGHT, 2);
			else if (keys[DOWN])
				p1.updatePlayer(WIDTH, HEIGHT, 3);
			else if (keys[LEFT])
				p1.updatePlayer(WIDTH, HEIGHT, 0);
			else if (keys[RIGHT])
				p1.updatePlayer(WIDTH, HEIGHT, 1);

			if (!isPaused){
				for (int i = 0; i < MAX_WIZARDS; i++)
				{
					wizards[i].updateWizard(p1.getX(), p1.getY(), mapW, mapH);
				}
				for (int i = 0; i < MAX_WIZARDS; i++)
				{
					wizards[i].collideWithPlayer(p1);
				}
			}
			for (int i = 0; i < MAX_EGGS; i++)
			{
				eggs[i].updateEgg(p1);
			}
			for (int i = 0; i < MAX_EGGS; i++)
			{
				eggs[i].collideWithPlayer(p1, eggs, MAX_EGGS);
			}

			// Check for mappy tile collisions
			if (p1.collisionHealBlock() || p1.collisionNestBlock()) {
				// heal player
				if (healEventTime == 0) { // player enters lava -- start counting
					if (!healOn) {
						p1.healPlayer(false);
						healOn = true;
					}
					healEventTime++;
				}
				else if (healEventTime < healCoef) { // start counting frames
					healEventTime++;
				}
				else if (healEventTime == healCoef) {
					bool nest = false;
					if (p1.collisionNestBlock()) {
						nest = true;
					}
					p1.healPlayer(nest);
					healEventTime = 0;

				}
			}
			else { // player leaves lava
				healEventTime = -1; // reset timer
				healOn = false;
			}
			if (p1.collisionLavaBlock()) {
				// damage player
				if (lavaEventTime == 0) { // player enters lava -- start counting
					if (!lavaOn) {
						p1.dmgPlayer();
						//	cout << "inital dmg " << endl;
						lavaOn = true;
					}
					lavaEventTime++;
					//			cout << "health = " << p1.getHealth() << endl;
				}
				else if (lavaEventTime < lavaDmgCoef) { // start counting frames
					lavaEventTime++;
				}
				else if (lavaEventTime == lavaDmgCoef) { // for each half second dmg the player
					p1.dmgPlayer();
					lavaEventTime = 0;
				}
			}
			else { // player leaves lava
				lavaEventTime = -1; // reset timer
				lavaOn = false;
			}
			if (p1.collisionNestBlock()) {
				// check for eggs and deposit
				// check for win condition
				// advance level variable
				// pause game
				if (p1.getNumEggs() > 0) {
					p1.addToScore(p1.getNumEggs());
					p1.setNumEggs(0);
				}

				for (int i = 0; i < MAX_EGGS; i++)
				{
					if (eggs[i].isCollected() && !eggs[i].isSafe()) {
						eggs[i].setSafe();
						eggCounter++;
						totalEggsSaved++;
					}
				}
			}
			render = true;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}
		if (render && al_is_event_queue_empty(event_queue))
		{
			render = false;

			//update the map scroll position
			xOff = p1.getX() + p1.getWidth() - WIDTH / 2;
			yOff = p1.getY() + p1.getHeight() - HEIGHT / 2;

			//avoid moving beyond the map edge
			if (xOff < 0) {
				xOff = 0;
			}

			if (xOff >(mapwidth * mapblockwidth - WIDTH)) {
				xOff = mapwidth * mapblockwidth - WIDTH;
			}

			if (yOff < 0) {
				yOff = 0;
			}

			if (yOff >(mapheight * mapblockheight - HEIGHT)) {
				yOff = mapheight * mapblockheight - HEIGHT;
			}

			timeLeft = timebank - gameTime; // calculate time before print function
			if (timeLeft == 0){ // if time is up set game condition to finished
				gameFinished = true;
			}

			// Drawing suites and Functions
			MapDrawBG(xOff, yOff, 0, 0, WIDTH, HEIGHT);
			MapDrawFG(xOff, yOff, 0, 0, WIDTH, HEIGHT, 0);

			for (int i = 0; i < MAX_WIZARDS; i++)
			{
				wizards[i].drawWizard(xOff, yOff);
			}

			for (int i = 0; i < MAX_EGGS; i++)
			{
				eggs[i].drawEgg(xOff, yOff);
			}

			p1.drawPlayer(xOff, yOff, 0, false);
			displayStatus(p1, timeLeft, *menuFont, lineSpace);
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));

			// Check game condition
			if (timeLeft == 0 || p1.getHealth() == 0) {
				gameFinished = true;
			}


			if (pass(eggs, MAX_EGGS)) {
				al_stop_sample(&id_aud_level01);
				musicOff = true;
				isPaused = true;
				for (int i = 0; i < timeLeft; i++)
				{
					p1.incrementScore();
				}
				for (int i = 0; i < MAX_WIZARDS; i++)
				{
					wizards[i].destroy();
				}
				levelDone = true;
			}

			if (level == 3 && levelDone == true) {
				won = true;
				gameFinished = true;
			}

			if (gameFinished){
				done = true;
			}

			if (won && done) { // if player won draw win screen
				al_draw_bitmap(winner, 0, 0, 0);
				al_draw_textf(menuFont1, al_map_rgb(0, 102, 255), 400, 250, ALLEGRO_ALIGN_CENTRE, "Score: %i", p1.getScore());
				al_draw_textf(menuFont1, al_map_rgb(0, 102, 255), 400, 350, ALLEGRO_ALIGN_CENTRE, "Eggs Saved: %i", totalEggsSaved);

			}
			else if (!won && done) { // if player lost draw lose screen
				al_draw_bitmap(loser, 0, 0, 0);
				al_draw_textf(menuFont1, al_map_rgb(0, 102, 255), 400, 250, ALLEGRO_ALIGN_CENTRE, "Score: %i", p1.getScore());
				al_draw_textf(menuFont1, al_map_rgb(0, 102, 255), 400, 350, ALLEGRO_ALIGN_CENTRE, "Eggs Saved: %i", totalEggsSaved);
			}
		}
	}

	// Flip to show end screen and rest for 10 seconds
	al_flip_display();
	//cout << "resting" << endl;
	al_rest(10);

	// End game and free memory
	MapFreeMem();
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);

	for (int i = 0; i < MAX_WIZARDS; i++)
	{
		wizards[i].destroy();
	}
	
	for (int i = 0; i < MAX_EGGS; i++)
	{
		eggs[i].destroy();
	}


	return 0;
}

// Main function implementations
int collided(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
	return blockdata->tl;
}

void displayTime(int xPosPlayer, int yPosPlayer, ALLEGRO_FONT clockFont, int timeRemaining) {
	al_draw_textf(&clockFont, al_map_rgb(250, 255, 0), xPosPlayer, yPosPlayer, ALLEGRO_ALIGN_CENTRE, "Time:%i", timeRemaining);

}

// Note for screen cast - explain different methods of pin pointing spawn locations. cout/tile counting in mappy...
void loadNextMap(int level, Player &p) {
	switch (level) {
	case 1:
		MapLoad("level_01.fmp", 1);
		break;
	case 2:
		MapLoad("level_01.fmp", 1);
		break;
	case 3:
		MapLoad("level_01.fmp", 1);
		break;
	default:
		break;
	}
}


bool loadNextStory(int &level, ALLEGRO_FONT levelFont, ALLEGRO_FONT storyFont, int lineSpace, int xOff, int yOff, int WIDTH, int HEIGHT, int storyFontSize, bool &done, bool &isPaused, ALLEGRO_BITMAP *img) {
	bool ready = false;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_EVENT ev;
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	while (!ready){
		switch (level)
		{
		case 1:
			al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgb(211,211,211));
			al_draw_filled_rectangle(15, 15, WIDTH-15, HEIGHT-15, al_map_rgb(0,0,233));
			al_draw_textf(&levelFont, al_map_rgb(250, 250, 250), 400, 50, ALLEGRO_ALIGN_CENTRE, "Level: %i", level);
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 25, 80 + (storyFontSize + (lineSpace * 1)), ALLEGRO_ALIGN_LEFT, "Sound the ALARM! Chocobo Forest is under attack!");
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 25, 80 + (storyFontSize + (lineSpace * 2)), ALLEGRO_ALIGN_LEFT, "There is nothing we can do about the evil wizards");
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 25, 80 + (storyFontSize + (lineSpace * 3)), ALLEGRO_ALIGN_LEFT, "until the Red wings of Baron arrive. However you can");
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 25, 80 + (storyFontSize + (lineSpace * 4)), ALLEGRO_ALIGN_LEFT, "still save the flocks' eggs! Gather your family and");
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 25, 80 + (storyFontSize + (lineSpace * 5)), ALLEGRO_ALIGN_LEFT, "return them to your nest!");
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 400, 80 + (storyFontSize + (lineSpace * 6)), ALLEGRO_ALIGN_CENTRE, "(Press any key to begin)");
			al_draw_filled_rectangle(400 - (al_get_bitmap_width(img) / 2) - 10, 380-10, 400 - (al_get_bitmap_width(img) / 2) + 10+500, 380+190, al_map_rgb(211, 211, 211));
			al_draw_bitmap(img, 400-(al_get_bitmap_width(img)/2), 380, 0);
			al_flip_display();
			break;
		case 2:
			xOff = 800;
			al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgb(211, 211, 211));
			al_draw_filled_rectangle(15, 15, WIDTH - 15, HEIGHT - 15, al_map_rgb(0, 0, 233));
			al_draw_textf(&levelFont, al_map_rgb(250, 250, 250), 400, 50, ALLEGRO_ALIGN_CENTRE, "Level: %i", level);
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 25, 80 + (storyFontSize + (lineSpace * 1)), ALLEGRO_ALIGN_LEFT, "Great work you've saved many eggs, but there are");
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 25, 80 + (storyFontSize + (lineSpace * 2)), ALLEGRO_ALIGN_LEFT, "still more to save. If you need help, look for");
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 25, 80 + (storyFontSize + (lineSpace * 3)), ALLEGRO_ALIGN_LEFT, "the dancing peppers!");
			
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 400, 80 + (storyFontSize + (lineSpace * 6)), ALLEGRO_ALIGN_CENTRE, "(Press any key to begin)");
			al_draw_filled_rectangle(400 - (al_get_bitmap_width(img) / 2) - 10, 380 - 10, 400 - (al_get_bitmap_width(img) / 2) + 10 + 500, 380 + 190, al_map_rgb(211, 211, 211));
			al_draw_bitmap(img, 400 - (al_get_bitmap_width(img) / 2), 380, 0);
			al_flip_display();
			break;
		case 3:
			al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT, al_map_rgb(211, 211, 211));
			al_draw_filled_rectangle(15, 15, WIDTH - 15, HEIGHT - 15, al_map_rgb(0, 0, 233));
			al_draw_textf(&levelFont, al_map_rgb(250, 250, 250), 400, 50, ALLEGRO_ALIGN_CENTRE, "Level: %i", level);
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 25, 80 + (storyFontSize + (lineSpace * 1)), ALLEGRO_ALIGN_LEFT, "There are only a few more to save. You can drop");
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 25, 80 + (storyFontSize + (lineSpace * 2)), ALLEGRO_ALIGN_LEFT, "them off one are a time or collect them all!");
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 25, 80 + (storyFontSize + (lineSpace * 3)), ALLEGRO_ALIGN_LEFT, "watch out for the throny bushes and try to keep");
			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 25, 80 + (storyFontSize + (lineSpace * 4)), ALLEGRO_ALIGN_LEFT, "your distance from the red wizards!");

			al_draw_text(&storyFont, al_map_rgb(250, 250, 250), 400, 80 + (storyFontSize + (lineSpace * 6)), ALLEGRO_ALIGN_CENTRE, "(Press any key to begin)");
			al_draw_filled_rectangle(400 - (al_get_bitmap_width(img) / 2) - 10, 380 - 10, 400 - (al_get_bitmap_width(img) / 2) + 10 + 500, 380 + 190, al_map_rgb(211, 211, 211));
			al_draw_bitmap(img, 400 - (al_get_bitmap_width(img) / 2), 380, 0);
			al_flip_display();
			break;
		default:
			break;
		}

		// Wait for player to ready next level
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				ready = true;
				isPaused = false;
				done = true;
				return false;
				break;
			default:
				ready = true;
				isPaused = false;
				return true;
				break;
			}
		}
	}
}

void resetClock(time_t &startTime, time_t &gameTime, int &differenceInTime, time_t &currentTime, int &timeLeft) {
	startTime = time(NULL);
	gameTime = 0;
	differenceInTime = 0;
	currentTime = 0;
	timeLeft = 60;
}

bool tileValueHeal(int x, int y) {
	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	if (data->user1 == 2)
	{
		//cout << "heal" << endl;
		return true;
	}
	else
		return false;
}

bool tileValueLava(int x, int y) {
	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	if (data->user1 == 1)
	{
		//cout << "lava" << endl;
		return true;
	}
	else
		return false;
}

bool tileValueNest(int x, int y) {
	BLKSTR* data;
	data = MapGetBlock(x / mapblockwidth, y / mapblockheight);

	if (data->user1 == 3)
	{
		//cout << "nest" << endl;
		return true;
	}
	else
		return false;
}

// Draws Final Fantasy style menu boxes on the bottom of the screen with additional game information
void displayStatus(Player &p, int timeLeft, ALLEGRO_FONT font, int spacer) {
	// make calls to obtain player health data
	float maxHp = p.getMaxHealth();
	float curHp = p.getHealth();
	float damagePercent = curHp / maxHp;
	int healthBarSize = 275;
	int dmgBarSize = healthBarSize * damagePercent;
	
	// print health, score, power-up???
	al_draw_filled_rounded_rectangle(-20, 480, 656, 590, 10, 10, al_map_rgb(211, 211, 211));
	al_draw_filled_rounded_rectangle(-20, 485, 651, 585, 10, 10, al_map_rgb(0, 0, 230));
	al_draw_textf(&font, al_map_rgb(250, 255, 255), 163, 495, ALLEGRO_ALIGN_CENTRE, "HP  %i / %i", p.getHealth(), p.getMaxHealth());
	al_draw_filled_rounded_rectangle(25, 495 + spacer, 300, 495 + (2 * spacer), 5, 5, al_map_rgb(51, 204, 51));
	if (maxHp > curHp && curHp > 0) {
		al_draw_filled_rounded_rectangle(25 + dmgBarSize, 495 + spacer, 300, 495 + (2 * spacer), 2, 2, al_map_rgb(250, 0, 0));
	}
	else if (curHp <= 0) {
		al_draw_filled_rounded_rectangle(25, 495 + spacer, 300, 495 + (2 * spacer), 5, 5, al_map_rgb(250, 0, 0));
	}

	// print score
	al_draw_text(&font, al_map_rgb(250, 255, 255), 490, 495, ALLEGRO_ALIGN_CENTRE, "XP");
	al_draw_textf(&font, al_map_rgb(250, 255, 255), 490, 495 + spacer, ALLEGRO_ALIGN_CENTRE, "%i", p.getScore());

	// print time
	al_draw_filled_rounded_rectangle(665, 480, 810, 590, 10, 10, al_map_rgb(211, 211, 211));
	al_draw_filled_rounded_rectangle(670, 485, 810, 585, 10, 10, al_map_rgb(0, 0, 230));
	al_draw_text(&font, al_map_rgb(250, 255, 255), 735, 495, ALLEGRO_ALIGN_CENTRE, "Time");
	al_draw_textf(&font, al_map_rgb(250, 255, 255), 735, 495 + spacer, ALLEGRO_ALIGN_CENTRE, "%i", timeLeft);
	al_flip_display();	
}

void wizardGenerator(int level, Wizard wizards[], int sizeArray, int w, int h) {
	int levelCount;
	int blueCount;
	int redCount;
	int x;
	int y;
	bool engagedPosition;

	switch (level)
	{
	case 1:
		levelCount = 15;
		blueCount = 10;
		redCount = levelCount - blueCount;
		for (int i = 0; i < sizeArray; i++)
		{
			Wizard wiz;
			do
			{
				x = rand() % w;
				y = rand() % h;
				BLKSTR *blockdata;
				blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
				engagedPosition = blockdata->tl;
				if (x < 320){
					x += 300;
				}if (y > 1000) {
					y - 500;
				}
			} while (engagedPosition);

			if (blueCount > 0) {
				wiz.initWizard("blue", x, y, level);
				blueCount--;
				wiz.setLive(true);
			}
			else {
				wiz.initWizard("red", x, y, level);
				//cout << x << " red x     red y " << y << endl;
				if (redCount > 0) {
					redCount--;
					wiz.setLive(true);
				}
			}
			wizards[i] = wiz;
		}
		break;
	case 2:
		levelCount = 30;
		blueCount = 15;
		redCount = levelCount - blueCount;
		for (int i = 0; i < sizeArray; i++)
		{
			Wizard wiz;
			do
			{
				x = rand() % w;
				y = rand() % h;
				BLKSTR *blockdata;
				blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
				engagedPosition = blockdata->tl;
				if (x < 500 && x > 50) {
					if (y < 1500 && y > 1150) {
						engagedPosition = true;
					}
				}
				else if ((y < 1500 && y > 1150)) {
					if (x < 320 && x > 64) {
						engagedPosition = true;
					}
				}
			} while (engagedPosition);

			if (blueCount > 0) {
				wiz.initWizard("blue", x, y, level);
				blueCount--;
				wiz.setLive(true);
			}
			else {
				wiz.initWizard("red", x, y, level);
				if (redCount > 0) {
					redCount--;
					wiz.setLive(true);
				}
			}
			wizards[i] = wiz;
		}
		break;
	case 3:
		levelCount = 50;
		blueCount = 15;
		redCount = levelCount - blueCount;
		for (int i = 0; i < sizeArray; i++)
		{
			Wizard wiz;
			do
			{
				x = rand() % w;
				y = rand() % h;
				BLKSTR *blockdata;
				blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
				engagedPosition = blockdata->tl;
				if (x < 320 && x > 64) {
					if (y < 1500 && y > 1150) {
						engagedPosition = true;
					}
				}
				else if ((y < 1500 && y > 1150)) {
					if (x < 320 && x > 64) {
						engagedPosition = true;
					}
				}
			} while (engagedPosition);

			if (blueCount > 0) {
				wiz.initWizard("blue", x, y, level);
				blueCount--;
				wiz.setLive(true);
			}
			else {
				wiz.initWizard("red", x, y, level);
				if (redCount > 0) {
					redCount--;
					wiz.setLive(true);
				}
			}
			wizards[i] = wiz;
		}
		break;
	default:
		break;
	}
}

void eggGenerator(Egg eggs[], int sizeArray, int w, int h, Player &p) {
	int x;
	int y;
	bool engagedPosition;
	
	for (int i = 0; i < sizeArray; i++)
	{
		Egg egg;
		do
		{
			x = rand() % w;
			y = rand() % h;
			BLKSTR *blockdata;
			blockdata = MapGetBlock(x / mapblockwidth, y / mapblockheight);
			engagedPosition = blockdata->tl;
		} while (engagedPosition);
		egg.initEgg(x, y, p.getSpeed());
		eggs[i] = egg;
	}
}

bool pass(Egg eggs[], int size) {
	for (int i = 0; i < size; i++)
	{
		if (!eggs[i].isSafe()) {
			return false;
		}
	}
	return true;
}