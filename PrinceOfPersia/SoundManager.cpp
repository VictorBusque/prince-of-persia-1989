#include "SoundManager.h"
#include <iostream>
using namespace std;
#pragma comment (lib, "lib/fmod_vc.lib") 

/******** CLASS VARIABLE DECLARATIONS ********/

bool Sound::on = true; //is sound on?
bool Sound::possible = true; //is it possible to play sound?
char * Sound::currentSound; //currently played sound
//FMOD-specific stuff
FMOD_RESULT Sound::result;
FMOD::System * Sound::fmodsystem;
FMOD::Sound* Sound::sound;
FMOD::Channel * Sound::channel;
FMOD::ChannelGroup * Sound::channelgroup;

/******** METHODS' IMPLEMENTATIONS ********/

//initialises sound
void Sound::initialise(void) {
	//create the sound system. If fails, sound is set to impossible
	result = FMOD::System_Create(&fmodsystem);
	if (result != FMOD_OK) possible = false;
	//if initialise the sound system. If fails, sound is set to impossible
	if (possible) result = fmodsystem->init(2, FMOD_INIT_NORMAL, 0);
	if (result != FMOD_OK) possible = false;
	//sets initial sound volume (mute)
	if (possible) channel->setVolume(1.0f);
}

//sets the actual playing sound's volume
void Sound::setVolume(float v) {
	if (possible && on && v >= 0.0f && v <= 1.0f) {
		channel->setVolume(v);
	}
}

//loads a soundfile
void Sound::load(const char * filename) {
	possible = true;
	currentSound = (char *)filename;
	if (possible && on) {
		result = sound->release();
		result = fmodsystem->createStream(currentSound, 0, 0, &sound);
		if (result != FMOD_OK) possible = false;
	}
}

//frees the sound object
void Sound::unload(void) {
	if (possible) {
		result = sound->release();
	}
}

//plays a sound (no argument to leave pause as dafault)
void Sound::play(bool pause) {
	if (possible && on) {
		result = fmodsystem->playSound(sound, NULL,  pause, &channel);
		channel->setMode(FMOD_LOOP_OFF);
	}
}

//toggles sound on and off
void Sound::toggleSound(void) {
	on = !on;
	if (on == true) { load(currentSound); play(); }
	if (on == false) { unload(); }
}

//pause or unpause the sound
void Sound::setPause(bool pause) {
	channel->setPaused(pause);
}

//turn sound on or off
void Sound::setSound(bool s) {
	on = s;
}


//tells whether the sound is on or off
bool Sound::getSound(void) {
	return on;
}
