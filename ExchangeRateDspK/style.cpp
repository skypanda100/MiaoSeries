#include "style.h"

static ERDK_STYLE DARK_STYLE = {
    0x302F2F,
    0x000000,
    0x111111,
    0xB1B1b1,
    0xB1B1B1
};

static ERDK_STYLE LIGHT_STYLE = {
    0xF0F0F0,
    0xFFFFFF,
    0xDDDDDD,
    0x000000,
    0x000000
};

static ERDK_STYLE CHOOSEN_STYLE = DARK_STYLE;

void SET_STYLE(int style){
    switch(style){
    case LIGHT:
        CHOOSEN_STYLE = LIGHT_STYLE;
        break;
    case DARK:
        CHOOSEN_STYLE = DARK_STYLE;
        break;
    }
}

ERDK_STYLE GET_STYLE(){
    return CHOOSEN_STYLE;
}
