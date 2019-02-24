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
#define ERROR_TILE 0
#define PLAYER_IDLE 1
#define PLAYER_RUN 2
#define COIN_IDLE 10

//Font
#define MENU_FONT_PATH "Resources/fonts/Menu_Font.ttf"

//textures
#define MAIN_MENU_BACKGROUND_PATH "Resources/res/MainMenu_Background.png"
#define MENU_BUTTON_PATH "Resources/res/Menu_Button.png"

//Player
#define PLAYER_TEX_PATH "Resources/res/Player_Sprite.png"

//Level 
#define LEVEL_PATH "Resources/level/"
#define TRAINING_LEVEL_1 "level1"
#define TRAINING_LEVEL_2 "level2"
#define TRAINING_LEVEL_3 "level3"
#define TRAINING_LEVEL_4 "level4"
#define TRAINING_LEVEL_5 "level5"
#define TRAINING_LEVEL_6 "level6"
#define TRAINING_LEVEL_7 "level7"
#define TRAINING_LEVEL_8 "level8"
#define TRAINING_LEVEL_9 "level9"

//Level Times
#define LEVEL_1_TIME 5
#define LEVEL_2_TIME 7
#define LEVEL_3_TIME 10
#define LEVEL_4_TIME 15
#define LEVEL_5_TIME 15
#define LEVEL_6_TIME 15
#define LEVEL_7_TIME 15
#define LEVEL_8_TIME 15

//trainning defines
#define DISPLAY_TRAINNING true