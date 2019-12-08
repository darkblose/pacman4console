/****************************************************
* Pacman For Console V1.2                           *
* By: Rev. Dr. Mike Billars (doctormike@gmail.com)  *
* Date: 2006-12-12                                  *
*                                                   *
* Please see file COPYING for details on licensing  *
*       and redistribution of this program          *
*                                                   *
****************************************************/

#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include "pacman.h"
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>

#define EXIT_MSG "Good bye!"
#define END_MSG "Game Over"
#define QUIT_MSG "Bye"
#define LEVEL_ERR "Cannot find level file: "

void IntroScreen();                                 //Show introduction screen and menu
void CheckCollision();                              //See if Pacman and Ghosts collided
void CheckScreenSize();                             //Make sure resolution is at least 32x29
void CreateWindows(int y, int x, int y0, int x0);   //Make ncurses windows
void Delay();                                       //Slow down game for better control
void DrawWindow();                                  //Refresh display 
void ExitProgram(const char *message);              //Exit and display something
void GetInput();                                    //Get user input
void InitCurses();                                  //Start up ncurses
void LoadLevel(char *levelfile);                    //Load level into memory
void MainLoop();                                    //Main program function
void MoveGhosts();                                  //Update Ghosts' location
void MovePacman();                                  //Update Pacman's location
void PauseGame();                                   //Pause

//Player 2
void CheckCollision2();
void Delay2();
void DrawWindow2();
void GetInput2();
void LoadLevel2(char *levelfile);
void MainLoop2();
void MovePacman2();

int display_menu(void);  //Display Menu

//For ncurses
WINDOW * win;
WINDOW * status;

//For colors
enum { Wall = 1, Normal, Pellet, PowerUp, GhostWall, Ghost1, Ghost2, Ghost3, Ghost4, Ghost5, Ghost6, Ghost7, Ghost8, BlueGhost, Pacman, Pacman2 };

//I know global variables are bad, but it's just sooo easy!
int Loc[10][2] = { 0 };					//Location of Ghosts and Pacman
int Dir[10][2] = { 0 };					//Direction of Ghosts and Pacman
int StartingPoints[10][2] = { 0 };			//Default location in case Pacman/Ghosts die
int Invincible = 0;					//Check for invincibility
int Invincible2 = 0;
int Food = 0;						//Number of pellets left in level
int Level[39][38] = { 0 };				//Main level array
int LevelNumber = 0;					//What level number are we on?
int GhostsInARow = 0;					//Keep track of how many points to give for eating ghosts
int tleft = 0;						//How long left for invincibility
int menu = 1;

int main(int argc, char *argv[]) 
{

	while(menu)
	{
		menu = display_menu();
   
		if(menu == 1)
		{
			int j = 0;
			srand( (unsigned)time( NULL ) );
	
			InitCurses();
			CheckScreenSize();
			CreateWindows(39, 38, 1, 1);

			//If they specified a level to load
			if((argc > 1) && (strlen(argv[1]) > 1)) 
			{
        IntroScreen();
				LoadLevel(argv[1]);
				MainLoop();
			}
        
			//If not, display intro screen then use default levels
			else 
			{
				//Show intro "movie"
				IntroScreen();

				j = 1;
				//They want to start at a level 1-9
				if(argc > 1)
        {
          for(LevelNumber = '1'; LevelNumber <= '9'; LevelNumber++)
          {
            if(LevelNumber == argv[1][0]) j = LevelNumber - '0';			
          }								  
					//Load 9 levels, 1 by 1, if you can beat all 9 levels in a row, you're awesome
					for(LevelNumber = j; LevelNumber < 10; LevelNumber++) 
					{
						LevelFile[strlen(LevelFile) - 6] = '0';
						LevelFile[strlen(LevelFile) - 5] = LevelNumber + '0';
						LoadLevel(LevelFile);
						Invincible = 0;			//Reset invincibility
					}
        }
			}
		}
   
		else if (menu == 2)
		{
			int j = 0;
			srand( (unsigned)time( NULL ) );
	
			InitCurses();
			CheckScreenSize();
			CreateWindows(39, 38, 1, 1);

			//If they specified a level to load
			if((argc > 1) && (strlen(argv[1]) > 1)) 
			{
        IntroScreen();
				LoadLevel2(argv[1]);
				MainLoop2();
			}
        
			//If not, display intro screen then use default levels
			else 
			{
				//Show intro "movie"
				IntroScreen();

				j = 1;
				//They want to start at a level 1-9
				if(argc > 1)
        {
					for(LevelNumber = '1'; LevelNumber <= '9'; LevelNumber++)
          {
						if(LevelNumber == argv[1][0]) j = LevelNumber - '0';	
          }										  
					//Load 9 levels, 1 by 1, if you can beat all 9 levels in a row, you're awesome
					for(LevelNumber = j; LevelNumber < 10; LevelNumber++) 
					{
						LevelFile[strlen(LevelFile) - 6] = '0';
						LevelFile[strlen(LevelFile) - 5] = LevelNumber + '0';
						LoadLevel2(LevelFile);
						Invincible2 = 0;			//Reset invincibility
					}
        }
			}
		}
   
		else if (menu == 3)
		{
			exit(0);
		}
	}
	ExitProgram(EXIT_MSG);	  
}

int display_menu(void) //Display Menu
{
	int menu = 0;

	while(1)
	{
		system("clear");
		printf("\n\n\t\t\t      OpenSource Team Project \n");
		printf("\n\t\t\t\t  P A C M A N \n");
		printf("\n\t\t\t\t B589036 SHIM JAEWOO \t\n");
		printf("\n\t\t\t\t B589051 LEE DONGHUN \t\n ");
		printf("\n\t\t\t\t 1) Solo ");
		printf("\n\t\t\t\t 2) Duo ");
    printf("\n\t\t\t\t 3) Exit \n\n\n");
		printf("\n\n\t\t\t\t  HOW TO PLAY PACMAN ");
    printf("\n\n\t\t\t\t  HOW TO PLAY PACMAN ");
    printf("\n\n\t\t\t\t  HOW TO PLAY PACMAN ");
    printf("\n\n\t\t\t\t  HOW TO PLAY PACMAN ");
    printf("\n\n\t\t\t\t  HOW TO PLAY PACMAN \n");
   
   
		scanf("%d",&menu);
   
		if(menu < 1 || menu > 3)
		{
			continue;
		}
		else
		{
			return menu;
		}
	}
	return 0;
}

void CheckCollision() 
{
	int a = 0;
 
	for(a = 0; a < 8; a++) 
  {
		//Collision
		if((Loc[a][0] == Loc[8][0]) && (Loc[a][1] == Loc[8][1])) 
    {

			//Ghost dies
			if(Invincible == 1) 
      {
				Points = Points + GhostsInARow * 20;
				mvwprintw(win, Loc[8][0], Loc[8][1] - 1, "%d", (GhostsInARow * 20));
				GhostsInARow *= 2;
				wrefresh(win);

				usleep(1000000);

				Loc[a][0] = StartingPoints[a][0]; Loc[a][1] = StartingPoints[a][1];
			}

			//Pacman dies
			else 
      {
				wattron(win, COLOR_PAIR(Pacman));
				mvwprintw(win, Loc[8][0], Loc[8][1], "X");

				wrefresh(win);

				Lives--;
				usleep(1000000);

				if(Lives == -1) ExitProgram(END_MSG);

				//Reset level
				for(a = 0; a < 9; a++) 
        {
					Loc[a][0] = StartingPoints[a][0];
					Loc[a][1] = StartingPoints[a][1];
				}
				Dir[0][0] =  1; Dir[0][1] =  0;
				Dir[1][0] = -1; Dir[1][1] =  0;
				Dir[2][0] =  0; Dir[2][1] = -1;
				Dir[3][0] =  0; Dir[3][1] =  1;
				Dir[4][0] =  1; Dir[4][1] =  0;
				Dir[5][0] = -1; Dir[5][1] =  0;
				Dir[6][0] =  0; Dir[6][1] = -1;
				Dir[7][0] =  0; Dir[7][1] =  1;
				Dir[8][0] =  1; Dir[8][1] =  0;

				DrawWindow();

				usleep(1000000);
			}
		}
	}
}

void CheckScreenSize() 
{
        int h, w; getmaxyx(stdscr, h, w);
        
        if((h < 32) || (w < 29)) 
        {
                endwin();
                fprintf(stderr, "\nSorry.\n");
                fprintf(stderr, "To play Pacman for Console, your console window must be at least 32x29\n");
                fprintf(stderr, "Please resize your window/resolution and re-run the game.\n\n");
                exit(0);
        }
}

void CreateWindows(int y, int x, int y0, int x0) 
{
	win = newwin(y, x, y0, x0);
	status = newwin(3, 37, 39, 1);
}

void Delay() 
{

	struct timeb t_start, t_current;
	ftime(&t_start);

	//Slow down the game a little bit
	do 
  {
		GetInput();
		ftime(&t_current);
	} 
  while (abs(t_start.millitm - t_current.millitm) < SpeedOfGame);
}

void DrawWindow() 
{
	int a = 0; int b = 0;
	char chr = ' ';
	int attr;

	//Display level array
	for(a = 0; a < 39; a++) for(b = 0; b < 38; b++) 
  {
		switch(Level[a][b]) 
    {
		case 0: chr = ' '; attr = A_NORMAL; wattron(win, COLOR_PAIR(Normal));    break;
		case 1: chr = ' '; attr = A_NORMAL; wattron(win, COLOR_PAIR(Wall));      break;
		case 2: chr = '.'; attr = A_NORMAL; wattron(win, COLOR_PAIR(Pellet));    break;
		case 3: chr = '*'; attr = A_BOLD;   wattron(win, COLOR_PAIR(PowerUp));   break;
		case 4: chr = ' '; attr = A_NORMAL; wattron(win, COLOR_PAIR(GhostWall)); break;
		}
		mvwaddch(win, a, b, chr | attr);
	}

	//Display number of lives, score, and level
	attr = A_NORMAL;
	wmove(status, 1, 1);
	wattron(status, COLOR_PAIR(Pacman));
	for(a = 0; a < Lives; a++)
  {
    wprintw(status, "C ");
  }
	wprintw(status, "  ");
	wattron(status, COLOR_PAIR(Normal));
	mvwprintw(status, 2, 2, "Level: %d     Score: %d ", LevelNumber, Points);
	wrefresh(status);

	//Display ghosts
	if(Invincible == 0) 
  {
		wattron(win, COLOR_PAIR(Ghost1)); mvwaddch(win, Loc[0][0], Loc[0][1], '&');
		wattron(win, COLOR_PAIR(Ghost2)); mvwaddch(win, Loc[1][0], Loc[1][1], '&');
		wattron(win, COLOR_PAIR(Ghost3)); mvwaddch(win, Loc[2][0], Loc[2][1], '&');
		wattron(win, COLOR_PAIR(Ghost4)); mvwaddch(win, Loc[3][0], Loc[3][1], '&');
		wattron(win, COLOR_PAIR(Ghost5)); mvwaddch(win, Loc[4][0], Loc[4][1], '&');
		wattron(win, COLOR_PAIR(Ghost6)); mvwaddch(win, Loc[5][0], Loc[5][1], '&');
		wattron(win, COLOR_PAIR(Ghost7)); mvwaddch(win, Loc[6][0], Loc[6][1], '&');
		wattron(win, COLOR_PAIR(Ghost8)); mvwaddch(win, Loc[7][0], Loc[7][1], '&');
	}

	//OR display vulnerable ghosts
	else 
  {
		wattron(win, COLOR_PAIR(BlueGhost));
		mvwaddch(win, Loc[0][0], Loc[0][1], tleft + '0');
		mvwaddch(win, Loc[1][0], Loc[1][1], tleft + '0');
		mvwaddch(win, Loc[2][0], Loc[2][1], tleft + '0');
		mvwaddch(win, Loc[3][0], Loc[3][1], tleft + '0');
		mvwaddch(win, Loc[4][0], Loc[4][1], tleft + '0');
		mvwaddch(win, Loc[5][0], Loc[5][1], tleft + '0');
		mvwaddch(win, Loc[6][0], Loc[6][1], tleft + '0');
		mvwaddch(win, Loc[7][0], Loc[7][1], tleft + '0');
	}

	//Display Pacman
	wattron(win, COLOR_PAIR(Pacman)); 
  mvwaddch(win, Loc[8][0], Loc[8][1], 'C');

	wrefresh(win);
}

void ExitProgram(const char *message) 
{
	endwin();
	printf("%s\n", message);
	exit(0);
}

void GetInput() 
{
	int ch;
	static int chtmp;

	ch = getch();

	//Buffer input
	if(ch == ERR) ch = chtmp;
	chtmp = ch;

	switch (ch) 
  {
		case KEY_UP:    case 'w': case 'W':
			if((Level[(Loc[8][0]-1) % 39][Loc[8][1]] != 1)
			&& (Level[(Loc[8][0]-1) % 39][Loc[8][1]] != 8))
				{ Dir[8][0] = -1; Dir[8][1] =  0; }
			break;

		case KEY_DOWN:  case 's': case 'S':
			if((Level[(Loc[8][0]+1) % 39][Loc[8][1]] != 1)
			&& (Level[(Loc[8][0]+1) % 39][Loc[8][1]] != 8))
				{ Dir[8][0] =  1; Dir[8][1] =  0; }
			break;

		case KEY_LEFT:  case 'a': case 'A':
			if((Level[Loc[8][0]][(Loc[8][1]-1) % 38] != 1)
			&& (Level[Loc[8][0]][(Loc[8][1]-1) % 38] != 8))
				{ Dir[8][0] =  0; Dir[8][1] = -1; }
			break;

		case KEY_RIGHT: case 'd': case 'D':
			if((Level[Loc[8][0]][(Loc[8][1]+1) % 38] != 1)
			&& (Level[Loc[8][0]][(Loc[8][1]+1) % 38] != 8))
				{ Dir[8][0] =  0; Dir[8][1] =  1; }
			break;

		case 'p': case 'P':
			PauseGame();
			chtmp = getch();
			break;

		case 'q': case 'Q':
			ExitProgram(QUIT_MSG);
			break;

	}
}

void InitCurses() 
{
	initscr();
	start_color();
	curs_set(0);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	nonl();
	cbreak();
	noecho();

	init_pair(Normal,    COLOR_WHITE,   COLOR_BLACK);
	init_pair(Wall,      COLOR_WHITE,   COLOR_WHITE);
	init_pair(Pellet,    COLOR_WHITE,   COLOR_BLACK);
	init_pair(PowerUp,   COLOR_BLUE,    COLOR_BLACK);
	init_pair(GhostWall, COLOR_WHITE,   COLOR_CYAN);
	init_pair(Ghost1,    COLOR_RED,     COLOR_BLACK);
	init_pair(Ghost2,    COLOR_CYAN,    COLOR_BLACK);
	init_pair(Ghost3,    COLOR_MAGENTA, COLOR_BLACK);
	init_pair(Ghost4,    COLOR_YELLOW,  COLOR_BLACK);
 	init_pair(Ghost5,    COLOR_RED,     COLOR_BLACK);
	init_pair(Ghost6,    COLOR_CYAN,    COLOR_BLACK);
	init_pair(Ghost7,    COLOR_MAGENTA, COLOR_BLACK);
	init_pair(Ghost8,    COLOR_YELLOW,  COLOR_BLACK);
	init_pair(BlueGhost, COLOR_BLUE,    COLOR_RED);
	init_pair(Pacman,    COLOR_YELLOW,  COLOR_BLACK);
  init_pair(Pacman2,    COLOR_YELLOW,  COLOR_BLACK);
}

void IntroScreen() 
{
	int a = 0;
	int b = 23;

	a=getch();
	a=getch();
	a=getch();

	mvwprintw(win, 20, 8, "Press any key...");

	//Scroll Pacman to middle of screen
	for(a = 0; a < 13; a++) 
  {
		if(getch()!=ERR) return;
		wattron(win, COLOR_PAIR(Pacman));
		mvwprintw(win, 8, a, " C");
		wrefresh(win);
		usleep(100000);
	}

	//Show "Pacman"
	wattron(win, COLOR_PAIR(Pacman));
	mvwprintw(win, 8, 12, "PACMAN");
	wrefresh(win);
	usleep(1000000);

	//Ghosts Chase Pacman
	for(a = 0; a < 23; a++) 
  {
		if(getch()!=ERR) return;
		wattron(win, COLOR_PAIR(Pellet)); mvwprintw(win, 13, 23, "*");
		wattron(win, COLOR_PAIR(Pacman)); mvwprintw(win, 13, a, " C");
		wattron(win, COLOR_PAIR(Ghost1)); mvwprintw(win, 13, a-3, " &");
		wattron(win, COLOR_PAIR(Ghost3)); mvwprintw(win, 13, a-5, " &");
		wattron(win, COLOR_PAIR(Ghost2)); mvwprintw(win, 13, a-7, " &");
		wattron(win, COLOR_PAIR(Ghost4)); mvwprintw(win, 13, a-9, " &");

		wrefresh(win);
		usleep(100000);
	}

	usleep(150000);

	//Pacman Chases Ghosts
	for(a = 25; a > 2; a--) 
  {
		if(getch()!=ERR) return;
		wattron(win, COLOR_PAIR(Pellet)); mvwprintw(win, 13, 23, " ");

		//Make ghosts half as fast
		if(a%2) b--;

		wattron(win, COLOR_PAIR(BlueGhost)); mvwprintw(win, 13, b-9, "& & & &");
		wattron(win, COLOR_PAIR(Pacman)); mvwprintw(win, 13, b-9+1, " ");
		wattron(win, COLOR_PAIR(Pacman)); mvwprintw(win, 13, b-9+3, " ");
		wattron(win, COLOR_PAIR(Pacman)); mvwprintw(win, 13, b-9+5, " ");
		wattron(win, COLOR_PAIR(Pacman)); mvwprintw(win, 13, b-9+7, " ");
		wattron(win, COLOR_PAIR(Pacman)); mvwprintw(win, 13, b-9+1, " ");
		wattron(win, COLOR_PAIR(Pacman)); mvwprintw(win, 13, b-9+3, " ");
		wattron(win, COLOR_PAIR(Pacman)); mvwprintw(win, 13, b-9+5, " ");
		wattron(win, COLOR_PAIR(Pacman)); mvwprintw(win, 13, b-9+7, " ");
		wattron(win, COLOR_PAIR(Pacman)); mvwprintw(win, 13, a-3, "C          ");
		wattron(win, COLOR_PAIR(Pellet)); mvwprintw(win, 13, 23, " ");
		wrefresh(win);
		usleep(100000);
	}
}

void LoadLevel(char *levelfile) 
{

	int a = 0; int b = 0;
	size_t l;
	char error[sizeof(LEVEL_ERR)+255] = LEVEL_ERR;
	FILE *fin;
	Food = 0;

	//Reset defaults
	Dir[0][0] =  1; Dir[0][1] =  0;
	Dir[1][0] = -1; Dir[1][1] =  0;
	Dir[2][0] =  0; Dir[2][1] = -1;
	Dir[3][0] =  0; Dir[3][1] =  1;
	Dir[4][0] =  1; Dir[4][1] =  0;
	Dir[5][0] = -1; Dir[5][1] =  0;
	Dir[6][0] =  0; Dir[6][1] = -1;
	Dir[7][0] =  0; Dir[7][1] =  1;
	Dir[8][0] =  1; Dir[8][1] =  0;

	//Open file
	fin = fopen(levelfile, "r");

	//Make sure it didn't fail
	if(!(fin)) 
  {
		l = sizeof(error)-strlen(error)-1;
		strncat(error, levelfile, l);
		if(strlen(levelfile) > l)
    {
			error[sizeof(error)-2] = '.', error[sizeof(error)-3] = '.', error[sizeof(error)-4] = '.';
    }
		ExitProgram(error);
	}

	//Open file and load the level into the array
	for(a = 0; a < 39; a++) 
  {
		for(b = 0; b < 38; b++) 
    {
			fscanf(fin, "%d", &Level[a][b]);
      
			if(Level[a][b] == 2) Food++;
			if(Level[a][b] ==  5) { Loc[0][0] = a; Loc[0][1] = b; Level[a][b] = 0; }
			if(Level[a][b] ==  6) { Loc[1][0] = a; Loc[1][1] = b; Level[a][b] = 0; }
			if(Level[a][b] ==  7) { Loc[2][0] = a; Loc[2][1] = b; Level[a][b] = 0; }
			if(Level[a][b] ==  8) { Loc[3][0] = a; Loc[3][1] = b; Level[a][b] = 0; }
			if(Level[a][b] ==  9) { Loc[4][0] = a; Loc[4][1] = b; Level[a][b] = 0; }
			if(Level[a][b] == 10) { Loc[5][0] = a; Loc[5][1] = b; Level[a][b] = 0; }
			if(Level[a][b] == 11) { Loc[6][0] = a; Loc[6][1] = b; Level[a][b] = 0; }
			if(Level[a][b] == 12) { Loc[7][0] = a; Loc[7][1] = b; Level[a][b] = 0; }
			if(Level[a][b] == 13) { Loc[8][0] = a; Loc[8][1] = b; Level[a][b] = 0; }
		}
	}

	fscanf(fin, "%d", &LevelNumber);

	//Save initial character points for if Pacman or Ghosts die
	for(a = 0; a < 9; a++)
  {
		StartingPoints[a][0] = Loc[a][0], StartingPoints[a][1] = Loc[a][1];
  }

}



void MainLoop() 
{
	DrawWindow();
	wrefresh(win);
	wrefresh(status);
	usleep(1000000);

	do 
  {
		MovePacman();	DrawWindow();	CheckCollision();
		MoveGhosts();	DrawWindow();	CheckCollision();
		if(Points > FreeLife) { Lives++; FreeLife *= 2;}
		Delay();
	}
   
  while (Food > 0);

	DrawWindow();
	usleep(1000000);
 
}

void MoveGhosts() 
{
	int a = 0; int b = 0; int c = 0;
	int tmpx = 0;  int tmpy = 0;
	int tmpdx = 0; int tmpdy = 0;
	int checksides[] = { 0, 0, 0, 0, 0, 0 };
	static int SlowerGhosts = 0;

	if(Invincible == 1) 
  {
		SlowerGhosts++;
		if(SlowerGhosts > HowSlow)
			SlowerGhosts = 0;
	}

	if((Invincible == 0) || SlowerGhosts < HowSlow)

	//Loop through each ghost
	for(a = 0; a < 8; a++)
  {

		//Switch sides?
		     if((Loc[a][0] ==  0) && (Dir[a][0] == -1)) Loc[a][0] = 38;
		else if((Loc[a][0] == 38) && (Dir[a][0] ==  1)) Loc[a][0] =  0;
		else if((Loc[a][1] ==  0) && (Dir[a][1] == -1)) Loc[a][1] = 37;
		else if((Loc[a][1] == 37) && (Dir[a][1] ==  1)) Loc[a][1] =  0;
		else 
    {
      b = 0;
      //Determine which directions we can go
      for(b = 0; b < 4; b++) checksides[b] = 0;
      if(Level[Loc[a][0] - 1][Loc[a][1]] != 1) checksides[1] = 1;
      if(Level[Loc[a][0] + 1][Loc[a][1]] != 1) checksides[0] = 1;
      if(Level[Loc[a][0]][Loc[a][1] + 1] != 1) checksides[2] = 1;
      if(Level[Loc[a][0]][Loc[a][1] - 1] != 1) checksides[3] = 1;

      //Don't do 180 unless we have to
		  c = 0; for(b = 0; b < 4; b++) if(checksides[b] == 1) c++;

		  if(c > 1) 
      {
			       if(Dir[a][0] ==  1) checksides[1] = 0;
			  else if(Dir[a][0] == -1) checksides[0] = 0;
		  	else if(Dir[a][1] ==  1) checksides[3] = 0;
			  else if(Dir[a][1] == -1) checksides[2] = 0;
		  }

		  c = 0;
		  do 
      {
		  	//Decide direction
		  	b = (int)(rand() / (1625000000 / 4));

		  	if(checksides[b] == 1) 
        {
			  	     if(b == 0) { Dir[a][0] =  1; Dir[a][1] =  0; }
			  	else if(b == 1) { Dir[a][0] = -1; Dir[a][1] =  0; }
		  		else if(b == 2) { Dir[a][0] =  0; Dir[a][1] =  1; }
		  		else if(b == 3) { Dir[a][0] =  0; Dir[a][1] = -1; }
		  	}
		  	else 
        {
  				if(Invincible == 0) 
          {
			  	//Chase Pacman
			  	     if((Loc[8][0] > Loc[a][0]) && (checksides[0] == 1)) { Dir[a][0] =  1; Dir[a][1] =  0; c = 1; }
		  		else if((Loc[8][0] < Loc[a][0]) && (checksides[1] == 1)) { Dir[a][0] = -1; Dir[a][1] =  0; c = 1; }
		  		else if((Loc[8][1] > Loc[a][1]) && (checksides[2] == 1)) { Dir[a][0] =  0; Dir[a][1] =  1; c = 1; }
			  	else if((Loc[8][1] < Loc[a][1]) && (checksides[3] == 1)) { Dir[a][0] =  0; Dir[a][1] = -1; c = 1; }
				  }

				  else 
          {
			  	//Run away from Pacman
				       if((Loc[8][0] > Loc[a][0]) && (checksides[1] == 1)) { Dir[a][0] = -1; Dir[a][1] =  0; c = 1; }
			  	else if((Loc[8][0] < Loc[a][0]) && (checksides[0] == 1)) { Dir[a][0] =  1; Dir[a][1] =  0; c = 1; }
			  	else if((Loc[8][1] > Loc[a][1]) && (checksides[3] == 1)) { Dir[a][0] =  0; Dir[a][1] = -1; c = 1; }
			  	else if((Loc[8][1] < Loc[a][1]) && (checksides[2] == 1)) { Dir[a][0] =  0; Dir[a][1] =  1; c = 1; }
			  	}
		  	}
		  } 
      while ((checksides[b] == 0) && (c == 0));

		  //Move Ghost
		  Loc[a][0] += Dir[a][0];
		  Loc[a][1] += Dir[a][1];
		}
	}
}

void MovePacman() 
{

	static int itime = 0;

	//Switch sides?
	     if((Loc[8][0] ==  0) && (Dir[8][0] == -1)) Loc[8][0] = 38;
	else if((Loc[8][0] == 38) && (Dir[8][0] ==  1)) Loc[8][0] =  0;
	else if((Loc[8][1] ==  0) && (Dir[8][1] == -1)) Loc[8][1] = 37;
	else if((Loc[8][1] == 37) && (Dir[8][1] ==  1)) Loc[8][1] =  0;
	else 
  {
		//Move Pacman
		Loc[8][0] += Dir[8][0];
		Loc[8][1] += Dir[8][1];

		//If he hit a wall, move back
		if((Level[Loc[8][0]][Loc[8][1]] == 1) || (Level[Loc[8][0]][Loc[8][1]] == 8)) 
    {
			Loc[8][0] -= Dir[8][0];	Loc[8][1] -= Dir[8][1];
		}
  }

	//What is he eating?
	switch (Level[Loc[8][0]][Loc[8][1]]) 
  {
		case 2:	//Pellet
			Level[Loc[8][0]][Loc[8][1]] = 0;
			Points++;
			Food--;
			break;
		case 3:	//PowerUp
			Level[Loc[8][0]][Loc[8][1]] = 0;
			Invincible = 1;
			if(GhostsInARow == 0) GhostsInARow = 1;
			itime = time(0);
			break;
	}

	if(Invincible == 1)  tleft = (11 - LevelNumber - time(0) + itime);

	if(tleft < 0) 
  { 
    Invincible = 0; GhostsInARow = 0; tleft = 0; 
  }

}

void PauseGame() 
{
	int chtmp;

	//Display pause dialog
	wattron(win, COLOR_PAIR(Pacman));
	mvwprintw(win, 12, 10, "********");
	mvwprintw(win, 13, 10, "*PAUSED*");
	mvwprintw(win, 14, 10, "********");
	wrefresh(win);
	
	//And wait
	do 
  {
		chtmp = getch();
	} 
  while (chtmp == ERR);
}

//Player 2
void CheckCollision2() 
{
	int a = 0;
 
	for(a = 0; a < 8; a++) 
  {
		//Collision
		if((Loc[a][0] == Loc[8][0]) && (Loc[a][1] == Loc[8][1])) 
    {

			//Ghost dies
			if((Loc[a][0] == Loc[8][0]) && (Loc[a][1] == Loc[8][1])) 
      {

			  //Ghost dies
		  	if(Invincible == 1) 
        {
	  			Points = Points + GhostsInARow * 20;
		  		mvwprintw(win, Loc[8][0], Loc[8][1] - 1, "%d", (GhostsInARow * 20));
			  	GhostsInARow *= 2;
				  wrefresh(win);
  
	  			usleep(1000000);
  
	  			Loc[a][0] = StartingPoints[a][0]; Loc[a][1] = StartingPoints[a][1];
		  	}

			  //Pacman dies
			  else 
        {
				  wattron(win, COLOR_PAIR(Pacman));
			  	mvwprintw(win, Loc[8][0], Loc[8][1], "X");

	  			wrefresh(win);

		  		Lives--;
			  	usleep(1000000);

				  if(Lives == -1) ExitProgram(END_MSG);

		  		//Reset level
	  			for(a = 0; a < 9; a++) 
          {
  					Loc[a][0] = StartingPoints[a][0];
	  				Loc[a][1] = StartingPoints[a][1];
	  			}
	  			Dir[0][0] =  1; Dir[0][1] =  0;
   				Dir[1][0] = -1; Dir[1][1] =  0;
	  			Dir[2][0] =  0; Dir[2][1] = -1;
	  			Dir[3][0] =  0; Dir[3][1] =  1;
	  			Dir[4][0] =  1; Dir[4][1] =  0;
	  			Dir[5][0] = -1; Dir[5][1] =  0;
	  			Dir[6][0] =  0; Dir[6][1] = -1;
	  			Dir[7][0] =  0; Dir[7][1] =  1;
	  			Dir[8][0] =  1; Dir[8][1] =  0;
        
	  			DrawWindow2();

	  			usleep(1000000);
			  }
	  	}
    } 
    if((Loc[a][0] == Loc[9][0]) && (Loc[a][1] == Loc[9][1]))
    {
      //Ghost dies
      if(Invincible2 == 1) 
      {
        Points = Points + GhostsInARow * 20;
				mvwprintw(win, Loc[9][0], Loc[9][1] - 1, "%d", (GhostsInARow * 20));
				GhostsInARow *= 2;
				wrefresh(win);

				usleep(1000000);

				Loc[a][0] = StartingPoints[a][0]; Loc[a][1] = StartingPoints[a][1];
			}
			//Pacman dies
			else 
      {
				wattron(win, COLOR_PAIR(Pacman));
				mvwprintw(win, Loc[9][0], Loc[9][1], "X");

				wrefresh(win);

				Lives--;
				usleep(1000000);

				if(Lives == -1) ExitProgram(END_MSG);

				//Reset level
				for(a = 0; a < 9; a++) 
        {
					Loc[a][0] = StartingPoints[a][0];
					Loc[a][1] = StartingPoints[a][1];
				}
				Dir[0][0] =  1; Dir[0][1] =  0;
				Dir[1][0] = -1; Dir[1][1] =  0;
				Dir[2][0] =  0; Dir[2][1] = -1;
				Dir[3][0] =  0; Dir[3][1] =  1;
				Dir[4][0] =  1; Dir[4][1] =  0;
				Dir[5][0] = -1; Dir[5][1] =  0;
				Dir[6][0] =  0; Dir[6][1] = -1;
				Dir[7][0] =  0; Dir[7][1] =  1;
				Dir[8][0] =  1; Dir[8][1] =  0;
        Dir[9][0] =  -1; Dir[9][1] =  0;
				DrawWindow2();

				usleep(1000000);
			}
		}
	}
}

void Delay2() 
{

	struct timeb t_start, t_current;
	ftime(&t_start);

	//Slow down the game a little bit
	do 
  {
		GetInput2();
		ftime(&t_current);
	} while (abs(t_start.millitm - t_current.millitm) < SpeedOfGame);
}

void DrawWindow2() 
{
	int a = 0; int b = 0;
	char chr = ' ';
	int attr;

	//Display level array
	for(a = 0; a < 39; a++) for(b = 0; b < 38; b++) 
  {
		switch(Level[a][b]) 
    {
		case 0: chr = ' '; attr = A_NORMAL; wattron(win, COLOR_PAIR(Normal));    break;
		case 1: chr = ' '; attr = A_NORMAL; wattron(win, COLOR_PAIR(Wall));      break;
		case 2: chr = '.'; attr = A_NORMAL; wattron(win, COLOR_PAIR(Pellet));    break;
		case 3: chr = '*'; attr = A_BOLD;   wattron(win, COLOR_PAIR(PowerUp));   break;
		case 4: chr = ' '; attr = A_NORMAL; wattron(win, COLOR_PAIR(GhostWall)); break;
		}
		mvwaddch(win, a, b, chr | attr);
	}

	//Display number of lives, score, and level
	attr = A_NORMAL;
	wmove(status, 1, 1);
	wattron(status, COLOR_PAIR(Pacman));
	for(a = 0; a < Lives; a++)
  {
		wprintw(status, "C ");
  }
	wprintw(status, "  ");
	wattron(status, COLOR_PAIR(Normal));
	mvwprintw(status, 2, 2, "Level: %d     Score: %d ", LevelNumber, Points);
	wrefresh(status);

	//Display ghosts
	if(Invincible == 0) 
  {
		wattron(win, COLOR_PAIR(Ghost1)); mvwaddch(win, Loc[0][0], Loc[0][1], '&');
		wattron(win, COLOR_PAIR(Ghost2)); mvwaddch(win, Loc[1][0], Loc[1][1], '&');
		wattron(win, COLOR_PAIR(Ghost3)); mvwaddch(win, Loc[2][0], Loc[2][1], '&');
		wattron(win, COLOR_PAIR(Ghost4)); mvwaddch(win, Loc[3][0], Loc[3][1], '&');
		wattron(win, COLOR_PAIR(Ghost5)); mvwaddch(win, Loc[4][0], Loc[4][1], '&');
		wattron(win, COLOR_PAIR(Ghost6)); mvwaddch(win, Loc[5][0], Loc[5][1], '&');
		wattron(win, COLOR_PAIR(Ghost7)); mvwaddch(win, Loc[6][0], Loc[6][1], '&');
		wattron(win, COLOR_PAIR(Ghost8)); mvwaddch(win, Loc[7][0], Loc[7][1], '&');
	}

	//OR display vulnerable ghosts
	else 
  {
		wattron(win, COLOR_PAIR(BlueGhost));
		mvwaddch(win, Loc[0][0], Loc[0][1], tleft + '0');
		mvwaddch(win, Loc[1][0], Loc[1][1], tleft + '0');
		mvwaddch(win, Loc[2][0], Loc[2][1], tleft + '0');
		mvwaddch(win, Loc[3][0], Loc[3][1], tleft + '0');
		mvwaddch(win, Loc[4][0], Loc[4][1], tleft + '0');
		mvwaddch(win, Loc[5][0], Loc[5][1], tleft + '0');
		mvwaddch(win, Loc[6][0], Loc[6][1], tleft + '0');
		mvwaddch(win, Loc[7][0], Loc[7][1], tleft + '0');
	}

	//Display Pacman
	wattron(win, COLOR_PAIR(Pacman)); 
  mvwaddch(win, Loc[8][0], Loc[8][1], 'C');
  
  wattron(win, COLOR_PAIR(Pacman2)); 
  mvwaddch(win, Loc[9][0], Loc[9][1], 'C');

	wrefresh(win);
}

void GetInput2() 
{
	int ch;

	static int chtmp;

	ch = getch();

	//Buffer input
	if(ch == ERR) ch = chtmp;
	chtmp = ch;

	switch (ch) 
  {
		case 'w': case 'W':  
			if((Level[(Loc[8][0]-1) % 39][Loc[8][1]] != 1)
			&& (Level[(Loc[8][0]-1) % 39][Loc[8][1]] != 8))
				{ Dir[8][0] = -1; Dir[8][1] =  0; }
			break;

		case 's': case 'S':   
			if((Level[(Loc[8][0]+1) % 39][Loc[8][1]] != 1)
			&& (Level[(Loc[8][0]+1) % 39][Loc[8][1]] != 8))
				{ Dir[8][0] =  1; Dir[8][1] =  0; }
			break;

		case 'a': case 'A': 
			if((Level[Loc[8][0]][(Loc[8][1]-1) % 38] != 1)
			&& (Level[Loc[8][0]][(Loc[8][1]-1) % 38] != 8))
				{ Dir[8][0] =  0; Dir[8][1] = -1; }
			break;

		case 'd': case 'D':
			if((Level[Loc[8][0]][(Loc[8][1]+1) % 38] != 1)
			&& (Level[Loc[8][0]][(Loc[8][1]+1) % 38] != 8))
				{ Dir[8][0] =  0; Dir[8][1] =  1; }
			break;
      
   case KEY_UP: 
			if((Level[(Loc[9][0]-1) % 39][Loc[9][1]] != 1)
			&& (Level[(Loc[9][0]-1) % 39][Loc[9][1]] != 9))
				{ Dir[9][0] = -1; Dir[9][1] =  0; }
			break;

		case KEY_DOWN:
			if((Level[(Loc[9][0]+1) % 39][Loc[9][1]] != 1)
			&& (Level[(Loc[9][0]+1) % 39][Loc[9][1]] != 9))
				{ Dir[9][0] =  1; Dir[9][1] =  0; }
			break;

		case KEY_LEFT:
			if((Level[Loc[9][0]][(Loc[9][1]-1) % 38] != 1)
			&& (Level[Loc[9][0]][(Loc[9][1]-1) % 38] != 9))
				{ Dir[9][0] =  0; Dir[9][1] = -1; }
			break;

		case KEY_RIGHT:
			if((Level[Loc[9][0]][(Loc[9][1]+1) % 38] != 1)
			&& (Level[Loc[9][0]][(Loc[9][1]+1) % 38] != 9))
				{ Dir[9][0] =  0; Dir[9][1] =  1; }
			break;

		case 'p': case 'P':
			PauseGame();
			chtmp = getch();
			break;

		case 'q': case 'Q':
			ExitProgram(QUIT_MSG);
			break;

	}
}

void LoadLevel2(char *levelfile) 
{

	int a = 0; int b = 0;
	size_t l;
	char error[sizeof(LEVEL_ERR)+255] = LEVEL_ERR;
	FILE *fin;
	Food = 0;

	//Reset defaults
	Dir[0][0] =  1; Dir[0][1] =  0;
	Dir[1][0] = -1; Dir[1][1] =  0;
	Dir[2][0] =  0; Dir[2][1] = -1;
	Dir[3][0] =  0; Dir[3][1] =  1;
	Dir[4][0] =  1; Dir[4][1] =  0;
	Dir[5][0] = -1; Dir[5][1] =  0;
	Dir[6][0] =  0; Dir[6][1] = -1;
	Dir[7][0] =  0; Dir[7][1] =  1;
	Dir[8][0] =  1; Dir[8][1] =  0;
  Dir[9][0] = -1; Dir[9][1] =  0;

	//Open file
	fin = fopen(levelfile, "r");

	//Make sure it didn't fail
	if(!(fin)) 
  {
		l = sizeof(error)-strlen(error)-1;
		strncat(error, levelfile, l);
		if(strlen(levelfile) > l)
    {
			error[sizeof(error)-2] = '.', error[sizeof(error)-3] = '.', error[sizeof(error)-4] = '.';
    }
		ExitProgram(error);
	}

	//Open file and load the level into the array
	for(a = 0; a < 39; a++) 
  {
		for(b = 0; b < 38; b++) 
    {
			fscanf(fin, "%d", &Level[a][b]);
      
			if(Level[a][b] == 2) Food++;
			if(Level[a][b] ==  5) { Loc[0][0] = a; Loc[0][1] = b; Level[a][b] = 0; }
			if(Level[a][b] ==  6) { Loc[1][0] = a; Loc[1][1] = b; Level[a][b] = 0; }
			if(Level[a][b] ==  7) { Loc[2][0] = a; Loc[2][1] = b; Level[a][b] = 0; }
			if(Level[a][b] ==  8) { Loc[3][0] = a; Loc[3][1] = b; Level[a][b] = 0; }
			if(Level[a][b] ==  9) { Loc[4][0] = a; Loc[4][1] = b; Level[a][b] = 0; }
			if(Level[a][b] == 10) { Loc[5][0] = a; Loc[5][1] = b; Level[a][b] = 0; }
			if(Level[a][b] == 11) { Loc[6][0] = a; Loc[6][1] = b; Level[a][b] = 0; }
			if(Level[a][b] == 12) { Loc[7][0] = a; Loc[7][1] = b; Level[a][b] = 0; }
			if(Level[a][b] == 13) { Loc[8][0] = a; Loc[8][1] = b; Level[a][b] = 0; }
      if(Level[a][b] == 14) { Loc[9][0] = a; Loc[9][1] = b; Level[a][b] = 0; }
		}
	}

	fscanf(fin, "%d", &LevelNumber);

	//Save initial character points for if Pacman or Ghosts die
	for(a = 0; a < 10; a++)
  {
		StartingPoints[a][0] = Loc[a][0], StartingPoints[a][1] = Loc[a][1];
  }

}

void MainLoop2() 
{

	DrawWindow2();
	wrefresh(win);
	wrefresh(status);
	usleep(1000000);

	do 
  {
		MovePacman2();	DrawWindow2();	CheckCollision2();
		MoveGhosts();	DrawWindow2();	CheckCollision2();
		if(Points > FreeLife) { Lives++; FreeLife *= 2;}
		Delay2();

	} 
  
  while (Food > 0);

	DrawWindow2();
	usleep(1000000);

}

void MovePacman2() 
{

	static int itime = 0;

	//Switch sides?
	     if((Loc[8][0] ==  0) && (Dir[8][0] == -1)) Loc[8][0] = 38;
	else if((Loc[8][0] == 38) && (Dir[8][0] ==  1)) Loc[8][0] =  0;
	else if((Loc[8][1] ==  0) && (Dir[8][1] == -1)) Loc[8][1] = 37;
	else if((Loc[8][1] == 37) && (Dir[8][1] ==  1)) Loc[8][1] =  0;
  else if((Loc[9][0] ==  0) && (Dir[9][0] == -1)) Loc[9][0] = 38;
	else if((Loc[9][0] == 38) && (Dir[9][0] ==  1)) Loc[9][0] =  0;
	else if((Loc[9][1] ==  0) && (Dir[9][1] == -1)) Loc[9][1] = 37;
	else if((Loc[9][1] == 37) && (Dir[9][1] ==  1)) Loc[9][1] =  0;
	else 
  {
		//Move Pacman
		Loc[8][0] += Dir[8][0];
		Loc[8][1] += Dir[8][1];
    Loc[9][0] += Dir[9][0];
		Loc[9][1] += Dir[9][1];

		//If he hit a wall, move back
		if((Level[Loc[8][0]][Loc[8][1]] == 1) || (Level[Loc[8][0]][Loc[8][1]] == 8))
    {
			Loc[8][0] -= Dir[8][0];	Loc[8][1] -= Dir[8][1];
		}
   	if((Level[Loc[9][0]][Loc[9][1]] == 1) || (Level[Loc[9][0]][Loc[9][1]] == 9))
    {
			Loc[9][0] -= Dir[9][0];	Loc[9][1] -= Dir[9][1];
		}
	}

	//What is he eating?
	switch (Level[Loc[8][0]][Loc[8][1]]) 
  {
		case 2:	//Pellet
			Level[Loc[8][0]][Loc[8][1]] = 0;
			Points++;
			Food--;
			break;
		case 3:	//PowerUp
			Level[Loc[8][0]][Loc[8][1]] = 0;
			Invincible = 1;
			if(GhostsInARow == 0) GhostsInARow = 1;
			itime = time(0);
			break;
	}
  switch (Level[Loc[9][0]][Loc[9][1]]) 
  {
    case 2:	//Pellet
      Level[Loc[9][0]][Loc[9][1]] = 0;
			Points++;
			Food--;
			break;
		case 3:	//PowerUp
			Level[Loc[9][0]][Loc[9][1]] = 0;
			Invincible = 1;
			if(GhostsInARow == 0) GhostsInARow = 1;
			itime = time(0);
			break;
  }

	if(Invincible == 1)  tleft = (11 - LevelNumber - time(0) + itime);
 
  if(Invincible2 == 1)  tleft = (11 - LevelNumber - time(0) + itime);
  
	if(tleft < 0) 
  { 
    Invincible = 0; GhostsInARow = 0; tleft = 0; 
  }
}