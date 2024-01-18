#ifndef testAStar_MapGeneral_h
#define testAStar_MapGeneral_h

enum class MapInfoType{
    Invalid   =  0, 
    Block     = -1, 
    Road      =  1, 
    Seat      =  8, 
    Start     = 10, 
    Thing     = 11, 
};

enum class PlayerActionType{
    Invalid    = 0, 
    Down       = 1, 
    Left       = 2, 
    Right      = 3, 
    Up         = 4, 
    Max        = 5, 
};

#define MAPINFO_ASTAR_DIRECTION_NUM 4
#define MAPINFO_MAX_STEP_NUM 10000
#define MAPINFO_MAX_STORE_PATH_SIZE 10000

#endif
