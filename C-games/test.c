#include <ncursesw/ncurses.h>
//#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>

//#define WIDTH 60
//#define HEIGHT 60

typedef struct Pos{
	int x,y;
	struct Pos* next;
}Pos;
Pos* create(int x,int y){
	Pos* temp = malloc(sizeof(Pos));
	if(!temp) return temp;
	temp->x = x;
	temp->y = y;
	temp->next = NULL;
	return temp;
}

void main() {
	setlocale(LC_ALL, "");
    WINDOW* win = initscr();
    keypad(win, true);
    nodelay(win, true);
    srand(time(NULL));
    int posX = 0;
    int posY = 0;
    int dirX = 1;
    int dirY = 0;
	int height, width;
	getmaxyx(win,height,width);
    int foodX = (rand() % (width - 2)) + 1;
    int foodY = (rand() % (height - 2)) + 1;
	Pos* start = create(0,0); 
	Pos* end = start;
    while (true) {
        int pressed = wgetch(win);
        if (pressed == KEY_LEFT && dirX != 1) {
            dirX = -1;
            dirY = 0;
        } else if (pressed == KEY_RIGHT && dirX != -1) {
            dirX = 1;
            dirY = 0;
        } else if (pressed == KEY_UP && dirY != 1) {
            dirX = 0;
            dirY = -1;
        } else if (pressed == KEY_DOWN && dirY != -1) {
            dirX = 0;
            dirY = 1;
        }
		/*while(cur->next){
			cur->x = cur->next->x;
			cur->y = cur->next->y;
			if(cur->next == NULL) end = cur;
			cur = cur->next;
		}*/
		Pos* temp = start;
		end->next = create(end->x + dirX, end->y + dirY);
		start = start->next;
		end = end->next;
		free(temp);

		erase();

		Pos* cur= start;
        //mvaddstr(foodY, foodX, "&");
        //mvaddch(foodY, foodX, ACS_DIAMOND);
		mvaddwstr(foodY, foodX, L"🍎");

		while(cur){
			//mvaddstr(cur->y,cur->x,"[]");
            //mvaddch(cur->y, cur->x, ACS_CKBOARD);
		    mvaddwstr(cur->y, cur->x, L"{🐍}");
			if(cur->next == NULL) end = cur;
			cur = cur->next;
		}
		
		if (((foodX >= end->x - 1) && (foodX <=end->x + 1)) && ((foodY >= end->y - 1) && (foodY <=end->y + 1))) {
			foodX = (rand() % (width - 2)) + 1;
			foodY = (rand() % (height - 2)) + 1;
			//foodX = rand();
	        //foodY = rand();
			end->next = create(end->x + dirX, end->y + dirY);
			end = end->next;
		}
		refresh();

        if (dirX == 1 || dirX == - 1) usleep(100000);
		else usleep(200000);  

    }

    endwin();
}
