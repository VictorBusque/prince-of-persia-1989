#include "inc/fmod.hpp" //FMOD Ex
#pragma comment(lib, "fmodL_vc.lib")
#pragma comment(lib, "fmod_vc.lib")

/******** CLASS DEFINITION ********/

class Sound {
private:
	static bool on; //is sound on?
	static bool possible; //is it possible to play sound?
	static char * currentSound; //currently played sound
	//FMOD-specific stuff
	static FMOD_RESULT result;
	static FMOD::System * fmodsystem;
	static FMOD::Sound * sound;
	static FMOD::Channel * channel;
	static FMOD::ChannelGroup *channelgroup;

public:
	static void initialise(void); //initialises sound

	//sound control
	static void setVolume(float v); //sets the actual playing sound's volume
	static void load(const char * filename); //loads a soundfile
	static void unload(void); //frees the sound object
	static void play(bool pause = false); //plays a sound (may be started paused; no argument for unpaused)

	//getters
	static bool getSound(void); //checks whether the sound is on

	//setters
	static void setPause(bool pause); //pause or unpause the sound
	static void setSound(bool sound); //set the sound on or off

	//toggles
	static void toggleSound(void); //toggles sound on and off
	//static void togglePause(void); //toggle pause on/off
};

