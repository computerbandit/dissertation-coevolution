#pragma once

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define TEXTURE_SIZE 16
#define ZOOM_FACTOR 4.0f
#define TILE_SIZE (TEXTURE_SIZE * ZOOM_FACTOR)
#define SCALE_FACTOR (TILE_SIZE/TEXTURE_SIZE)

#define SPLASH_STATE_SHOW_TIME 0.5f
#define SPLASH_STATE_BACKGROUND_PATH "Resources/res/Splash_Background.png"


//Spritesheets
#define TILES "Tile_Sheet"
#define TILE_SHEET "Resources/res/Tile_Sheet.png"
#define EXTRA "Extra_Sheet"
#define EXTRA_SHEET "Resources/res/Extra_Sheet.png"
#define PLAYER "Player_Sheet"
#define PLAYER_SHEET "Resources/res/Player_Sheet.png"

//Animation States
#define PLAYER_IDLE 1
#define PLAYER_RUN 2
#define COIN_IDLE 10
#define COIN_COLLECTED 11

//Font
#define MENU_FONT_PATH "Resources/fonts/Menu_Font.ttf"

//textures
#define MAIN_MENU_BACKGROUND_PATH "Resources/res/MainMenu_Background.png"
#define MENU_BUTTON_PATH "Resources/res/Menu_Button.png"

//Player
#define PLAYER_TEX_PATH "Resources/res/Player_Sprite.png"

//Level 
#define LEVEL_PATH "Resources/level/"
#define VALIDATION_LEVEL_PATH LEVEL_PATH "validationlevels/"
#define TRAINING_LEVEL_PATH LEVEL_PATH "trainlevels/"
#define GAME_LEVEL_PATH LEVEL_PATH "gamelevels/"
#define VALIDATION_LEVEL_1 VALIDATION_LEVEL_PATH "lvl-0"
#define VALIDATION_LEVEL_2 VALIDATION_LEVEL_PATH "lvl-1"
#define VALIDATION_LEVEL_3 VALIDATION_LEVEL_PATH "lvl-2"
#define VALIDATION_LEVEL_4 VALIDATION_LEVEL_PATH "lvl-3"
#define VALIDATION_LEVEL_5 VALIDATION_LEVEL_PATH "lvl-4"
#define VALIDATION_LEVEL_6 VALIDATION_LEVEL_PATH "lvl-5"
#define VALIDATION_LEVEL_7 VALIDATION_LEVEL_PATH "lvl-6"
#define VALIDATION_LEVEL_8 VALIDATION_LEVEL_PATH "lvl-7"
#define VALIDATION_LEVEL_9 VALIDATION_LEVEL_PATH "lvl-8"

//Level Times
#define LEVEL_1_TIME 5
#define LEVEL_2_TIME 7
#define LEVEL_3_TIME 10
#define LEVEL_4_TIME 15
#define LEVEL_5_TIME 15
#define LEVEL_6_TIME 15
#define LEVEL_7_TIME 15
#define LEVEL_8_TIME 15