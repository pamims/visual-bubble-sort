#include <curses.h>
#include <stdlib.h>
#include <time.h>


/* - TYPEDEFS - */
// Used to easily identify color pairs in this program
typedef enum { red = 1, green = 2, red_white = 3, cyan = 4 } text_color;


/* - FUNCTION PROTOTYPES - */
// Prepares the terminal window as desired for this program
void terminal_setup(int, int);
// Draws a bar to the screen
void draw_bar(int);
// Draw the list as bars
void draw_list_bars(int *, size_t, int, int);
// Swaps two integer values
void swap(int *, int *);
// Shuffle an array of ints
void shuffle(int *, size_t);
// Print a message in cyan
void print_cyan_message(const char *);


/* - CODE ENTRY POINT - */
int main(int argc, char *argv[]) {
    // const values
    const char title[] =
        "  ____        _     _     _         _____            _   \n"
        " |  _ \\      | |   | |   | |       / ____|          | |  \n"
        " | |_) |_   _| |__ | |__ | | ___  | (___   ___  _ __| |_ \n"
        " |  _ <| | | | '_ \\| '_ \\| |/ _ \\  \\___ \\ / _ \\| '__| __|\n"
        " | |_) | |_| | |_) | |_) | |  __/  ____) | (_) | |  | |_ \n"
        " |____/ \\__,_|_.__/|_.__/|_|\\___| |_____/ \\___/|_|   \\__|\n\n"
        "             Press 'Q' to end the program.\n"
        "    Press any other key to advance the sort algorithm.\n";
    const char end_message[]  = "                       Thank you!";
    const int width = 58;   // Width of terminal window
    const int height = 50;  // Height of terminal window
    const size_t size = 50; // Size of the list array

    // variables used in the sort algorithm
    int ceiling = size;   // The index value at which the index is reset to 0
    int new_ceiling = 0;  // The value that ceiling will update to each pass
    int index = 0;         // The current position in the list
    int list[size];         // The list of ints to be sorted
    for (int i = 0; i < size; i++) {
        list[i] = i + 1;
    }

    // Initialize the terminal and its settings
    initscr();
    terminal_setup(height, width);
    // Display title screen
    print_cyan_message(title);
    napms(4000);
    // Shuffle the list
    shuffle(list, size);

    // Bubble sort and draw each individual step
    while (ceiling) {
        draw_list_bars(list, size, index, ceiling);
        if (getch() == 'q') break;
        if (list[index] > list[index + 1]) {
            swap(&list[index], &list[index + 1]);
            new_ceiling = index + 1;
        }
        index++;
        if (index >= ceiling - 1) {
            ceiling = new_ceiling;
            new_ceiling = index = 0;
        }
    }
    draw_list_bars(list, size, index, ceiling);
    napms(500);

    // Print exit message
    print_cyan_message(end_message);
    napms(2000);
    endwin();
    return 0;
}


/* - FUNCTION DEFINITIONS - */
void terminal_setup(int rows, int columns) {
    resize_term(rows, columns);
    noecho();
    start_color();
    init_pair(red, COLOR_RED, COLOR_BLACK);
    init_pair(green, COLOR_GREEN, COLOR_BLACK);
    init_pair(red_white, COLOR_RED, COLOR_WHITE);
    init_pair(cyan, COLOR_CYAN, COLOR_BLACK);
    return;
}


void draw_bar(int length) {
    for (int i = 0; i < length; i++) printw("#");
    printw("\n");
    return;
}


void draw_list_bars(int *list, size_t size, int index, int ceiling) {
    clear();
    if (ceiling) {
        // Draw red unsorted before index
        attron(COLOR_PAIR(red));
        for (int i = 0; i < index; i++) draw_bar(list[i]);
        // Draw index highlighted in white
        attron(COLOR_PAIR(red_white));
        draw_bar(list[index]);
        // Draw any unsorted after the index
        attron(COLOR_PAIR(red));
        for (int i = index + 1; i < ceiling; i++) draw_bar(list[i]);
    }
    // Draw green sorted
    attron(COLOR_PAIR(green));
    for (int i = ceiling; i < size; i++) draw_bar(list[i]);
    attroff(COLOR_PAIR(green));
    refresh();
    return;
}


void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    return;
}


void shuffle(int *list, size_t size) {
    srand(time(NULL));
    for (int i = 0; i < size; i++)
        swap(&list[i], &list[rand() % (int)size]);
    return;
}


void print_cyan_message(const char *message) {
    clear();
    attron(COLOR_PAIR(cyan));
    printw("\n\n\n\n\n%s",message);
    attroff(COLOR_PAIR(cyan));
    refresh();
    return;
}
