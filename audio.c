#include <SDL.h>
#include <SDL_mixer.h>
#include "audio.h";
#include <Windows.h>
Mix_Music* Music = NULL;
Mix_Chunk* but_yes = NULL;
Mix_Chunk* but_no = NULL;
Mix_Chunk* wall_hit = NULL;
Mix_Chunk* bat_hit = NULL;
Mix_Chunk* block = NULL;
Mix_Chunk* live = NULL;
Mix_Chunk* lvl = NULL;
Mix_Chunk* one = NULL;
Mix_Chunk* two = NULL;
Mix_Chunk* three = NULL;
Mix_Chunk* you = NULL;
Mix_Chunk* die = NULL;
void audio_init() {
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	but_yes = Mix_LoadWAV("source/button_yes.wav");
	but_no = Mix_LoadWAV("source/button_no.wav");
	live = Mix_LoadWAV("source/lives.wav");
	block = Mix_LoadWAV("source/block.wav");
	wall_hit = Mix_LoadWAV("source/wall_hit.wav");
	bat_hit = Mix_LoadWAV("source/bat_hit.wav");
	lvl = Mix_LoadWAV("source/level.wav");
	one = Mix_LoadWAV("source/one.wav");
	two = Mix_LoadWAV("source/two.wav");
	three = Mix_LoadWAV("source/three.wav");
	you = Mix_LoadWAV("source/you.wav");
	die = Mix_LoadWAV("source/die.wav");
}
void aud_yes() {
	Mix_PlayChannel(-1,but_yes,0);
}
void aud_no() {
	Mix_PlayChannel(-1, but_no, 0);
}
void aud_dec_live() {
	Mix_PlayChannel(-1, live, 0);
}
void aud_block() {
	Mix_PlayChannel(-1, block, 0);
}
void aud_wall_hit() {
	Mix_PlayChannel(-1, wall_hit, 0);
}
void aud_bat_hit() {
	Mix_PlayChannel(-1, bat_hit, 0);
}
void win() {
	Music = NULL;
	Music = Mix_LoadMUS("source/win.mp3");
	Mix_PlayMusic(Music, 1);
	printf("\t\t\t\t\t\t YOU WON!\n\n\n");
	Sleep(3000);

	printf("\t\t\t\t\t   Press ESC to exit game\n\n\n");
	
	do {

	} while (GetKeyState(VK_ESCAPE)>=0);
}
void gameover() {
	Music = NULL;
	Music = Mix_LoadMUS("source/gameover.mp3");
	Mix_PlayMusic(Music, 1);
	Mix_PlayChannel(-1, you, 0);
	Sleep(1000);
	
	Mix_PlayChannel(-1, die, 0);
	Sleep(1500);
	do {

	} while (GetKeyState(VK_ESCAPE) >= 0);
}
void aud_level(int level) {
	Mix_PlayChannel(-1, lvl, 0);
	Sleep(1000);
	if (level==1) Mix_PlayChannel(-1, one, 0);
	if (level == 2) Mix_PlayChannel(-1, two, 0);
	if (level == 3) Mix_PlayChannel(-1, three, 0);
}
void audio_main_menu() {
	Music = Mix_LoadMUS("source/main_menu.mp3");
	Mix_PlayMusic(Music, -1);
}

void audio_stop() {
	Mix_HaltMusic();
	Music = NULL;
}
void audio_level(int level) {
	//audio_stop();
	if (level == 1) {
		Music = Mix_LoadMUS("source/level1.mp3");
		Mix_PlayMusic(Music, -1);
	}
	if (level == 2) {
		Music = Mix_LoadMUS("source/level2.mp3");

		Mix_PlayMusic(Music, -1);
	}
	if (level == 3) {
		Music = Mix_LoadMUS("source/level3.mp3");

		Mix_PlayMusic(Music, -1);
	}
}
void audio_change_level(int level) {
	
	if (level == 1) {
		Music = Mix_LoadMUS("source/change_level_1.wav");
		Mix_PlayMusic(Music, 1);
	}
	if (level == 2) {
		Music = Mix_LoadMUS("source/change_level_2.wav");
		
		Mix_PlayMusic(Music, 1);
	}
	if (level == 3) {
		Music = Mix_LoadMUS("source/change_level_3.wav");
		
		Mix_PlayMusic(Music, 1);
	}
	Sleep(4000);
	aud_level(level);
	//audio_level(level);
}

void close() {
	Mix_Quit();
	SDL_Quit();
}
