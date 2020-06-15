//
// Created by seb on 27/04/2020.
//

#ifndef MASTER_NEATER_HELPER_HPP
#define MASTER_NEATER_HELPER_HPP

#include <math.h>

#define ERROR(x) (std::string("\033[31m") + x + "\033[39m")
#define WARNING(x) (std::string("\033[33m") + x + "\033[39m")
#define INFO(x) (std::string("\033[32m") + x + "\033[39m")

#define ACTIVATION(x) ((1 / (1 + exp(-2 * x))) * 2 - 1)

#define THRESHOLD 2.8f

#define BIAS 0
#define WEIGHT 2
#define WEIGHT_TUNE 1
#define ELITE 1


#define MUT_NODE 1
#define MUT_CONNECTION 1
#define MUT_CONNECTION_STATUS 0
#define MUT_WEIGHT 1
#define MUT_TOTAL_WEIGHT 0

#define MUT_NODE_VALUE 0.5
#define MUT_CONNECTION_VALUE 0.2
#define MUT_CONNECTION_STATUS_VALUE 0.01
#define MUT_WEIGHT_VALUE 0.3
#define MUT_TOTAL_WEIGHT_VALUE 0.05

#define GRID_SIZE 25

#define APPLE_REWARD 500
#define LIVING_REWARD 1
#define FOOD_REWARD 50
#define FOOD_BASE 125

#define SPECIE_SIZE 200
#define NBTHREAD 4

#define CtoI(x) ((float)(x >= '0' && x <= '9'? x - 48 : x - 55))

#define FIFO_IN "Arcade-Learning-Environment/ale_fifo_out"
#define FIFO_OUT "Arcade-Learning-Environment/ale_fifo_in"

#define ALE "./Arcade-Learning-Environment/ale"
#define MAX_FRAME "10000"
//char * const ALE_NonGraphic [] = {ALE, "-game_controller", "fifo", "-max_num_frames", MAX_FRAME, "Arcade-Learning-Environment/roms/space_invaders.bin"};
//char * const ALE_Graphic[] = {ALE, "-game_controller", "fifo", "-max_num_frames", MAX_FRAME, "-display_screen", "true", "Arcade-Learning-Environment/roms/space_invaders.bin"};
//char * const ALE_GraphicSound[] = {ALE, "-game_controller", "fifo", "-max_num_frames", MAX_FRAME, "-display_screen", "true", "-sound", "true", "Arcade-Learning-Environment/roms/space_invaders.bin"};

//enum class Action {
//    PLAYER_A_NOOP           = 0,
//    PLAYER_A_FIRE           = 1,
//    PLAYER_A_UP             = 2,
//    PLAYER_A_RIGHT          = 3,
//    PLAYER_A_LEFT           = 4,
//    PLAYER_A_DOWN           = 5,
//    PLAYER_A_UPRIGHT        = 6,
//    PLAYER_A_UPLEFT         = 7,
//    PLAYER_A_DOWNRIGHT      = 8,
//    PLAYER_A_DOWNLEFT       = 9,
//    PLAYER_A_UPFIRE         = 10,
//    PLAYER_A_RIGHTFIRE      = 11,
//    PLAYER_A_LEFTFIRE       = 12,
//    PLAYER_A_DOWNFIRE       = 13,
//    PLAYER_A_UPRIGHTFIRE    = 14,
//    PLAYER_A_UPLEFTFIRE     = 15,
//    PLAYER_A_DOWNRIGHTFIRE  = 16,
//    PLAYER_A_DOWNLEFTFIRE   = 17,
//    PLAYER_B_NOOP           = 18,
//    PLAYER_B_FIRE           = 19,
//    PLAYER_B_UP             = 20,
//    PLAYER_B_RIGHT          = 21,
//    PLAYER_B_LEFT           = 22,
//    PLAYER_B_DOWN           = 23,
//    PLAYER_B_UPRIGHT        = 24,
//    PLAYER_B_UPLEFT         = 25,
//    PLAYER_B_DOWNRIGHT      = 26,
//    PLAYER_B_DOWNLEFT       = 27,
//    PLAYER_B_UPFIRE         = 28,
//    PLAYER_B_RIGHTFIRE      = 29,
//    PLAYER_B_LEFTFIRE       = 30,
//    PLAYER_B_DOWNFIRE       = 31,
//    PLAYER_B_UPRIGHTFIRE    = 32,
//    PLAYER_B_UPLEFTFIRE     = 33,
//    PLAYER_B_DOWNRIGHTFIRE  = 34,
//    PLAYER_B_DOWNLEFTFIRE   = 35,
//    RESET                   = 40, // MGB: Use SYSTEM_RESET to reset the environment.
//    UNDEFINED               = 41,
//    RANDOM                  = 42,
//    SAVE_STATE              = 43,
//    LOAD_STATE              = 44,
//    SYSTEM_RESET            = 45,
//    LAST_ACTION_INDEX       = 50
//};

#endif //MASTER_NEATER_HELPER_HPP
