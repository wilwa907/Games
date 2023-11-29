// Created by William Wärn on 2022-12-14.
// Compile and execute with: "g++ snake.cpp -o play -lncurses" followed by "./play"

#include <iostream>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>
#include <fstream>
using namespace std;

// Variables
bool gameOver;
const int width = 20, height = 20;
int x, y, x_fruit, y_fruit, score = 0, highscore, x_enemy, y_enemy;
char name[20];
enum direction{STOP=0, LEFT, RIGHT, UP, DOWN};
direction dir;

// Tail variables
int x_tail[100], y_tail[100];
int n_tail = 0;

void Setup() 
{   // Reading the current highscore from file
    ifstream infile;
    infile.open("highscore.txt");
    infile >> highscore;
    infile >> name;
    infile.close();
    
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    
    gameOver = FALSE;
    dir = STOP;
    x = width/2; // Start position of snake
    y = height/2;
    n_tail = 0; // Reset of necessary variables
    score = 0;
    
    srand((unsigned) time(NULL)); // Make the fruit and enemy appear random
    x_fruit = rand() % width+1;
    y_fruit = rand() % height+1;
    x_enemy = rand() % width+1;
    y_enemy = rand() % height+1;
}

void Draw()
{
    int dx = rand() % 3 - 1; // Random x velocity (-1, 0, or 1)
    int dy = rand() % 3 - 1; // Random y velocity (-1, 0, or 1)
    
    // Update position
    x_enemy += dx;
    y_enemy += dy;
    
    // Check if position is out of bounds for enemy
    if (x_enemy < 1)
        x_enemy = 2;
    if (x_enemy > width)
        x_enemy = width;
    if (y_enemy < 1)
        y_enemy = 2;
    if (y_enemy > height)
        y_enemy = height;
    
    clear(); // Clear board

    // Creating board, snake and fruit
    for (int i=0; i<height+2; i++)
    {
        for (int j=0; j<width+2; j++)
        {
            if ((i==0 || i==height+1) || (j==0 || j==width+1))
                mvprintw(i, j, "#");
            
            else if (i==y && j==x) {
                attron(COLOR_PAIR(2));
                mvprintw(i, j, "O");
                attroff(COLOR_PAIR(2));
            }
            
            else if (i==y_fruit && j==x_fruit)
                mvprintw(i, j, "@");
            
            else if (i==y_enemy && j==x_enemy) {
                attron(COLOR_PAIR(1));
                mvprintw(i, j, "X");
                attroff(COLOR_PAIR(1));
            }
            
            else {
                for (int k=0; k<n_tail; k++)
                {
                    if (i==y_tail[k] && j==x_tail[k]){
                        attron(COLOR_PAIR(2));
                        mvprintw(i, j, "o");
                        attroff(COLOR_PAIR(2));
                    }
                }
            }
        }
    }
    mvprintw(0, 24, "Score: %d", score);
    mvprintw(2, 24, "Highscore: %d", highscore);
    mvprintw(3, 24, "Username: %s", name);
    refresh();
}

void Input()
{
    keypad(stdscr, TRUE);
    halfdelay(1);
    int c = getch();
    
    switch(c)
    {
        case KEY_LEFT:
            dir = LEFT;
            break;
        case KEY_RIGHT:
            dir = RIGHT;
            break;
        case KEY_UP:
            dir = UP;
            break;
        case KEY_DOWN:
            dir = DOWN;
            break;
        case 113: // Terminate game by pressig q-key (ASCII)
            gameOver = TRUE;
            break;
    }
}

void Logic()
{   // Used for first part of tail to follow head
    int x_prev = x_tail[0];
    int y_prev = y_tail[0];
    x_tail[0] = x;
    y_tail[0] = y;
    
    // Used for rest of tail
    int x2_prev, y2_prev;
    for (int i=1; i<n_tail; i++)
    {
        x2_prev = x_tail[i];
        y2_prev = y_tail[i];
        x_tail[i] = x_prev;
        y_tail[i] = y_prev;
        x_prev = x2_prev;
        y_prev = y2_prev;
    }
    
    switch(dir) // Moving the snake
    {
         case LEFT:
             x--;
             break;
         case RIGHT:
             x++;
             break;
         case UP:
             y--;
             break;
         case DOWN:
             y++;
             break;
         default:
             break;
    }
     
    if (x > width || x < 1 || y < 1 || y > height) // If running into a wall
        gameOver = TRUE;
    
    if (x == x_fruit && y == y_fruit) // If eating fruit
    {
         score++;
         srand((unsigned) time(NULL));
         x_fruit = rand() % width+1; // Create new fruit
         y_fruit = rand() % height+1;
         n_tail++;
    }
    
    if (x == x_enemy && y == y_enemy) // If running into enemy
        gameOver = TRUE;
    
    for (int i=0; i<n_tail; i++) // Check collision between head and tail
    {
        if (x_tail[i]==x && y_tail[i]==y)
            gameOver = TRUE;
    }
}

void Highscore()
{
    if (gameOver==TRUE && score>highscore)
    {
        clear();
        refresh();
        cout << "Your name: ";
        system("stty echo");
        cin >> name;
        system("stty -echo");

       // If getting new highscore, assign it to the storing file highscore.txt
        highscore = score;
        ofstream outfile;
        outfile.open("highscore.txt");
        outfile << highscore << endl;
        outfile << name << endl;
        outfile.close();
    }
}

int main()
{
    int choice;
    do{
        cout << "\u250F\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2513" << endl;
        cout << "\u2503 WELCOME TO SNAKE GAME \u2503" << endl;
        cout << "\u2517\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u251B" << endl;
        cout << "0. Quit" << endl << "1. Play" << endl;
        cin >> choice;

        switch(choice){
            case 0:
                cout << "Bye, see you next time!" << endl;
                break;
            case 1:
                Setup();
                while (!gameOver)
                {
                    Draw();
                    Input();
                    Logic();
                    usleep(100000); // Delay to slow down the movement
                }
                sleep(3);
                Highscore();
                endwin();
                break;
            default:
                cout << "Invalid input, try again!" << endl;
            }
        }
    while(choice!=0);
    return 0;
}

