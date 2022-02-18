/*
  Name: Raghav Marwah
  Last Edit: 06/04/2016
  About Program: PONG-2D Table Tennis Simulator (Custom Edition)
*/

/*	CHANGELOG

	v0.5	Added life bar with 5 lives (now the game is not endless), added game overscreen.

	v0.6	Bug causing player to lose when ball touches corner of the bat fixed.

	v0.7	Loading screen and main menu attached to the game, Minor UI enhancements.

	v0.8	Added pause game functionality.

	v0.9	Removed unused variables, some code refinement.

	v1.0	Divided source code into functions, Milestone 1 (01/13/2015)

	v1.1	Fixed main menu REDRAW issue, added scorebox.

	v1.2	Added transition effects, optimised ui, difficulty selection, bug fixes.

	v1.3	Added color settings for ball and bat.

	v1.4	Highscore file integrated, highscore reset option added.

	v1.5	Login and New Profile screen added. (Please use only one option at a time!)

	v1.6	Fixed username & password input related bugs, Changed main menu elements.

	v1.7	Fixed conflicting profile issue. (The game accepted pre-existing usernames)

	v1.8	Code refinement minor UI changes..

	v1.9	Fixed use only one option forced on players in mainmenu and login..

	v2.0   	Milestone 2 (12/15/2015)

*/

#include<fstream.h>
#include<conio.h>
#include<graphics.h>
#include<dos.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

struct myScore
{
	long advScore;
	long begScore;
	char advPlayer[11];
	char begPlayer[11];
	myScore()
	{	advScore=894;
		begScore=894;
		strcpy(advPlayer,"NULL");
		strcpy(begPlayer,"NULL");
		}
};
class profile
{	char usrname[11];
	char password[11];
	public:
	char* rUS()
	{	return usrname;
	}
	char* rPS()
	{	return password;
	}
	void inUS(char u[])
	{	strcpy(usrname,u);
	}
	void inPS(char w[])
	{       strcpy(password,w);
	}
	void modUS();
	void modPS();

};
void profile::modUS()
{	cout<<"\n\n=> Current Username: "<<usrname;
	cout<<"\n=> Enter new Username: ";
	gets(usrname);
}
void profile::modPS()
{	cout<<"\n=> Current Password: "<<password;
	cout<<"\n=> Enter new Passowrd: ";
	gets(password);
}

char myPlayer[11];	//Global Player Username

//||||||||||||||||||||||| LIST OF FUCNTIONS |||||||||||||||||||||
void gamebegin();
void mainmenu();
void maingame(float,float,float,int,int,int);
void login();
void newuser();
void choice();
void transition(int);
void modeselect(int,int);
void pause();
void profile_set();
void settings();
void reset();
long crypt(int);
int decrypt(long);

void main()
{
	int mode,driver=DETECT;
	initgraph(&driver,&mode,"//BGI");

	gamebegin();	//Calls the loading screen fuction
	choice();	//Calls the function with login and signup options
	mainmenu();	//Calls the main menu function
}

//|||||||||||||||||||||||||| PONG GAME |||||||||||||||||||||||||
void maingame(float x1, float x2, float x3, int ballC, int batC, int mode)
{
	cleardevice();
	int hscore;
	myScore M;
	fstream highscr;
	highscr.open("hgscr.pg",ios::in|ios::binary);
	highscr.read((char*)&M,sizeof(M));
	if(mode==0){
		long adS=M.begScore;
		hscore=decrypt(adS);
	}
	else if(mode==1){
		long bgS=M.advScore;
		hscore=decrypt(bgS);
	}
	highscr.close();
	randomize();
	int yp1=250, yn1=310, move;
	float yp2=250, yn2=310;
	float xb=100, yb=280;
	int xgov=1, ygov=random(2);	//xgov & ygov determine ball movement
	int lives=5, score=0, debug=564;
	char scr[25], hscr[25];

	setbkcolor(2);

	/*Court Boundry Design*/
	setfillstyle(1,15);
	bar(20,87,620,97);	//Top line of court
	bar(20,450,620,460);		//Bottom line of court
	bar(20,90,30,460);	///left line of court
	bar(620,90,610,450);		//Right line of court
	bar(317,97,323,449);	//Mid line of court
	circle(320,275,35);
	circle(320,275,25);

	setfillstyle(6,15);
	bar(45,98,55,449);	//Left separator
	bar(585,98,595,449);	//Right separator

	setcolor(15);
	settextstyle(0,0,2);
	outtextxy(10,30,"Lives Left:-");
	setfillstyle(1,8);      //LifeBox Design
	bar(220,15,600,65);
	setcolor(15);
	circle(270,40,20);     //Lives remaining indicator
	circle(340,40,20);
	circle(410,40,20);
	circle(480,40,20);
	circle(550,40,20);
	setfillstyle(1,4);
	floodfill(270,40,15);
	floodfill(340,40,15);
	floodfill(410,40,15);
	floodfill(480,40,15);
	floodfill(550,40,15);

	setcolor(WHITE);
	settextstyle(2,0,6);
	outtextxy(270,460,"HITS");

	//Overall Game Do-While Loop
	do{
		int t1=0, t2=0;
		float t3=0, t4=0;

		setcolor(LIGHTGREEN);
		if(lives==4)              //Lives reduced indicator
		floodfill(550,40,15);
		else if(lives==3)
		floodfill(480,40,15);
		else if(lives==2)
		floodfill(410,40,15);
		else if(lives==1)
		floodfill(340,40,15);
		else if(lives==0)	//No lives remaining
		{
			floodfill(270,40,15);
			delay(400);
			transition(10);
			itoa(score,scr,10);
			cleardevice();
			setbkcolor(0);
			setcolor(15);
			settextstyle(0,0,7);
			outtextxy(70,200,"Game Over");
			setcolor(WHITE);
			settextstyle(2,0,6);
			outtextxy(490,0,"Mode:");
			if(mode==0)
			{	outtextxy(540,0,"Beginner");
				if(score>hscore)
				{       hscore=score;
					M.begScore=crypt(score);
					strcpy(M.begPlayer,myPlayer);
					highscr.open("hgscr.pg",ios::out|ios::trunc|ios::binary);
					highscr.write((char*)&M,sizeof(M));
					highscr.close();
				}}
			else if(mode==1)
			{	outtextxy(540,0,"Advanced");
				if(score>hscore)
				{       hscore=score;
					M.advScore=crypt(score);
					strcpy(M.advPlayer,myPlayer);
					highscr.open("hgscr.pg",ios::out|ios::trunc|ios::binary);
					highscr.write((char*)&M,sizeof(M));
					highscr.close();
				}}
			outtextxy(430,460,"Press Esc. To Return");
			outtextxy(10,460,"Your Score:");
			outtextxy(120,460,scr);
			itoa(hscore,hscr,10);
			outtextxy(10,0,"High Score:");
			outtextxy(120,0,hscr);
			if(mode==0)
			outtextxy(160,0,M.begPlayer);
			else if(mode==1)
			outtextxy(160,0,M.advPlayer);

			int gmovr;
			do{
				gmovr=0;
				if(kbhit())
				gmovr=getch();
			}while(gmovr!=27);

			mainmenu(); 	}  //Return to main menu

		setfillstyle(1,15);	//Mid court redesign
		setcolor(15);
		bar(317,97,323,449);
		circle(320,275,35);
		circle(320,275,25);

		setfillstyle(1,batC);	//Player1
		bar(570,yp1,580,yn1);

		setfillstyle(1,4);	//Player2
		bar(60,yp2,70,yn2);

		/*SCOREBOX*/
		setfillstyle(1,8);
		bar(340,462,390,480);
		itoa(score,scr,10);
		setcolor(WHITE);
		settextstyle(2,0,6);
		outtextxy(345,460,scr);

		setfillstyle(1,2);	//Ball Design
		bar(xb-8,yb-8,xb+8,yb+8);
		setfillstyle(1,ballC);
		setcolor(ballC);
		circle(xb,yb,6);
		floodfill(xb,yb,ballC);

		move=0;			//Disable extra movement

		if(kbhit())
		move=getch();

		if(move==72)		//Upward Movement
		{	yp1=yp1-50;
			yn1=yn1-50;

			if(yp1<=100)	//Limit Upward Movement
			{       yp1=yp1+50;
				yn1=yn1+50;
				setfillstyle(1,2);
				bar(570,yp1,580,yn1);
				yp1=110;
				yn1=170;
				setfillstyle(1,1);
				bar(570,yp1,580,yn1);
			}
			else
			{	setfillstyle(1,2);
				t1=yn1+55;
				bar(570,yn1,580,t1);
			}
		}


		else if(move==80)	   //Downward Movement
		{	yp1=yp1+50;
			yn1=yn1+50;

			if(yn1>=450)       //Limit Downward Movement
			{   	yp1=yp1-50;
				yn1=yn1-50;
				setfillstyle(1,2);
				bar(570,yp1,580,yn1);
				yp1=380;
				yn1=440;
				setfillstyle(1,1);
				bar(570,yp1,580,yn1);

			}
			else
			{	setfillstyle(1,2);
				t2=yp1-55;
				bar(570,yp1,580,t2);
			}
		}
		else if(move==32)
		pause();	       //Pause the game
		else if(move==42)
		{	if(debug==564)
			debug=550;
			else if (debug==550)
			debug=564;
		}

	if(yb>142 && yb<410)	//Limits AI to cross boundry
	{	yp2=yb-30;
		yn2=yb+30;
		}

	if(yp2<=100)
	{	yp2=yp2+1;
		yn2=yn2+1;
	}
	else
	{	setfillstyle(1,2);
		t3=yn2+10;
		bar(60,yn2,70,t3);
	}

	if(yn2>=450)
	{	yp2=yp2-1;
		yn2=yn2-1;
	}
	else
	{	setfillstyle(1,2);
		t4=yp2-10;
		bar(60,yp2,70,t4);
	}

	if(xb<76)
		xgov=1;		//+ve x-dir movement
	if(xb>debug)//564||550
	{	xgov=2;		//-ve x-dir movement
		score++;
	}
	if(yb<110)
		ygov=1;		//-ve y-dir movement
	if(yb>440)
		ygov=2;		//+ve y-dir movement

	//Update Ball x & y co-ordinates
	if(xgov==1 && ygov==1)
	{	xb=xb+x1;
		yb=yb+x1;
	}

	if(xgov==1 && ygov==2)
	{	xb=xb+x1;
		yb=yb-x1;
	}

	if(xgov==2 && ygov==1)
	{	xb=xb-x2;
		yb=yb+x3;
	}

	if(xgov==2 && ygov==2)
	{	xb=xb-x1;
		yb=yb-x3;
	}

	//What if player loses??
	if(xb>=563)
		if(yb<(yp1-9) || yb>(yn1+9))
		{       sound(700);
			delay(130);
			nosound();
			setfillstyle(1,2);
			bar(xb-8,yb-9,xb+8,yb+9); //Reset Ball
			xb=100;
			yb=280;
			xgov=1;			//Reset movement
			ygov=1;
			setfillstyle(1,2);	//Reset AI player
			bar(60,yp2,70,yn2);
			yp2=250;
			yn2=310;
			lives--;
			score--;		//Reduce life
			delay(970);
		}

	}while(move!=27);
	highscr.close();
	cleardevice();
	mainmenu();
}

//||||||||||||||||||||||| GAME LOADING SCREEN |||||||||||||||||||
void gamebegin()
{	cleardevice();
	setbkcolor(0);

	rectangle(0,0,638,479);
	rectangle(160,420,480,430);
	settextstyle(0,0,8);
	outtextxy(100,50,"PONG-2D");

	setcolor(BLUE);
	setfillstyle(1,BLUE);
	pieslice(100,240,0,360,80);

	setcolor(RED);
	setfillstyle(1,RED);
	pieslice(540,240,0,360,80);

	setfillstyle(1,BLACK);
	bar(2,315,636,320);

	setfillstyle(1,BROWN);
	bar(80,315,120,405);
	bar(520,315,560,405);

	setfillstyle(1,WHITE);
	bar(98,325,102,395);
	bar(538,325,542,395);
	setcolor(WHITE);
	pieslice(320,360,0,360,30);

	for(float i=0;i<=290;i+=1)	//Progress Bar
	{
		setfillstyle(2,7);
		bar(160+i,420,190+i,430);
		delay(15);
	}

	setcolor(WHITE);
	settextstyle(2,0,6);
	outtextxy(200,440,"Press Any Key To Continue");

	getch();
	cleardevice();

}

//||||||||||||||||||||||||| MAIN MENU ||||||||||||||||||||||||||||
void mainmenu()
{	char *choice = new char;   //Dynamic Memory Allocation
	int nballC=14, nbatC=1;
	cleardevice();
	setbkcolor(BLACK);
	setcolor(15);
	settextstyle(0,0,7);
	outtextxy(120,30,"PONG-2D");
	//Menu selection options..
	settextstyle(2,0,45);
	outtextxy(115,135,"1. Start");
	outtextxy(115,175,"2. Instructions");
	outtextxy(115,215,"3. About Project");
	outtextxy(115,255,"4. Profile");
	outtextxy(115,295,"5. Settings");
	outtextxy(115,335,"6. Reset");
	outtextxy(115,375,"7. Exit");
	//Displaying logged in username and game version..
	settextstyle(2,0,6);
	outtextxy(5,460,"Welcome:");
	outtextxy(85,460,myPlayer);
	outtextxy(510,460,"version: 2.0.S");

	do{
	*choice='0';
	*choice=getch();
	switch(*choice)
	{
		case '1':       delete choice;
				modeselect(nballC,nbatC);
				break;

		case '3':	cleardevice();
				setbkcolor(BLACK);
				setcolor(15);
				settextstyle(0,0,4);
				outtextxy(120,30,"About Project");
				settextstyle(3,0,1);
				outtextxy(5,160,"Developer:- Raghav Marwah");
				outtextxy(5,250,"PONG-2D is based on the original PONG game by Atari.");
				outtextxy(5,290,"This game was my Grade 12 CS project.");
                outtextxy(5,330,"Big thanks to my high school CS teacher Ms. Prajjya D.");
				outtextxy(5,370,"St. Cecilia's Public School (New Delhi, India)");
				getch();
				delete choice;
				mainmenu();
				break;


		case '2':	cleardevice();
				setbkcolor(BLACK);
				setcolor(15);
				settextstyle(0,0,4);
				outtextxy(120,30,"Instructions");
				settextstyle(3,0,1);
				outtextxy(5,140,"Your player (BLUE) resides on the right side of the screen.");
				outtextxy(5,180,"Your objective is to play as long as you can vs. the computer.");
				outtextxy(5,220,"The AI will never lose, so your only objective is to save your life.");
				outtextxy(5,300,"*Initially you have 5 lives indicated at the top right of your screen");
				outtextxy(5,340,"*Use UP/DOWN arrow keys to move your controller...");
				outtextxy(5,380,"*Press SPACE to pause and ESC to return to main menu..");
				outtextxy(5,420,"*Each life you lose you are charged 1 point..");
				getch();
				delete choice;
				mainmenu();
				break;

		case '4':       delete choice;
				profile_set();
				break;

		case '5':       delete choice;
				settings();
				break;

		case '6':       delete choice;
				reset();
				break;

		case '7':       delete choice;
				exit(0);
	}
	}while(*choice<'1'||*choice>'7');

	cleardevice();
}


//|||||||||||||||||||||||| IN-GAME PAUSE |||||||||||||||||||||||||||
void pause()
{	int ps;
	setcolor(WHITE);
	settextstyle(2,0,7);
	outtextxy(60,55,"PAUSED");
	do{
	ps=0;
	if(kbhit())
	ps=getch();
	if(ps==27)
	{	cleardevice();
		mainmenu();
		}
	}while(ps!=32);		//Infinite loop until space is pressed
	setcolor(GREEN);
	setfillstyle(0,GREEN);
	bar(55,50,155,80);
}
//||||||||||||||||||||| TRANSITION ||||||||||||||||||||||||||||||||||
void transition(int sp)
{
	for(int tl=320,tu=240,tr=320,td=240;tl>=0&&tu>=0;tl-=sp,tu-=sp,tr+=sp,td+=sp)
	{
		setcolor(0);
		setfillstyle(0,0);
		bar(tl,tu,tr,td);
		}
}
//|||||||||||||||||||| MODE SELECTION ||||||||||||||||||||||||
void modeselect(int mballC,int mbatC)
{
	cleardevice();
	setbkcolor(BLACK);
	setcolor(15);
	settextstyle(0,0,2);
		outtextxy(130,30,"Select Difficulty Level");
	settextstyle(2,0,7);
		outtextxy(270,163,"Beginner");
		outtextxy(270,200,"Advanced");
	setfillstyle(1,15);
		setcolor(15);
		circle(260,175,5);
		floodfill(260,175,15);

	int select=0, move=0;

	do{
		move=getch();
		if(move==72)
		{	select=0;
			setcolor(0);
			setfillstyle(1,0);
			bar(255,210,265,220);
			setfillstyle(1,15);
			setcolor(15);
			circle(260,175,5);
			floodfill(260,175,15);
		}
		else if(move==80)
		{	select=1;
			setcolor(0);
			setfillstyle(1,0);
			bar(255,170,265,180);
			setfillstyle(1,15);
			setcolor(15);
			circle(260,215,5);
			floodfill(260,215,15);
		}
		else if(move==27)
			mainmenu();
	}while(move!=13);

	transition(10);
	if(select==0)	//Beginner Mode
	maingame(1.0,1.1,1.2,mballC,mbatC,select);
	else if(select==1)	//Advanced Mode
	maingame(1.2,1.3,1.4,mballC,mbatC,select);
}
//||||||||||||||||||||| SETTINGS |||||||||||||||||||||||
void settings()
{	cleardevice();
	int xballC, xbatC;
	setbkcolor(BLACK);
	setcolor(15);
	settextstyle(0,0,4);
	outtextxy(190,30,"Settings");
	line(0,120,640,120);
	line(0,385,640,385);
	line(0,410,640,410);
	line(320,120,320,410);
	settextstyle(2,0,7);
	outtextxy(20,124,"Select Ball Color:");
	outtextxy(20,163,"1.Yellow");
	outtextxy(20,200,"2.Dark Grey");
	outtextxy(20,237,"3.Brown");
	outtextxy(20,274,"4.White");
	outtextxy(20,311,"5.Magenta");
	outtextxy(20,348,"6.Light Blue");
	outtextxy(340,124,"Select Bat Color:");
	outtextxy(340,163,"1.Blue");
	outtextxy(340,200,"2.Red");
	outtextxy(340,237,"3.White");
	outtextxy(340,274,"4.Yellow");
	outtextxy(340,311,"5.Brown");
	outtextxy(340,348,"6.Dark Grey");
	char xchoice;

	setfillstyle(1,15);
	setcolor(15);
	circle(9,135,5);
	floodfill(9,135,15);
	//Ball Color Input
	do{
		xchoice=0;
		xchoice=getch();
		switch(xchoice)
		{	case '1': xballC=14;
				  outtextxy(20,385,"=> Yellow");
				  break;
			case '2': xballC=8;
				  outtextxy(20,385,"=> Dark Grey");
				  break;
			case '3': xballC=6;
				  outtextxy(20,385,"=> Brown");
				  break;
			case '4': xballC=15;
				  outtextxy(20,385,"=> White");
				  break;
			case '5': xballC=5;
				  outtextxy(20,385,"=> Magenta");
				  break;
			case '6': xballC=9;
				  outtextxy(20,385,"=> Light Blue");
				  break;
		}
	}while(xchoice<'1'||xchoice>'6');

	setcolor(0);
	setfillstyle(1,0);
	bar(0,129,15,141);
	setfillstyle(1,15);
	setcolor(15);
	circle(329,135,5);
	floodfill(329,135,15);
	//Bat Color Input
	do{
		xchoice=0;
		xchoice=getch();
		switch(xchoice)
		{	case '1': xbatC=1;
				  outtextxy(340,385,"=> Blue");
				  break;
			case '2': xbatC=4;
				  outtextxy(340,385,"=> Red");
				  break;
			case '3': xbatC=15;
				  outtextxy(340,385,"=> White");
				  break;
			case '4': xbatC=14;
				  outtextxy(340,385,"=> Yellow");
				  break;
			case '5': xbatC=6;
				  outtextxy(340,385,"=> Brown");
				  break;
			case '6': xbatC=8;
				  outtextxy(340,385,"=> Dark Grey");
				  break;
		}
	}while(xchoice<'1'||xchoice>'6');
	setcolor(0);
	setfillstyle(1,0);
	bar(322,129,335,141);
	setcolor(WHITE);
	settextstyle(2,0,6);
	outtextxy(180,460,"Press Any Key To Launch Game");
	getch();
	modeselect(xballC,xbatC);
}
//|||||||||||||||||||||||||| CRYPT ||||||||||||||||||||||||||
long crypt(int sc)
{ 	long r_score;
	r_score=(7*sc+298)*3;//Crypting the score..
	return r_score;
}
//|||||||||||||||||||||||| DECRYPT |||||||||||||||||||||||
int decrypt(long csc)
{	int l_score;
	l_score=(csc/3-298)/7;//Decrypting the score..
	return l_score;
}
//||||||||||||||||||||||||| RESET|||||||||||||||||||||||||||
void reset()
{       cleardevice();
	setbkcolor(BLACK);
	setcolor(15);
	settextstyle(8,0,5);
	outtextxy(130,30,"Highscore Reset");
	settextstyle(2,0,6);
	outtextxy(150,200,"Do you really wanna do it? {Y/N}");
	char yn;
	do{
	yn=getch();
	if(yn=='y'||yn=='Y')
	{	outtextxy(280,300,"DONE!");
		myScore M1;
		fstream resetFile;
		//File open in trunc to delete all existing data..
		resetFile.open("hgscr.pg",ios::out|ios::trunc|ios::binary);
		resetFile.write((char*)&M1,sizeof(M1));
		resetFile.close();
		getch();
		mainmenu();
		}
	else if(yn=='n'||yn=='N')
		mainmenu();
		}while(yn!='y'||yn!='n'||yn!='N'||yn!='Y');
}
//|||||||||||||||||||||| LOGIN |||||||||||||||||||||||
void login()
{
	cleardevice();

	//Layout Design
	int check=0;
	int i,j;
	setcolor(DARKGRAY);
	setfillstyle(7,DARKGRAY);
	bar(0,0,640,480);
	setcolor(BLACK);
	setfillstyle(1,BLACK);
	bar(112,20,528,480);
	setcolor(BLUE);
	setfillstyle(1,BLUE);
	fillellipse(320,240,150,180);
	setcolor(BLACK);
	setfillstyle(1,BLACK);
	bar(120,380,520,480);
	setcolor(BROWN);
	setfillstyle(1,BROWN);
	bar(285,380,355,480);
	setcolor(WHITE);
	setfillstyle(1,WHITE);
	bar(315,390,325,470);
	setcolor(LIGHTGRAY);
	setfillstyle(1,LIGHTGRAY);
	bar(260,98,380,132);
	bar(300,200,450,223);
	bar(300,240,450,263);
	setcolor(WHITE);
	settextstyle(2,0,25);
	outtextxy(265,90,"LOGIN");
	settextstyle(2,0,7);
	outtextxy(185,200,"Username: ");
	outtextxy(185,240,"Password: ");

	//Functional Elements
	setcolor(WHITE);
	reset_usrname_login:
	setcolor(BLUE);
	setfillstyle(1,BLUE);
	bar(236,325,409,340);
	setcolor(WHITE);
	settextstyle(2,0,6);
	outtextxy(310,202,"10 characters");
	outtextxy(310,242,"10 characters");
	char us[11], ps[11], x[2];
	setfillstyle(1,15);
	setcolor(15);
	circle(458,212,4);
	floodfill(458,212,15);
	us[0]=getch();
	x[0]=us[0];
		if(x[0]==27)//Esc
			choice();
		else if(x[0]==13)//Enter
		{	setcolor(LIGHTGRAY);
			setfillstyle(1,LIGHTGRAY);
			bar(236,325,409,340);
			setcolor(WHITE);
			settextstyle(2,0,6);
			outtextxy(240,322,"No Input Provided!");
			delay(500);
			goto reset_usrname_login;
		}
		else if(x[0]==8)//Backspace
			goto reset_usrname_login;
	x[1]='\0';
	setcolor(LIGHTGRAY);
	setfillstyle(1,LIGHTGRAY);
	bar(300,200,450,224);
	setcolor(WHITE);
	settextstyle(2,0,6);
	outtextxy(333,202,x);
	i=1; j=8;
	do{
		check=0;
		if(i<10)
		{	us[i]=getch();
			if(us[i]!=13 && us[i]!=27 && us[i]!=8)
			{
				x[0]=us[i];
				setcolor(WHITE);
				outtextxy(333+j,202,x);
			}
			else if(us[i]==13)
			{
				break;
			}
			else if(us[i]==27)
			{
				return;
			}
			else if(us[i]==8)
			{       if(i>0)
				{
					check=1;
					setcolor(LIGHTGRAY);
					setfillstyle(1,LIGHTGRAY);
					bar(333+j-8,200,450,224);
					i--;
					j-=8;
				}
				if(i==0)
					goto reset_usrname_login;
			}

		}
		else
			break;

		if(check!=1)
		{	j+=8;
			i++;
		}


		}while(x[0]!=13);
		us[i]='\0'; //replace '\n' with '\0' at position i

	setfillstyle(1,15);
	setcolor(15);
	circle(458,252,4);
	floodfill(458,252,15);
	setcolor(BLUE);
	setfillstyle(1,BLUE);
	bar(453,207,463,217);
	ps[0]=getch();
	x[0]=ps[0];
	setcolor(LIGHTGRAY);
	setfillstyle(1,LIGHTGRAY);
	bar(300,240,450,264);
	setcolor(WHITE);
	settextstyle(2,0,6);
	outtextxy(333,242,"*");

	i=1; j=8;
	do{
		check=0;
		if(i<10)
		{	ps[i]=getch();
			if(ps[i]!=13 && ps[i]!=27 && ps[i]!=8)
			{
				x[0]=ps[i];
				setcolor(WHITE);
				outtextxy(333+j,242,"*");
			}
			else if(ps[i]==13)
			{
				break;
			}
			else if(ps[i]==27)
			{
				return;
			}
			else if(ps[i]==8)
			{       if(i>0)
				{
					check=1;
					setcolor(LIGHTGRAY);
					outtextxy(333+j-8,242,"*");
					i--;
					j-=8;
				}
				if(i==0)
				{	check=1;
					j=0;
				}
			}

		}
		else
			break;

		if(check!=1)
		{	j+=8;
			i++;
		}

		}while(x[0]!=13);
		ps[i]='\0';

		fstream pl;
		profile player;
		int found=0;
		pl.open("id.pg",ios::binary|ios::in);
		pl.seekg(0);
		pl.read((char*)&player,sizeof(player));
		while(!pl.eof())
		{       if(strcmp(us,player.rUS())==0 && strcmp(ps,player.rPS())==0)
				found=1;
			pl.read((char*)&player,sizeof(player));
		}
		if(found==0)
		{
			setcolor(LIGHTGRAY);
			setfillstyle(1,LIGHTGRAY);
			bar(236,325,409,340);
			setcolor(WHITE);
			settextstyle(2,0,6);
			outtextxy(240,322,"Profile Not Found!!");
			delay(800);
			login();
		}
		if(found==1){
			strcpy(myPlayer,us);
			mainmenu();
			}
}
//|||||||||||||||||||||| NEW USER |||||||||||||||||||||||
void newuser()
{
	cleardevice();
	//Layout Design
	int check=0;
	setcolor(DARKGRAY);
	setfillstyle(7,DARKGRAY);
	bar(0,0,640,480);
	setcolor(BLACK);
	setfillstyle(1,BLACK);
	bar(112,20,528,480);
	setcolor(RED);
	setfillstyle(1,RED);
	fillellipse(320,240,150,180);
	setcolor(BLACK);
	setfillstyle(1,BLACK);
	bar(120,380,520,480);
	setcolor(BROWN);
	setfillstyle(1,BROWN);
	bar(285,380,355,480);
	setcolor(WHITE);
	setfillstyle(1,WHITE);
	bar(315,390,325,470);
	setcolor(LIGHTGRAY);
	setfillstyle(1,LIGHTGRAY);
	bar(224,105,416,133);
	bar(300,200,450,223);	//Username Field
	bar(300,240,450,263);	//Password Field
	setcolor(WHITE);
	settextstyle(2,0,25);
	outtextxy(228,95,"NEW USER");
	settextstyle(2,0,7);
	outtextxy(185,200,"Username: ");
	outtextxy(185,240,"Password: ");
	//Functional Elements
	profile player, player_check;
	reset_usrname_newuser:
	setcolor(RED);
	setfillstyle(1,RED);
	bar(236,325,409,340);
	setcolor(WHITE);
	settextstyle(2,0,6);
	outtextxy(310,202,"10 characters");
	outtextxy(310,242,"10 characters");
	char us2[11], ps2[11], x2[2];
	setfillstyle(1,15);
	setcolor(15);
	circle(458,212,4);
	floodfill(458,212,15);
	us2[0]=getch();
	x2[0]=us2[0];
		if(x2[0]==27)//Esc
			choice();
		else if(x2[0]==13)//Enter
		{	setcolor(LIGHTGRAY);
			setfillstyle(1,LIGHTGRAY);
			bar(236,325,409,340);
			setcolor(WHITE);
			settextstyle(2,0,6);
			outtextxy(240,322,"No Input Provided!");
			delay(500);
			goto reset_usrname_newuser;
		}
		else if(x2[0]==8)//Backspace
			goto reset_usrname_newuser;
	x2[1]='\0';
	setcolor(LIGHTGRAY);
	setfillstyle(1,LIGHTGRAY);
	bar(300,200,450,224);
	setcolor(WHITE);
	settextstyle(2,0,6);
	outtextxy(333,202,x2);
	int i=1, j=8;
	do{
		check=0;
		if(i<10)
		{	us2[i]=getch();
			if(us2[i]!=13 && us2[i]!=27 && us2[i]!=8)
			{
				x2[0]=us2[i];
				setcolor(WHITE);
				outtextxy(333+j,202,x2);
			}
			else if(us2[i]==13)
			{
				break;
			}
			else if(us2[i]==27)
			{
				return;
			}
			else if(us2[i]==8)
			{       if(i>0)
				{
					check=1;
					setcolor(LIGHTGRAY);
					setfillstyle(1,LIGHTGRAY);
					bar(333+j-8,200,450,224);
					i--;
					j-=8;
				}
				if(i==0)
					goto reset_usrname_newuser;
			}

		}
		else
			break;

		if(check!=1)
		{	j+=8;
			i++;
		}


		}while(x2[0]!=13);
		us2[i]='\0'; //replace '\n' with '\0' at position i
		player.inUS(us2);

	setfillstyle(1,15);
	setcolor(15);
	circle(458,252,4);
	floodfill(458,252,15);
	setcolor(RED);
	setfillstyle(1,RED);
	bar(453,207,463,217);
	ps2[0]=getch();
	x2[0]=ps2[0];
	setcolor(LIGHTGRAY);
	setfillstyle(1,LIGHTGRAY);
	bar(300,240,450,264);
	setcolor(WHITE);
	settextstyle(2,0,6);
	outtextxy(333,242,"*");

	i=1; j=8;
	do{
		check=0;
		if(i<10)
		{	ps2[i]=getch();
			if(ps2[i]!=13 && ps2[i]!=27 && ps2[i]!=8)
			{
				x2[0]=ps2[i];
				setcolor(WHITE);
				outtextxy(333+j,242,"*");
			}
			else if(ps2[i]==13)
			{
				break;
			}
			else if(ps2[i]==27)
			{
				return;
			}
			else if(ps2[i]==8)
			{       if(i>0)
				{
					check=1;
					setcolor(LIGHTGRAY);
					outtextxy(333+j-8,242,"*");
					i--;
					j-=8;
				}
				if(i==0)
				{	check=1;
					j=0;
				}
			}

		}
		else
			break;

		if(check!=1)
		{	j+=8;
			i++;
		}

		}while(x2[0]!=13);
		ps2[i]='\0';
		player.inPS(ps2);
		strcpy(myPlayer,us2);
		fstream pl;

		//Opening file to check existing username..
		pl.open("id.pg",ios::binary|ios::in);
		pl.read((char*)&player_check,sizeof(player_check));
		if(pl.fail()==0){
		/*Existing username would not be checked if
		  file does not exist. */
		while(!pl.eof())
		{	if(strcmp(myPlayer,player_check.rUS())==0)
			{	setcolor(LIGHTGRAY);
				setfillstyle(1,LIGHTGRAY);
				bar(241,325,400,340);
				setcolor(WHITE);
				settextstyle(2,0,6);
				outtextxy(245,322,"Username Exists!");
				delay(500);
				pl.close();
				newuser();
			}
			pl.read((char*)&player_check,sizeof(player_check));
		}}
		pl.close();

		//Opening file to save the new profile..
		pl.open("id.pg",ios::binary|ios::app);
		pl.write((char*)&player,sizeof(player));
		pl.close();

		setcolor(LIGHTGRAY);
		setfillstyle(1,LIGHTGRAY);
		bar(243,325,395,340);
		setcolor(WHITE);
		settextstyle(2,0,6);
		outtextxy(247,322,"Profile Created!!");
		delay(800);
		mainmenu();

}
//|||||||||||||||||||||| CHOICE |||||||||||||||||||||||
void choice()
{       cleardevice();
	setbkcolor(BLACK);
	setcolor(LIGHTGRAY);
	setfillstyle(1,LIGHTGRAY);
	bar(120,0,520,480);
	setcolor(DARKGRAY);
	setfillstyle(2,DARKGRAY);
	bar(0,0,100,480);
	bar(540,0,640,480);
	setfillstyle(1,DARKGRAY);
	bar3d(160,100,480,380,2,1);
	setcolor(WHITE);
	settextstyle(2,0,45);
	outtextxy(190,110,"Option Menu:");
	settextstyle(2,0,7);
	outtextxy(270,213,"LOGIN");
	outtextxy(270,250,"SIGN-UP");
	setfillstyle(1,15);
	setcolor(15);
	circle(260,225,5);
	floodfill(260,225,15);

	int select=0, move=0;

	do{
		move=getch();
		if(move==72)
		{	select=0;
			setcolor(0);
			setfillstyle(1,8);
			bar(255,260,265,270);
			setfillstyle(1,15);
			setcolor(15);
			circle(260,225,5);
			floodfill(260,225,15);
		}
		else if(move==80)
		{	select=1;
			setcolor(0);
			setfillstyle(1,8);
			bar(255,220,265,230);
			setfillstyle(1,15);
			setcolor(15);
			circle(260,265,5);
			floodfill(260,265,15);
		}
		else if(move==27)
			exit(0);
	}while(move!=13);

	if(select==0)
	login();
	else if(select==1)
	newuser();
}
//||||||||||||||||||||||||| PROFILE |||||||||||||||||||||||||
void profile_set()
{	cleardevice();
	setbkcolor(BLACK);
	setcolor(15);
	settextstyle(8,0,5);
		outtextxy(135,30,"Profile Settings");
	settextstyle(2,0,7);
		outtextxy(230,193,"Update Details");
		outtextxy(230,230,"Delete Profile");
	setfillstyle(1,15);
		setcolor(15);
		circle(220,205,5);
		floodfill(220,205,15);

	int move=0, select=0;

	do{
		move=getch();
		if(move==72)
		{	select=0;
			setcolor(0);
			setfillstyle(1,0);
			bar(215,240,225,250);
			setfillstyle(1,15);
			setcolor(15);
			circle(220,205,5);
			floodfill(220,205,15);
		}
		else if(move==80)
		{	select=1;
			setcolor(0);
			setfillstyle(1,0);
			bar(215,200,225,210);
			setfillstyle(1,15);
			setcolor(15);
			circle(220,245,5);
			floodfill(220,245,15);
		}
		else if(move==27)
			mainmenu();
	}while(move!=13);

	if(select==0)
	{
		clrscr();
		cleardevice();
		setbkcolor(BROWN);
		cout<<"                             ###################\n";
		cout<<"                             # PONG-2D Console #\n";
		cout<<"                             ###################\n\n";
		cout<<"Welcome: "<<myPlayer;
		fstream master,temp;
		profile player;

		master.open("id.pg",ios::in|ios::binary);
		temp.open("temp22.pg",ios::out|ios::binary);
		master.read((char*)&player,sizeof(player));
		while(!master.eof())
		{	if(strcmp(myPlayer,player.rUS())==0)
			{
				player.modUS();
				player.modPS();
				strcpy(myPlayer,player.rUS());
			}
			temp.write((char*)&player,sizeof(player));
			master.read((char*)&player,sizeof(player));
		}
		cout<<"\n=> Data Saved\n";
		master.close();
		temp.close();
		remove("id.pg");
		rename("temp22.pg","id.pg");

		getch();
		mainmenu();
	}
	else if(select==1)
	{
		clrscr();
		cleardevice();
		setbkcolor(BROWN);
		cout<<"                             ###################\n";
		cout<<"                             # PONG-2D Console #\n";
		cout<<"                             ###################\n\n";
		cout<<"Welcome: "<<myPlayer<<endl<<endl;
		fstream master,temp;
		profile player;
		char user_choice;
		master.open("id.pg",ios::in|ios::binary);
		temp.open("temp22.pg",ios::out|ios::binary);
	do{
		cout<<"\n\n=> Do you really want to delete your profile {Y/N} ?";
		cin>>user_choice;
	if(user_choice=='y'||user_choice=='Y')
	{	master.read((char*)&player,sizeof(player));
		while(!master.eof())
		{	if(strcmp(myPlayer,player.rUS())!=0)
			{	temp.write((char*)&player,sizeof(player));
			}

			master.read((char*)&player,sizeof(player));
		}
		cout<<"\n=> Profile Deleted!!\n";
		cout<<"=> Press any key to exit...";
		master.close();
		temp.close();
		remove("id.pg");
		rename("temp22.pg","id.pg");
		getch();
		exit(0);
	}
	else if(user_choice=='n'||user_choice=='N')
	{	cout<<"\n=> Operaton aborted by user..";
		cout<<"\n=> Press any key to continue...";
		getch();
		mainmenu();
	}
	else
		cout<<"\n=> Invalid Input!!";
	}while(user_choice!='y'||user_choice!='Y'||user_choice!='n'||user_choice!='N');
	}
}
