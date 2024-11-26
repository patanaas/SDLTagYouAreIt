#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandardIncludes.h"
#include "Singleton.h"

class Renderer;
class Level;
class InputController;
class Keyboard;

class GameController : public Singleton<GameController> {
public:
    // Constructors and Destructors
    GameController();
    virtual ~GameController();

    // Methods
    void RunGame();
    void Initialize();
    void ShutDown();
    void HandleInput(SDL_Event _event);

    void SetQuitFlag(bool quit) { m_quit = quit; }

private:
    // Members
    SDL_Event m_sdlEvent;
    bool m_quit;

    Level* m_level; // Add this member for managing the game level
};

#endif // GAME_CONTROLLER_H
