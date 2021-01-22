#include "SoundFX.h"
#include<iostream>
#pragma comment(lib, "winmm.lib")
using namespace std;

SoundFX::SoundFX()
{
	Init();
}


SoundFX::~SoundFX()
{
}

void SoundFX::Init() {
	Sound::initialise();
}

void SoundFX::PlayThemeSong() {
	mciSendString((LPCWSTR)L"open sounds\\theme.mp3 type mpegvideo alias themesong", NULL, 0, NULL);
	mciSendString((LPCWSTR)L"setaudio themesong volume to 500", NULL, 0, NULL);
	mciSendString((LPCWSTR)L"play themesong repeat", NULL, 0, NULL);
}

void SoundFX::PlayWinSong() {
	mciSendString((LPCWSTR)L"stop themesong", NULL, 0, NULL);
	mciSendString((LPCWSTR)L"open sounds\\won_light.mp3 type mpegvideo alias wonsong", NULL, 0, NULL);
	mciSendString((LPCWSTR)L"play wonsong repeat", NULL, 0, NULL);
}

void SoundFX::StopThemeSong() {
	mciSendString((LPCWSTR)L"stop themesong", NULL, 0, NULL);
}

void SoundFX::StopWinSong() {
	mciSendString((LPCWSTR)L"stop wonsong", NULL, 0, NULL);
}

void SoundFX::PlayFX(FX FXid) {
	switch (FXid) {
	case STEP:
		Sound::load("sounds\\step.wav");
		break;
	case INTRO_THEME:
		Sound::load("sounds\\intro_theme.wav");
		break;
	case START_GAME:
		Sound::load("sounds\\meet_Jaffar.wav");
		break;
	case DRINK_POTION:
		Sound::load("sounds\\life_potion.wav");
		break;
	case BUMP:
		Sound::load("sounds\\bump.wav");
		break;
	case BUTTON_OPEN:
		Sound::load("sounds\\button-open.wav");
		break;
	case FIGHT_DEATH:
		Sound::load("sounds\\fight_death.wav");
		break;
	case GATE_OPEN:
		Sound::load("sounds\\gate-open.wav");
		break;
	case GATE_CLOSE:
		Sound::load("sounds\\gate-close.wav");
		break;
	case GUARD_HIT:
		Sound::load("sounds\\guard-hit.wav");
		break;
	case HARM:
		Sound::load("sounds\\harm.wav");
		break;
	case LAND_FATAL:
		Sound::load("sounds\\land-fatal.wav");
		break;
	case LAND_HARM:
		Sound::load("sounds\\land-harm.wav");
		break;
	case LAND_SOFT:
		Sound::load("sounds\\land-soft.wav");
		break;
	case LOOSE_CRASH:
		Sound::load("sounds\\loose-crash.wav");
		break;
	case LOOSE_FLOOR1:
		Sound::load("sounds\\loose-floor-1.wav");
		break;
	case LOOSE_FLOOR2:
		Sound::load("sounds\\loose-floor-2.wav");
		break;
	case LOOSE_FLOOR3:
		Sound::load("sounds\\loose-floor-3.wav");
		break;
	case SLICED:
		Sound::load("sounds\\sliced.wav");
		break;
	case SLICER:
		Sound::load("sounds\\slicer.wav");
		break;
	case SPIKED:
		Sound::load("sounds\\spiked.wav");
		break;
	case SPIKES:
		Sound::load("sounds\\spikes.wav");
		break;
	case SWORD_ATTACK:
		Sound::load("sounds\\sword-attack.wav");
		break;
	case SWORD_DEFENSE:
		Sound::load("sounds\\sword-defense.wav");
		break;
	case SWORD_DRAWN:
		Sound::load("sounds\\sword-drawn.wav");
		break;
	case TAKE_SWORD_KILL_ENEMY:
		Sound::load("sounds\\take-sword-kill-enemy.wav");
		break;
	}
	Sound::play();
}

/*
STEP, INTRO_THEME, START_GAME, BUMP, BUTTON_OPEN,
GATE_OPEN, GATE_CLOSE, GUARD_HIT, HARM, LAND_FATAL,
LAND_HARM, LAND_SOFT, LOOSE_CRASH, LOOSE_FLOOR1,
LOOSE_FLOOR2, LOOSE_FLOOR3, SLICED, SLICER, DRINK_POTION,
SPIKED, SPIKES, SWORD_ATTACK, SWORD_DEFENSE, SWORD_DRAWN
*/

