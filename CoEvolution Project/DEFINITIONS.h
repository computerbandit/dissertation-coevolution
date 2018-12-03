#pragma once
#include "TILE_DEFINITIONS.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define TEXTURE_SIZE 16
#define TILE_SIZE 64.0f
#define SCALE_FACTOR TILE_SIZE/TEXTURE_SIZE

#define SPLASH_STATE_SHOW_TIME 0.5f
#define SPLASH_STATE_BACKGROUND_PATH "Resources/res/Splash_Background.png"

//trainning speed 
#define DISPLAY_TRAINNING true
#define DEFUALT_TRAINNING_POPULATION_SIZE 50
#define DEFUALT_TRAINNGNG_TIME_TO_LIVE 12.0f
#define DEFUALT_TRAINNING_SPEED_MULTIPLIER 1.0f

//textures
#define MAIN_MENU_BACKGROUND_PATH "Resources/res/MainMenu_Background.png"
#define MAIN_MENU_PLAY_BUTTON_PATH "Resources/res/Play_Button.png"
#define MAIN_MENU_TRAIN_BUTTON_PATH "Resources/res/Train_Button.png"
#define MAIN_MENU_EXIT_BUTTON_PATH "Resources/res/Exit_Button.png"

//Player
#define PLAYER_TEX_PATH "Resources/res/Player_Sprite.png"

//Level 
#define TRAINNING_LEVEL_1 "Resources/level/level1.txt"
#define TRAINNING_LEVEL_2 "Resources/level/level2.txt"

//Spritesheets
#define TILES "Tile_Sheet"
#define TILE_SHEET "Resources/res/Tile_Sheet.png"
#define PLAYER "Player_Sheet"
#define PLAYER_SHEET "Resources/res/Player_Sheet.png"