#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm> // For std::min and std::max
#include "Level.h"
#include <SDL_mixer.h>

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
const int FRAME_RATE = 60;              // Reduced to 60 FPS for smoother animation
const int FRAME_DELAY = 1000 / FRAME_RATE;
const int NUM_NPCS = 10;

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return 1;
    }
    Mix_AllocateChannels(16);  // Allocating 16 channels for simultaneous sound effects

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create SDL window
    SDL_Window* window = SDL_CreateWindow(
        "SDLTagYouAreIt",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create SDL renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("../Assets/Fonts/arial.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize level
    Level level(renderer);
    level.init(NUM_NPCS);

    bool running = true;
    SDL_Event event;

    // Debugging variables
    int frameCount = 0;
    auto lastSecond = std::chrono::high_resolution_clock::now();

    // Game loop
    while (running) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_i: // Increase NPC speed
                    for (auto npc : level.getNPCs()) {
                        npc->setSpeed(std::min(npc->getSpeed() + 10, 60));
                    }
                    break;
                case SDLK_d: // Decrease NPC speed
                    for (auto npc : level.getNPCs()) {
                        npc->setSpeed(std::max(npc->getSpeed() - 10, 0));
                    }
                    break;
                case SDLK_s: // Save level
                    level.save("level.bin");
                    break;
                case SDLK_l: // Load level
                    level.load("level.bin");
                    break;
                case SDLK_ESCAPE: // Quit game
                    running = false;
                    break;
                }
            }

            // Pass the event to the player for movement controls
            level.getPlayer()->handleEvent(event);
        }

        // Update the game state
        float deltaTime = FRAME_DELAY / 1000.0f;
        level.update(deltaTime);

        // Check if the game is over
        if (level.isGameOver()) {
            running = false;
        }

        // Render the game
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        level.render(renderer, font);
        SDL_RenderPresent(renderer);

        // Frame delay logic
        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float, std::milli> frameDuration = frameEnd - frameStart;
        if (frameDuration.count() < FRAME_DELAY) {
            SDL_Delay(static_cast<int>(FRAME_DELAY - frameDuration.count()));
        }

        // Debug: Track frames per second (FPS)
        frameCount++;
        auto currentSecond = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsedSecond = currentSecond - lastSecond;
        if (elapsedSecond.count() >= 1.0f) {
            std::cout << "FPS: " << frameCount << std::endl;
            frameCount = 0;
            lastSecond = currentSecond;
        }
    }

    // Cleanup
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    Mix_CloseAudio();
    //Mix_FreeChunk(tagSound);

    return 0;
}
