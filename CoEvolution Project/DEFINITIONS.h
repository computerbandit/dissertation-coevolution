#pragma once
#include "TILE_DEFINITIONS.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define TEXTURE_SIZE 16
#define ZOOM_FACTOR 4.0f
#define TILE_SIZE (TEXTURE_SIZE * ZOOM_FACTOR)
#define SCALE_FACTOR (TILE_SIZE/TEXTURE_SIZE)

#define SPLASH_STATE_SHOW_TIME 0.5f
#define SPLASH_STATE_BACKGROUND_PATH "Resources/res/Splash_Background.png"

//trainning defines

#define DISPLAY_TRAINNING true

//Font
#define MENU_FONT_PATH "Resources/fonts/Menu_Font.ttf"

//textures
#define MAIN_MENU_BACKGROUND_PATH "Resources/res/MainMenu_Background.png"
#define MENU_BUTTON_PATH "Resources/res/Menu_Button.png"

//Player
#define PLAYER_TEX_PATH "Resources/res/Player_Sprite.png"

//Level 
#define TRAINNING_LEVEL_1 "Resources/level/level1.txt"
#define TRAINNING_LEVEL_2 "Resources/level/level2.txt"
#define TRAINNING_LEVEL_3 "Resources/level/level3.txt"
#define TRAINNING_LEVEL_4 "Resources/level/level4.txt"
#define TRAINNING_LEVEL_5 "Resources/level/level5.txt"
#define TRAINNING_LEVEL_6 "Resources/level/level6.txt"
#define TRAINNING_LEVEL_7 "Resources/level/level7.txt"
#define TRAINNING_LEVEL_8 "Resources/level/level8.txt"
#define TRAINNING_LEVEL_9 "Resources/level/level9.txt"

//Level Times
#define LEVEL_1_TIME 5
#define LEVEL_2_TIME 7
#define LEVEL_3_TIME 10
#define LEVEL_4_TIME 15
#define LEVEL_5_TIME 15
#define LEVEL_6_TIME 15
#define LEVEL_7_TIME 15
#define LEVEL_8_TIME 15


//Spritesheets
#define TILES "Tile_Sheet"
#define TILE_SHEET "Resources/res/Tile_Sheet.png"
#define PLAYER "Player_Sheet"
#define PLAYER_SHEET "Resources/res/Player_Sheet.png"