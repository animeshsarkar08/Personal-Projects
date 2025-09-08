#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
void f(int x,int y){
		mvaddstr(y, x, "*");
}
void main() {
    WINDOW* win = initscr();
    keypad(win, true);
    nodelay(win, true);
    int posX = 0;
    int posY = 0;
    int foodX = rand() % 20;
    int foodY = rand() % 20;
    int dirX = 1;
    int dirY = 0;
	int food_count = 2;
    while (true) {
        int pressed = wgetch(win);
        if (pressed == KEY_LEFT) {
            dirX = -1;
            dirY = 0;
        }
        if (pressed == KEY_RIGHT) {
            dirX = 1;
            dirY = 0;
			
        }
		if (pressed == KEY_UP) {
            dirX = 0;
            dirY = -1;
			
        }
        if (pressed == KEY_DOWN) {
            dirX = 0;
            dirY = 1;
			
        }
		int tX= posX;
		int tY = posY;
		posX += dirX;
		posY += dirY;
		erase();
		mvaddstr(tY, tX, "*");
		mvaddstr(posY, posX, "*"); 
	/*    while(tX != posX && tY != posY){
				mvaddstr(tY, tX, "*");
				tX += dirX;
				tY += dirY;
		}*/
		while(true){
			f(tY,tX);
			tX += dirX;
			tY += dirY;
		}

        usleep(100000);
	//	erase();
    }

    endwin();
}
