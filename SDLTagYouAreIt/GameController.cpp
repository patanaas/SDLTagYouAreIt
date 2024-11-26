#include "GameController.h"
#include "Renderer.h"
#include "Level.h"
#include "InputController.h"
#include "Keyboard.h"
#include <sstream>
#include <iostream> // Ensure this is included for debug logs

GameController::GameController()
    : m_level(nullptr), m_quit(false) {}

GameController::~GameController() {
    ShutDown();
}

void GameController::Initialize() {
    std::cout << "Initializing game controller...\n";

    Renderer::Instance().Initialize();
    InputController::Instance(); // Ensure InputController is initialized
    m_level = new Level();
    if (m_level) {
        m_level->Initialize();
        std::cout << "Level initialized successfully.\n";
    }
    else {
        std::cerr << "Failed to initialize level.\n";
    }
}

void GameController::ShutDown() {
    std::cout << "Shutting down game controller...\n";

    if (m_level) {
        delete m_level;
        m_level = nullptr;
    }

    Renderer::Instance().Shutdown();
}

void GameController::HandleInput(SDL_Event _event) {
    Keyboard* keyboard = InputController::Instance().KB();

    if (_event.type == SDL_QUIT || keyboard->KeyUp(_event, SDLK_ESCAPE)) {
        std::cout << "Exit command received.\n";
        m_quit = true;
    }

    if (keyboard->KeyUp(_event, SDLK_i)) {
        float newSpeed = m_level->GetNPCSpeed() + 10.0f;
        if (newSpeed <= 60.0f) {
            m_level->SetNPCSpeed(newSpeed);
            std::cout << "NPC speed increased to " << newSpeed << ".\n";
        }
    }
    else if (keyboard->KeyUp(_event, SDLK_d)) {
        float newSpeed = m_level->GetNPCSpeed() - 10.0f;
        if (newSpeed >= 0.0f) {
            m_level->SetNPCSpeed(newSpeed);
            std::cout << "NPC speed decreased to " << newSpeed << ".\n";
        }
    }
    else if (keyboard->KeyUp(_event, SDLK_s)) {
        // Save level
        std::ofstream saveFile("level.bin", std::ios::binary);
        if (saveFile.is_open()) {
            m_level->Serialize(saveFile);
            saveFile.close();
            std::cout << "Level saved to level.bin.\n";
        }
        else {
            std::cerr << "Failed to open save file.\n";
        }
    }
    else if (keyboard->KeyUp(_event, SDLK_l)) {
        // Load level
        std::ifstream loadFile("level.bin", std::ios::binary);
        if (loadFile.is_open()) {
            m_level->Deserialize(loadFile);
            loadFile.close();
            std::cout << "Level loaded from level.bin.\n";
        }
        else {
            std::cerr << "Failed to open load file.\n";
        }
    }
}

void GameController::RunGame() {
    Initialize();

    const int targetFPS = 80;
    const float deltaTime = 1.0f / targetFPS;

    std::cout << "Game loop starting...\n";

    while (!m_quit) {
        Uint32 frameStart = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            HandleInput(event);
        }

        // Debug: Ensure level update and rendering are called
        std::cout << "Updating level...\n";
        m_level->Update(deltaTime, InputController::Instance().KB());

        Renderer::Instance().ClearScreen();

        std::cout << "Rendering level...\n";
        m_level->Render();

        SDL_RenderPresent(Renderer::Instance().GetRenderer());

        Uint32 frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < 1000 / targetFPS) {
            SDL_Delay((1000 / targetFPS) - frameTime);
        }
        std::cout << "Frame rendered, frame time: " << frameTime << "ms.\n";
    }

    std::cout << "Exiting game loop.\n";

    ShutDown();
}
