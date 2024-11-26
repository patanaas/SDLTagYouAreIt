#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandardIncludes.h"
#include "AudioController.h"

// Week 9 Code for Input Core
class Renderer;
class TTFont;
class InputController;
// Code for Week 10 Audio Core
class AudioController;
class SoundEffect;
class Song;
class WavDraw;

class GameController : public Singleton<GameController>
{
public:
	// Constructors and Destructors
	GameController();
	virtual ~GameController();

	// Methods
	void RunGame();
	// Week 9 Code for Input Core
	//void Initialize();
	void ShutDown();
	void HandleInput(SDL_Event _event);

private:
	// Members
	SDL_Event m_sdlEvent;

	// Week 9 Code for Input Core
	Renderer* m_renderer;
	TTFont* m_fArial20;
	bool m_quit;
	InputController* m_input;
	//string m_text;
	//string m_smPos;
	//Point m_mPos;
	//string m_ctInfo;
	// Code for week 10 Audio Core
	AudioController* m_audio;
	SoundEffect* m_effects[MaxEffectChannels];
	Song* m_song;
	WavDraw* m_wavDraw;
	float m_zoomY;
};

#endif // !GAME_CONTROLLER_H

