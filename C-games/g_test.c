#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> // Needed for srand

// Define the dimensions for the game area
#define WIDTH 60
#define HEIGHT 20

// Represents one segment of the snake
typedef struct Pos {
    int x, y;
    struct Pos* next;
} Pos;

// A simple function to create a new snake segment
// We removed the modulus logic from here, as it's better handled in the main loop.
Pos* create(int x, int y) {
    Pos* temp = malloc(sizeof(Pos));
    if (!temp) return NULL;
    temp->x = x;
    temp->y = y;
    temp->next = NULL;
    return temp;
}

int main(void) { // It's standard practice for main to return an int
    WINDOW* win = initscr();
    cbreak();             // Get input character by character
    noecho();             // Don't echo characters to the screen
    curs_set(0);          // Hide the cursor
    keypad(win, true);    // Enable reading of function keys (like arrow keys)
    nodelay(win, true);   // Make wgetch non-blocking

    // Seed the ber generator
    srand(time(NULL));

    // Initial snake direction
    int dirX = 1;
    int dirY = 0;

    // Place the initial food randomly within the boundaries
    // We add 1 to avoid placing it on the border itself
    int foodX = (rand() % (WIDTH - 2)) + 1;
    int foodY = (rand() % (HEIGHT - 2)) + 1;

    // Create the initial snake (head and tail are the same at the start)
    Pos* start = create(WIDTH / 2, HEIGHT / 2);
    Pos* end = start;

    while (true) {
        // --- INPUT HANDLING ---
        int pressed = wgetch(win);
        // Added logic to prevent the snake from reversing on itself
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

        // --- GAME LOGIC ---

        // Calculate the new head's position
        int newX = end->x + dirX;
        int newY = end->y + dirY;

        // *** REVISED BOUNDARY WRAPPING LOGIC ***
        // This is more reliable than using the modulus operator.
        if (newX >= WIDTH - 1) newX = 1;
        if (newX < 1) newX = WIDTH - 2;
        if (newY >= HEIGHT - 1) newY = 1;
        if (newY < 1) newY = HEIGHT - 2;

        // Check if the snake ate the food
        bool ate_food = (newX == foodX && newY == foodY);

        // *** REVISED SNAKE MOVEMENT LOGIC ***
        // This "add head, remove tail" method is more robust.
        Pos* new_head = create(newX, newY);
        end->next = new_head;
        end = new_head;

        if (ate_food) {
            // If food is eaten, the snake grows, so we don't remove the tail.
            // We just spawn new food.
            foodX = (rand() % (WIDTH - 2)) + 1;
            foodY = (rand() % (HEIGHT - 2)) + 1;
        } else {
            // If no food is eaten, we move the snake by removing the tail segment.
            Pos* temp = start;
            start = start->next;
            free(temp);
        }

        // --- DRAWING ---
        erase();

        // Draw the boundary box
        box(win,HEIGHT,WIDTH);

        // Draw the food
        mvaddch(foodY, foodX, ACS_DIAMOND);

        // Draw the snake
        Pos* cur = start;
        while (cur) {
            // Use ACS_CKBOARD for a blocky look
            mvaddch(cur->y, cur->x, ACS_CKBOARD);
            cur = cur->next;
        }

        refresh();

        // Game speed (lower value is faster)
        usleep(100000);
    }

    endwin();
    return 0;
}
