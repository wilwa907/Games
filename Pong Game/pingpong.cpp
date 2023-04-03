#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <ncurses.h>
#include <time.h>
#include <fstream>
using namespace std;

enum direction{STOP=0, LEFT=1, UPLEFT=2, DOWNLEFT=3, RIGHT=4, UPRIGHT=5, DOWNRIGHT=6};

class cBall // Class for the ball
{
private:
    int x, y, x_original, y_original;
    direction dir;
public:
    cBall(int x_pos, int y_pos)
    {
        // Saves the original position of the ball to spawn
        x_original = x_pos; y_original = y_pos;
        x = x_pos; y = y_pos;
        dir = STOP;
    }
    
    void Reset() // Function that resets the game
    {
        x = x_original; y = y_original;
        dir = STOP;
    }
    
    void ChangeDirection(direction d)
    {
        dir = d;
    }
    
    void RandomDirection() // Creates random direction 1-6
    {
        dir = (direction)((rand() % 6) + 1);
    }
    
    inline int GetX(){return x;}
    inline int GetY(){return y;}
    inline int GetDirection(){return dir;}
    
    void Move()
    {
        switch(dir)
        {
            case STOP:
                break;
            case LEFT:
                x--;
                break;
            case RIGHT:
                x++;
                break;
            case UPLEFT:
                x--; y--;
                break;
            case DOWNLEFT:
                x--; y++;
                break;
            case UPRIGHT:
                x++; y--;
                break;
            case DOWNRIGHT:
                x++; y++;
                break;
            default:
                break;
        }
    }
};

class cPaddle // Class for the Paddles
{
private:
    int x, y, x_original, y_original;
public:
    cPaddle()
    {
        x = y = 0;
    }
    
    cPaddle(int x_pos, int y_pos) : cPaddle()
    {
        x_original = x_pos; y_original = y_pos;
        x = x_pos; y = y_pos;
    }
    
    inline void Reset(){x = x_original; y = y_original;}
    inline int GetX(){return x;}
    inline int GetY(){return y;}
    inline void MoveUp(){y--;}
    inline void MoveDown(){y++;}
};

class cGameManager
{
private:
    int width, height, score1, score2, lives1, lives2, setWins1, setWins2;
    char up1, down1, up2, down2;
    bool quit;
    // Creates pointers
    cBall * ball;
    cPaddle * player1;
    cPaddle * player2;
public:
    cGameManager(int w, int h) // Create constructor
    {
        setlocale(LC_ALL, "");
        initscr();
        clear();
        noecho();
        cbreak();
        curs_set(0);
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        
        srand((unsigned) time(NULL));
        quit = FALSE;
        up1 = 'w'; down1 = 's'; // Keys to control player 1 and 2
        up2 = 'i'; down2 = 'k';
        score1 = score2 = 0;
        lives1 = lives2 = 3;
        setWins1 = setWins2 = 0;
        width = w; height = h;
        ball = new cBall(w/2, h/2);
        player1 = new cPaddle(2, h/2-3);
        player2 = new cPaddle(w-1, h/2-3);
    }
    
    ~cGameManager() // Create destructor
    {
        delete ball;
        delete player1;
        delete player2;
    }
    
    void ScoreUp(cPaddle * player)
    {
        if (player == player1)
            score1++;
        else if (player == player2)
            score2++;
        // Reset the ball and players
        ball -> Reset();
        player1 -> Reset();
        player2 -> Reset();
    }
    
    void LivesDown(cPaddle * player)
    {
        if (player == player1){
            lives1--;
            if (lives1==0)
                setWins2++;
        }
        else if (player == player2){
            lives2--;
            if (lives2==0)
                setWins1++;
        }
        // Reset the ball and players
        ball -> Reset();
        player1 -> Reset();
        player2 -> Reset();
    }
    
    void print_aligned (string s, int width){
        int paddingWidth = width - int(s.length());
        int padLeft = paddingWidth/2;
        int padRight = paddingWidth - padLeft;
        for (int i=0; i<height/2; i++)
            cout << endl;
        cout << right << setw(padLeft + int(s.length())) << setfill(' ') << s << left << setw(padRight) << setfill(' ') << endl;
    }
    
    void createLargeNumber(int i, int j, int number)
    {
        if (number==0){
            mvprintw(i, j, "\u250F\u2501\u2513");
            mvprintw(i+1, j, "\u2503 \u2503");
            mvprintw(i+2, j, "\u2517\u2501\u251B");
        }
        if (number==1){
            mvprintw(i, j, "  \u2503");
            mvprintw(i+1, j, "  \u2503");
            mvprintw(i+2, j, "  \u2503");
        }
        if (number==2){
            mvprintw(i, j, "\u2501\u2501\u2513");
            mvprintw(i+1, j, "\u250F\u2501\u251B");
            mvprintw(i+2, j, "\u2517\u2501\u2501");
        }
        if (number==3){
            mvprintw(i, j, "\u2501\u2501\u2513");
            mvprintw(i+1, j, "\u2501\u2501\u252B");
            mvprintw(i+2, j, "\u2501\u2501\u251B");
        }
        if (number==4){
            mvprintw(i, j, "\u2503 \u2503");
            mvprintw(i+1, j, "\u2517\u2501\u252B");
            mvprintw(i+2, j, "  \u2503");
        }
        if (number==5){
            mvprintw(i, j, "\u250F\u2501\u2501");
            mvprintw(i+1, j, "\u2517\u2501\u2513");
            mvprintw(i+2, j, "\u2501\u2501\u251B");
        }
        if (number==6){
            mvprintw(i, j, "\u250F\u2501\u2501");
            mvprintw(i+1, j, "\u2523\u2501\u2513");
            mvprintw(i+2, j, "\u2517\u2501\u251B");
        }
        if (number==7){
            mvprintw(i, j, "\u2501\u2501\u2513");
            mvprintw(i+1, j, "  \u2503");
            mvprintw(i+2, j, "  \u2503");
        }
        if (number==8){
            mvprintw(i, j, "\u250F\u2501\u2513");
            mvprintw(i+1, j, "\u2523\u2501\u252B");
            mvprintw(i+2, j, "\u2517\u2501\u251B");
        }
        if (number==9){
            mvprintw(i, j, "\u250F\u2501\u2513");
            mvprintw(i+1, j, "\u2517\u2501\u252B");
            mvprintw(i+2, j, "  \u2503");
        }
    }
    
    void Draw()
    {
        clear(); // Clear board
        for (int i=0; i<height+2; i++)
        {
            for (int j=0; j<width+2; j++)
            {
                int ballx = ball-> GetX();
                int bally = ball-> GetY();
                int player1x = player1-> GetX();
                int player2x = player2-> GetX();
                int player1y = player1-> GetY();
                int player2y = player2-> GetY();
                
                if (i==1 && j==9) // Print lives for player 1
                    for (int k=0; k<lives1; k++){
                        attron(COLOR_PAIR(1));
                        mvprintw(i, j+k, "\u2665");
                        attroff(COLOR_PAIR(2));
                    }
                if (i==1 && j==width-10) // Print lives for player 2
                    for (int k=0; k<lives2; k++){
                        attron(COLOR_PAIR(1));
                        mvprintw(i, j+k, "\u2665");
                        attroff(COLOR_PAIR(1));
                    }
                
                if (i==height-2 && j==3 )
                    createLargeNumber(i, j, setWins1);
                if (i==height-2 && j==width-4)
                    createLargeNumber(i, j, setWins2);
                                    
                if (i==0 || i==height+1)
                    mvprintw(i, j, "#");
                
                if (ballx==j && bally==i){
                    attron(COLOR_PAIR(2));
                    mvprintw(i, j, "O");
                    attroff(COLOR_PAIR(2));
                }
                
                else if (player1x==j && player1y==i)
                    mvprintw(i, j, "\u2503");
                
                else if (player2x==j && player2y==i)
                    mvprintw(i, j, "\u2503");
                
                else if (j==0 || j==width+1)
                    mvprintw(i, j, "#");
                else if(j==width/2 && (i!=0 && i!=height+1))
                    mvprintw(i, j, ":");
                // Paddle for player 1
                else if (player1x==j && player1y + 1==i)
                    mvprintw(i, j, "\u2503");
                else if (player1x==j && player1y + 2==i)
                    mvprintw(i, j, "\u2503");
                else if (player1x==j && player1y + 3==i)
                    mvprintw(i, j, "\u2503");
                // Paddle for player 2
                else if (player2x==j && player2y + 1==i)
                    mvprintw(i, j, "\u2503");
                else if (player2x==j && player2y + 2==i)
                    mvprintw(i, j, "\u2503");
                else if (player2x==j && player2y + 3==i)
                    mvprintw(i, j, "\u2503");
            }
        }
        refresh();
    }
    
    void Input()
    {
        ball -> Move();
        
        int ballx = ball-> GetX();
        int bally = ball-> GetY();
        int player1x = player1-> GetX();
        int player2x = player2-> GetX();
        int player1y = player1-> GetY();
        int player2y = player2-> GetY();
        
        keypad(stdscr, TRUE);
        halfdelay(1);
        char current = getch();
        
        if (current == up1)
            if (player1y > 1)
                player1 -> MoveUp();
        if (current == up2)
            if (player2y > 1)
                player2 -> MoveUp();
        if (current == down1)
            if (player1y + 3 < height)
                player1 -> MoveDown();
        if (current == down2)
            if (player2y + 3 < height)
                player2 -> MoveDown();
        
        if (ball -> GetDirection() == STOP)
            ball -> RandomDirection();
        if (current == 'q')
            quit = true;
    }
    
    void Logic()
    {
        int ballx = ball-> GetX();
        int bally = ball-> GetY();
        int player1x = player1-> GetX();
        int player2x = player2-> GetX();
        int player1y = player1-> GetY();
        int player2y = player2-> GetY();
        
        // Left Paddle
        for (int i=0; i<4; i++)
            if (ballx == player1x + 1)
                if (bally == player1y + i)
                    ball -> ChangeDirection((direction)((rand() % 3) + 4));
        
        // Right Paddle
        for (int i=0; i<4; i++)
            if (ballx == player2x - 1)
                if (bally == player2y + i)
                    ball -> ChangeDirection((direction)((rand() % 3) + 1));
        
        // Bottom Wall
        if (bally == height)
            ball -> ChangeDirection(ball -> GetDirection() == DOWNRIGHT ? UPRIGHT : UPLEFT);
            
        // Top Wall
        if (bally == 1)
            ball -> ChangeDirection(ball -> GetDirection() == UPRIGHT ? DOWNRIGHT : DOWNLEFT);
        
        // Right Wall
        if (ballx == width + 1){
            ScoreUp(player1);
            LivesDown(player2);
        }
        
        // Left Wall
        if (ballx == 0){
            ScoreUp(player2);
            LivesDown(player1);
        }
        
        if (lives1==0 || lives2==0)
            lives1 = lives2 = 3;
        
        if (setWins1>9 || setWins2>9){
            clear();
            refresh();
            if (setWins1>=1){
                string winner = "PLAYER 1 WINS!!!";
                print_aligned(winner, width);
            }
            else{
                string winner = "PLAYER 2 WINS!!!";
                print_aligned(winner, width);
            }
            sleep(3);
            quit=TRUE;
        }
    }
    
    void Run()
    {
        while (!quit)
        {
            Draw();
            Input();
            Logic();
        }
        endwin();
    }
};

int main()
{
    int choice;
    do{
        cout << "\u250F\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2513" << endl;
        cout << "\u2503 WELCOME TO PING PONG GAME \u2503" << endl;
        cout << "\u2517\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u2501\u251B" << endl;
        cout << "0. Quit" << endl << "1. Play" << endl << "2. Game Settings" << endl;
        cin >> choice;
        
        switch (choice){
            case 0:{
                endwin();
                exit(0);
                break;}
            case 1:{
                cGameManager c(40, 20);
                c.Run();
                break;}
            case 2:{
                setlocale(LC_ALL, "");
                initscr();
                clear();
                noecho();
                cbreak();
                curs_set(0);
                mvprintw(0, 0, "Player 1: Up = w and Down = s");
                mvprintw(1, 0, "Player 2: Up = i and Down = k");
                mvprintw(4, 0, "Press any key to go back to the menu when done...");
                refresh();
                getch();
                endwin();}
            default:
                cout << "Invalid input, try again!" << endl;
        }
    }
    while(choice!=0);
    return 0;
}

