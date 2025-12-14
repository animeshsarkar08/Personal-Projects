//gcc tetris.c -o tetris $(pkg-config --libs --cflags ncurses) -lncursesw
//OR
// gcc tetris.c -o tetris $(pkg-config --libs --cflags ncurses)

#include <ncursesw/ncurses.h>
//#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <string.h>
#include <stdio.h>


// PlayGround Border Dimensions
#define B_X 33
#define B_Y 22

/*typedef enum Block{
	B = 1,L,J,Z,S,O,I,T
}Block;*/
typedef enum Block{
	L = 9,R,D = -1,S = 0
}Block;

void put_Block(int x,int y,Block C);

int main(){
	
	// Window setup
	setlocale(LC_ALL, "");//for the icons
    	WINDOW* win = initscr();
	keypad(win, true);
    	nodelay(win, true);
    	//srand(time(NULL));
    	srand(time(0));
	cbreak(); // Line buffering disabled
    	noecho(); // Don't echo input
	curs_set(0); // Hide the cursor

	// THE acutal playground box
	int box[B_X][B_Y];

	// Initalizing the whole box as 0(Space)
	for(int i = 0; i < B_X; i++){
		for(int j = 0; j < B_Y; j++){
			box[i][j] = 0;
		}
	}

	// Drawing the borders
	for(int y = 1; y < B_Y - 1; y++){
		//top border
		//box[0][y] = 1;
		// bottom border
		box[B_X - 1][y] = -1;
	}
	for(int x = 0; x < B_X; x++){
		//left border
		box[x][0] = 9;
		//right border
		box[x][B_Y - 1] = 10;
	}


	// Shapes and their rotations
	
	// Rotations of L-Shape
	int LR1[4][4] = {
		{0,2,0,0},
		{0,2,0,0},
		{0,2,2,0},
		{0,0,0,0}
	};
	int LR2[4][4] = {
		{0,0,0,0},
		{2,2,2,0},
		{2,0,0,0},
		{0,0,0,0}
	};
	int LR3[4][4] = {
		{2,2,0,0},
		{0,2,0,0},
		{0,2,0,0},
		{0,0,0,0}
	};
	int LR4[4][4] = {
		{0,0,2,0},
		{2,2,2,0},
		{0,0,0,0},
		{0,0,0,0}
	};

	// Rotations of J-Shape
	int JR1[4][4] = {
		{0,3,0,0},
		{0,3,0,0},
		{3,3,0,0},
		{0,0,0,0}
	};
	int JR2[4][4] = {
		{3,0,0,0},
		{3,3,3,0},
		{0,0,0,0},
		{0,0,0,0}
	};
	int JR3[4][4] = {
		{0,3,3,0},
		{0,3,0,0},
		{0,3,0,0},
		{0,0,0,0}
	};
	int JR4[4][4] = {
		{0,0,0,0},
		{3,3,3,0},
		{0,0,3,0},
		{0,0,0,0}
	};


	// Rotations of Z-Shape
	int ZR13[4][4] = {
		{0,0,0,0},
		{4,4,0,0},
		{0,4,4,0},
		{0,0,0,0}
	};

	int ZR24[4][4] = {
		{0,0,4,0},
		{0,4,4,0},
		{0,4,0,0},
		{0,0,0,0}
	};
	
	// Rotations of S-Shape
	int SR13[4][4] = {
		{0,0,0,0},
		{0,5,5,0},
		{5,5,0,0},
		{0,0,0,0}
	};

	int SR24[4][4] = {
		{0,5,0,0},
		{0,5,5,0},
		{0,0,5,0},
		{0,0,0,0}
	};
	
	// Rotations of O-Shape
	int OR1234[4][4] = {
		{0,0,0,0},
		{0,6,6,0},
		{0,6,6,0},
		{0,0,0,0}
	};

	// Rotations of I-Shape
	int IR13[4][4] = {
		{0,7,0,0},
		{0,7,0,0},
		{0,7,0,0},
		{0,7,0,0}
	};

	int IR24[4][4] = {
		{0,0,0,0},
		{7,7,7,7},
		{0,0,0,0},
		{0,0,0,0}
	};

	// Rotations of T-Shape
	int TR1[4][4] = {
		{0,8,0,0},
		{8,8,8,0},
		{0,0,0,0},
		{0,0,0,0}
	};
	int TR2[4][4] = {
		{0,8,0,0},
		{0,8,8,0},
		{0,8,0,0},
		{0,0,0,0}
	};
	int TR3[4][4] = {
		{0,0,0,0},
		{8,8,8,0},
		{0,8,0,0},
		{0,0,0,0}
	};

	int TR4[4][4] = {
		{0,8,0,0},
		{8,8,0,0},
		{0,8,0,0},
		{0,0,0,0}
	};



	//initialization of the current shape
	int start_x = 1,start_y = 10,shape = 2,prev_shape = 2,rotation = 0;

	// these will store the rotation of current shape
	int R1[4][4],R2[4][4],R3[4][4],R4[4][4];
	memcpy(R1,LR1,sizeof(LR1));
	memcpy(R2,LR2,sizeof(LR2));
	memcpy(R3,LR3,sizeof(LR3));
	memcpy(R4,LR4,sizeof(LR4));

	// dim will hold the current rotation of the current shape
	int dim[4][4];

	// the first rotation = 0;
	memcpy(dim,R1,sizeof(R1));
	
	bool go_down,go_left,go_right;


	while(true){
		erase();
		int pressed = wgetch(win);
		if (pressed == KEY_LEFT) {
			//for stablility when any key is pressed
			--start_x;

			// the most left a shape can go is till index 1
			//if(start_y > 1){
				go_left = true;
				for(int i = 0; i < 4; i++){
					for(int j = 0; j < 4; j++){
						if(dim[i][j] > 0 && box[start_x + i][start_y + j - 1] != 0){
							if(j == 0){
								go_left = false;
								break;
							}
							else if (dim[i][j - 1] == 0){
								go_left = false;
								break;
							
							}
						}
					}
				}
				if(go_left) --start_y;
			//}
		
		} else if (pressed == KEY_RIGHT) {
			//for stablility when any key is pressed
			--start_x;

			//if(start_y < B_Y - 1){
				go_right = true;
				for(int i = 0; i < 4; i++){
					for(int j = 0; j < 4; j++){
						if(dim[i][j] > 0 && box[start_x + i][start_y + j + 1] != 0){
							if(j == 3){
								go_right = false;
								break;
							}
							else if (dim[i][j + 1] == 0){
								go_right = false;
								break;
							
							}
						}
					}
				}
				if(go_right) ++start_y;
			//}

		} else if (pressed == KEY_UP) {
			//for stablility when any key is pressed
			--start_x;

			// Making enough space for rotation
			
			// suppose at 12(border) and start_y = 9, then the x-axis(horizontal) length will be
			// start_y + 3(3 blocks from start_y) = 12, this is a collison with the border
			// to avoid this start_y + 3 < B_Y - 1(border)

			// so the MAX(start_y + 3) = B_Y - 2, now if it is greater than B_Y - 2, then decrease start_y
			while(start_y + 3 > B_Y - 2) start_y--;

			// Same for left border
			// to make a rotation the MIN(start_y) = 1
			while(start_y < 1) start_y++;


			//change the current rotation
			rotation++;
			rotation %= 4; // 0,1,2,3
			switch(rotation){
				case 0:
					memcpy(dim,R1,sizeof(R1));
					break;
				case 1:
					memcpy(dim,R2,sizeof(R2));
					break;
				case 2:
					memcpy(dim,R3,sizeof(R3));
					break;
				case 3:
					memcpy(dim,R4,sizeof(R4));
					break;

			}

			
		} else if (pressed == KEY_DOWN) {
			if(start_x < B_X - 1) ++start_x;
		}

		// fixing the position of current block into the box
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				if(dim[i][j] > 0) box[start_x + i][start_y + j] = dim[i][j];
			}
		}


		// Printing the whole box
		for(int i = 0; i < B_X; i++){
			for(int j = 0; j < B_Y; j++){
				put_Block(i,j,box[i][j]);
			}
		}

		refresh();
		
		go_down = true;

		// Check if the block can move down of not 
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				// box[start_x + i + 1][start_y + j] != 0 this condition indicates that there is a piece under the current piece
				// and so we can't go downwards but wait their is a catch.
				// now the piece can be of the block itself this could stop the block from moving downwards
				if(dim[i][j] > 0 && box[start_x + i + 1][start_y + j] != 0){

					// This is the last row so we are sure that the piece under the current block is not the piece of itself
					if(i > 2){
						go_down = false;
						break;
					}
					// if i = 3 then i + 1 will be out of index (ERROR)
					// this shows that the dim value of the block is 0 but the box value is not 0 this indicates that the piece
					// under the current block is of either some other block or its a border
					else if (dim[i + 1][j] == 0){
						go_down = false;
						break;
					}

				}
			}
		}


		// Every block will move downwards if there is no blocks or it's not the border
		if(go_down){

			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					//clean the previous block traces
					if(dim[i][j] > 0) box[start_x + i][start_y + j] = 0;

				}
			}
			++start_x;
			start_x %= (B_X - 1);
			if(start_x == 0) start_x = 1;

		}else{
			// initializing a new block
			start_x = 1;
			start_y = 10;
			rotation = 0;

			// next block is choosen randomly
			while(shape == prev_shape){
				shape = rand() % 9;
				if(shape == 0) shape  = 2 + rand() % 9;
			}
			prev_shape = shape;

			switch(shape){
				case 2: 
					memcpy(R1,LR1,sizeof(LR1));
					memcpy(R2,LR2,sizeof(LR2));
					memcpy(R3,LR3,sizeof(LR3));
					memcpy(R4,LR4,sizeof(LR4));
					break;
			case 3: 
				memcpy(R1,JR1,sizeof(JR1));
				memcpy(R2,JR2,sizeof(JR2));
				memcpy(R3,JR3,sizeof(JR3));
				memcpy(R4,JR4,sizeof(JR4));
				break;
			case 4: 
				memcpy(R1,ZR13,sizeof(ZR13));
				memcpy(R2,ZR24,sizeof(ZR24));
				memcpy(R3,ZR13,sizeof(ZR13));
				memcpy(R4,ZR24,sizeof(ZR24));
				break;
			case 5: 
				memcpy(R1,SR13,sizeof(SR13));
				memcpy(R2,SR24,sizeof(SR24));
				memcpy(R3,SR13,sizeof(SR13));
				memcpy(R4,SR24,sizeof(SR24));
				break;
			case 6: 
				memcpy(R1,OR1234,sizeof(OR1234));
				memcpy(R2,OR1234,sizeof(OR1234));
				memcpy(R3,OR1234,sizeof(OR1234));
				memcpy(R4,OR1234,sizeof(OR1234));
				break;
			case 7: 
				memcpy(R1,IR13,sizeof(IR13));
				memcpy(R2,IR24,sizeof(IR24));
				memcpy(R3,IR13,sizeof(IR13));
				memcpy(R4,IR24,sizeof(IR24));
				break;
			case 8: 
				memcpy(R1,TR1,sizeof(TR1));
					memcpy(R2,TR2,sizeof(TR2));
					memcpy(R3,TR3,sizeof(TR3));
					memcpy(R4,TR4,sizeof(TR4));
					break;

				};

			//initalizing a randome rotation for shape
			rotation  = rand() % 4;
			switch(rotation){
				case 0:
					memcpy(dim,R1,sizeof(R1));
					break;
				case 1:
					memcpy(dim,R2,sizeof(R2));
					break;
				case 2:
					memcpy(dim,R3,sizeof(R3));
					break;
				case 3:
					memcpy(dim,R4,sizeof(R4));
					break;
			}



		}
		
		// checking for a complete line should be at the very end of the loop
		bool got_line = true;
		int jump = 0;
		for(int i = B_X; i > start_x; i--){
			for(int j = 1; j < B_Y - 1;j++){
				if(box[i][j] < 2){
					got_line = false;
					break;
				}
				else got_line = true;
			}
			if(got_line){
				jump++;
				//points++;
			}else{
				if(jump > 0){
					for(int j = 1; j < B_Y - 1;j++){
						box[i + jump][j] = box[i][j];
					}
				}
			}
		}
		usleep(300000);
	}
	endwin();
}

void put_Block(int x,int y,Block C){

	switch(C){

		case L:
			mvaddstr(x,(y * 2) + 19, "<!");
			break;
		case R: 
			mvaddstr(x,(y * 2) + 19, "!>");
			break;
		case D: 
			mvaddstr(x,(y * 2) + 19, "<=>!");
			break;
		case S:
			mvaddstr(x,(y * 2) + 19,".");
			break;
		default: 
			mvaddstr(x,(y * 2) + 19,"[]");
		/*
		case B:
			mvaddstr(x,(y * 2) + 10, "[]");
			break;
		case L:
			//mvaddwstr(x,(y * 2) + 10, L"🏞");
			mvaddstr(x,(y * 2) + 10, "[]");
			break;
		case J:
			//mvaddwstr(x,(y * 2) + 10, L"🌅");
			mvaddstr(x,(y * 2) + 10, "[]");
			break;
		case Z:
			//mvaddwstr(y,(x * 2) + 10, L"🏜");
			//mvaddwstr(y,(x * 2) + 10, L"💙");
			mvaddstr(x,(y * 2) + 10, "[]");
			break;
		case S:
			//mvaddwstr(x,(y * 2) + 10, L"♒");
			mvaddstr(x,(y * 2) + 10, "[]");
			break;
		case O:
			//mvaddwstr(x,(y * 2) + 10, L"✴");
			mvaddstr(x,(y * 2) + 10, "[]");
			break;
		case I:
			//mvaddwstr(x,(y * 2) + 10, L"❇");
			mvaddstr(x,(y * 2) + 10, "[]");
			break;
		case T:
			//mvaddwstr(x,(y * 2) + 10, L"🧧");
			mvaddstr(x,(y * 2) + 10, "[]");
			break;
		default: 
			//mvaddstr(x,(y * 2) + 10,".");
			mvaddstr(x,(y * 2) + 10, "[]");
			*/

	} 
	return;
}
