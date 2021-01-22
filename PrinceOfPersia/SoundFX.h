#include <windows.h>
#include <mmsystem.h>
#include <mciapi.h>
#include<string>
#include"SoundManager.h"
#pragma once

enum FX {
	STEP, INTRO_THEME, START_GAME, BUMP, BUTTON_OPEN,
	GATE_OPEN, GATE_CLOSE, GUARD_HIT, HARM, LAND_FATAL,
	LAND_HARM, LAND_SOFT, LOOSE_CRASH, LOOSE_FLOOR1,
	LOOSE_FLOOR2, LOOSE_FLOOR3, SLICED, SLICER, DRINK_POTION,
	SPIKED, SPIKES, SWORD_ATTACK, SWORD_DEFENSE, SWORD_DRAWN,
	TAKE_SWORD_KILL_ENEMY, FIGHT_DEATH
};

class SoundFX
{
public:


	SoundFX();
	~SoundFX();
	void Init();

	void PlayThemeSong();
	void PlayWinSong();
	void PlayFX(FX FXid);
	void SetFX(FX FXid);
	void StopThemeSong();
	void StopWinSong();
};
