#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <Windows.h>
#include <math.h>
#include "audio.h"
#define score_1 18
#define score_2 20
#define score_3	16
#define width 80 //initializing size of game field
#define height 30
int count = 0;
int score = 0;
int start = 0;
int current_level = 3;
double start_velocity = 0.2;
int lives = 3;
char map[height][width];
char arr[height][width+1]; //game field
int convert(char* str) {
	int flag = 1;
	int res = 0;
	for (int i = 0; str[i] != '\0'; ++i) {
		if (str[i] < '0' || str[i]>'9') {
			return -1;
		}
		if (str[i] == '-') {
			flag = -1;
		}
		else res = res * 10 + str[i] - '0';
	}
	return res * flag;
}
//----BAT-------------
typedef struct Bat {
	int x, y; //coordinates
	int w;
}Bat;
Bat bat;
void bat_init() {
	bat.w = 7;
	bat.y = height - 1;
	bat.x = (width - bat.w) / 2;
}
void bat_render() {
	for (int i = bat.x; i < bat.w + bat.x; i++) {
		arr[bat.y][i] = '&';
	}
}
void bat_move(int x) {
	bat.x = x;
	if (bat.x < 1) {
		bat.x = 1;
	}
	if (bat.x + bat.w >= width) {
		bat.x = width - bat.w - 1;
	}
}
//-----------------------------



//----------BALL------------
typedef struct Ball {
	double x, y;
	int x_int, y_int;
	double angle;
	double velocity;
}Ball;
Ball ball;

void ball_render() {
	arr[ball.y_int][ball.x_int] = '*';
}
void ball_move(double x, double y) {
	ball.x = x;
	ball.y = y;
	ball.x_int = (int)round(ball.x);
	ball.y_int = (int)round(ball.y);
}
void ball_init() {
	ball_move(bat.x+bat.w/2, bat.y - 1);
	ball.angle = 1;
	ball.velocity = start_velocity;
	
}
void ball_movement() {
	if (ball.angle < 0) {
		ball.angle += 2 * M_PI;
	}
	if (ball.angle > 2*M_PI) {
		ball.angle -= 2 * M_PI;
	}
	Ball prev=ball;
	ball_move(ball.x + cos(ball.angle) * ball.velocity, ball.y + sin(ball.angle)*ball.velocity);
	if ((arr[ball.y_int][ball.x_int] == '#') || (arr[ball.y_int][ball.x_int] == '&') 
		|| (arr[ball.y_int][ball.x_int] == (char)178)) {
		if (arr[ball.y_int][ball.x_int] == '#') {
			count += 1;
			aud_wall_hit();
		}
		if (arr[ball.y_int][ball.x_int] == (char)178) {
			score++;
			aud_block();
			int block_num = (ball.x_int - 1) / 3;
			int cell_1 = block_num * 3 + 1;
			for (int i = 0; i < 3; i++) {
				if (map[ball.y_int][i + cell_1] == (char)178) {
					map[ball.y_int][i + cell_1] = ' ';
				}
			}
		}
		if (arr[ball.y_int][ball.x_int] == '&') {
			aud_bat_hit();
			double pos = (ball.x - bat.x) / bat.w;
			pos *= 2;
			pos -= 1;
			pos *= M_PI_2*0.8;
			
			prev.angle = pos-M_PI_2;
		}
		else if (prev.x_int != ball.x_int && prev.y_int != ball.y_int) {
			if (arr[prev.y_int][ball.x_int] == arr[ball.y_int][prev.x_int]) {
				prev.angle = prev.angle + M_PI;
			}
			else {
				if (arr[prev.y_int][ball.x_int] == '#') {
					prev.angle = -prev.angle + M_PI + 2*M_PI;
				}
				else {
					prev.angle = -prev.angle + 2 * M_PI;
				}
			}
		}
		else {
			if (prev.y_int == ball.y_int) {
				prev.angle = -prev.angle + M_PI + 2 * M_PI;
			}
			else {
				prev.angle = -prev.angle + 2 * M_PI;
			}
		}
		
		ball = prev;
		if (count % 25 == 0 && count != 0) {
			ball.velocity += 0.05;
		}
		ball_movement();
	}
}
// --------------------
//function that inits the field
void init() {
	if (current_level == 1) {
		for (int i = 31; i < 58; i++) {
			map[7][i] = 178;
			map[8][i] = 178;
		}
	}
	
	if (current_level == 2) {
		for (int i = 16; i < 61; i++) {
			map[5][i] = '#';
		}
		for (int i = 16; i < 61; i+=6) {
			for (int j = i; j < i + 3; j++) {
				map[4][j] = (char)178;
			}
		}
		for (int i = 19; i < 61; i += 6) {
			for (int j = i; j < i + 3; j++) {
				map[3][j] = (char)178;
			}
		}
		for (int i = 31; i < 46; i++) {
			map[20][i] = '#';
			map[19][i] = (char)178;
		}
		
	}
	if (current_level == 3) {
		for (int i = 7; i < 76; i += 9) {
			for (int j = i; j < i + 3; j++) {
				map[15][j] = '#';
				map[8][j] = '#';
				map[14][j] = (char)178;
				map[7][j] = (char)178;
			}
		}
	}
}
void level_init() {
	memset(map, ' ', sizeof(map));
	init();
	for (int i = 0; i < width; i++) {
		map[0][i] = '#';
	}
	for (int i = 0; i < height; i++) {
		map[i][0] = '#';
		map[i][width-1] = '#';
	}
}
void level_render() {
	memset(arr, 0, sizeof(arr));
	for (int i = 0; i < height; i++) {
		memcpy(arr[i], map[i], sizeof(**map) * width);
	}
}

void render_field() {
	
	for (int i = 0; i < height; i++) {
		printf("%s", arr[i]);
		if (i == 7) {
			printf("    SCORE : %d", score);
		}
		if (i == 9) {
			printf("    LIVES : %d", lives);
		}
		if (i != height - 1) {
			printf("\n");
		}
	}
}
//-----------------------------
void set_cursor(int x,int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void hide_cursor() {
	HANDLE  handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);
}

void intro() {
	audio_main_menu();
	printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\t\tARKANOID\n\n\n");
	Sleep(1000);
	printf("\t\t\t\t\t    Press F to start");
	do {

	} while (GetKeyState('F') >= 0);
	aud_yes();
	system("cls");
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\tPlease, choose level from 1 to 3: ");
	int lvl;
	char lvl_string[200];
	int flag = 0;
	while (flag == 0) {
		scanf("%s", lvl_string);
		lvl = convert(lvl_string);
		switch (lvl) {
		case 1:
			current_level = 1;
			flag = 1;
			aud_yes();
			break;
		case 2:
			current_level = 2;
			flag = 1;
			aud_yes();
			break;
		case 3:
			current_level = 3;
			flag = 1;
			aud_yes();
			break;
		default:
			aud_no();
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\tTry again: ");
			break;
		}
	}
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\tPlease, choose difficulty from 1 to 3: ");
	flag = 0;
	while (flag == 0) {
		scanf("%s", lvl_string);
		lvl = convert(lvl_string);
		switch (lvl) {
		case 1:
			start_velocity = 0.2;
			flag = 1;
			aud_yes();
			break;
		case 2:
			start_velocity = 0.3;
			flag = 1;
			aud_yes();
			break;
		case 3:
			start_velocity = 0.4;
			flag = 1;
			aud_yes();
			break;
		default:
			aud_no();
			system("cls");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\tTry again: ");
			break;
		}
	}
	system("cls");
	audio_stop();
}
void change_level() {
	
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\t\tLEVEL %d\n\n\n", current_level);
	Sleep(100);
	audio_change_level(current_level);
	printf("\t\t\t\t\t       Loading...");
	Sleep(2000);
	system("cls");
	audio_stop();
}
int main() {
	audio_init();
	intro();
	change_level();
	audio_level(current_level);
	hide_cursor();
	bat_init();
	ball_init();
	level_init();
	do {
		set_cursor(0, 0);
		
		if (start == 1) {
			ball_movement();
		}
		if (lives == 0) {
			system("cls");
			break;
		}
		if (ball.y_int > height-2) {
			start = 0;
			lives -= 1;
			if (lives != 0) {
				aud_dec_live();
			}
			ball.velocity = start_velocity;
			ball.angle = -1;
			//count = 0;
			//break;
		}
		if ((score == score_1 && current_level==1)||
			(score == score_2 && current_level == 2)||
			(score == score_3 && current_level == 3)) {
			if (score == score_3 && current_level == 3) {
				current_level++;
				audio_stop();
				system("cls");
				break;
			}
			current_level++;
			score = 0;
			ball.velocity = start_velocity;
			ball.angle = -1;
			level_init();
			start = 0;
			change_level();
			audio_level(current_level);
		}
		level_render();
		bat_render();
		ball_render();
		
		render_field();

		if (GetKeyState(VK_LEFT)<0) {
			bat_move(bat.x - 1);
		}
		if (GetKeyState(VK_RIGHT) < 0) {
			bat_move(bat.x + 1);
		}
		if (GetKeyState(VK_UP) < 0) {
			start = 1;
		}
		if (start == 0) {
			ball_move(bat.x + bat.w / 2, bat.y - 1);
		}
		Sleep(10);
	} while (GetKeyState(VK_ESCAPE)>=0);
	
	if (lives == 0) {
		audio_stop();
		printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\t\tGAME_OVER\n\n\n");
		Sleep(1000);
		printf("\t\t\t\t\t   Press ESC to exit game\n\n\n");
		gameover();
		
		//system("cls");
	}
	else {
		
		printf("\n\n\n\n\n\n\n\n\n\n\n\n \t\t\t\t\t\tCONGRATULATIONS!\n\n\n");
		Sleep(2000);
		
		
		
		win();
		Sleep(1000);
	}
	if (current_level < 4) {
		close();
		return 0;
	}
	close();
	return 0;
}