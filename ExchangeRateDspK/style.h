#ifndef STYLE
#define STYLE

extern "C" {

enum{
    LIGHT,
    DARK
};

typedef struct ERDK_STYLE{
    int main_bg_color;
    int plot_bg_color;
    int grid_color;
    int edge_color;
    int font_color;
}ERDK_STYLE;

void SET_STYLE(int);

ERDK_STYLE GET_STYLE();

}

#endif // STYLE

