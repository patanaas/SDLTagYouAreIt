#include "Level.h"
#include "Renderer.h"
#include "InputController.h"
#include "AudioController.h"
#include "Keyboard.h"
#include "GameController.h"
#include <iostream>
#include <fstream>
#include <cmath> // For sqrt and pow

// Helper function to calculate distance between two points
float Distance(const Position& a, const Position& b) {
    return sqrt(pow(b.X - a.X, 2) + pow(b.Y - a.Y, 2));
}

Level::Level()
    : m_player(nullptr), m_npcTaggedCount(0), m_npcSpeed(60.0f) {}

Level::~Level() {
    if (m_player) {
        delete m_player;
        m_player = nullptr;
    }

    for (auto npc : m_npcs) {
        delete npc;
    }
    m_npcs.clear();
}

void Level::Initialize() {
    std::cout << "Initializing level...\n";

    CreatePlayer();
    if (m_player) {
        std::cout << "Player initialized at position: ("
            << m_player->GetPosition().X << ", "
            << m_player->GetPosition().Y << ")\n";
    }
    else {
        std::cout << "Player initialization failed!\n";
    }

    CreateNPCs();
    std::cout << "Number of NPCs initialized: " << m_npcs.size() << "\n";
    for (size_t i = 0; i < m_npcs.size(); ++i) {
        Position pos = m_npcs[i]->GetPosition();
        std::cout << "NPC " << i << " initialized at position: ("
            << pos.X << ", " << pos.Y << ")\n";
    }
}

void Level::CreatePlayer() {
    m_player = SpriteSheet::Pool->GetResource();
    if (!m_player) {
        std::cout << "Failed to allocate player sprite from pool.\n";
        return;
    }
    m_player->Load("../Assets/Textures/Warrior.tga");
    m_player->SetSize(17, 6, 69, 44);
    m_player->AddAnimation(EN_AN_IDLE, 0, 6, 6.0f);
    m_player->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);
    m_player->SetPosition(Position(960, 540)); // Center screen for 1920x1080
    m_player->SetTint(Color(0, 0, 255, 255));  // Blue tint for player
    m_player->SetScale(1.25f);

    std::cout << "Player sprite loaded and positioned.\n";
}

void Level::CreateNPCs() {
    for (int i = 0; i < 10; ++i) {
        SpriteSheet* npc = SpriteSheet::Pool->GetResource();
        if (!npc) {
            std::cout << "Failed to allocate NPC sprite from pool.\n";
            continue;
        }
        npc->Load("../Assets/Textures/Warrior.tga");
        npc->SetSize(17, 6, 69, 44);
        npc->AddAnimation(EN_AN_IDLE, 0, 6, 6.0f);
        npc->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);
        npc->SetScale(1.25f);
        npc->SetTint(Color(0, 255, 0, 255)); // Green tint for NPCs

        // Randomize NPC position
        int offsetX = rand() % 301 - 150; // -150 to 150
        int offsetY = rand() % 301 - 150; // -150 to 150
        npc->SetPosition(Position(960 + offsetX, 540 + offsetY)); // Random position around player

        m_npcs.push_back(npc);

        std::cout << "NPC " << i << " positioned at ("
            << 960 + offsetX << ", " << 540 + offsetY << ").\n";
    }
}

void Level::Update(float deltaTime, Keyboard* keyboard) {
    const Uint8* keyState = SDL_GetKeyboardState(NULL); // Get keyboard state
    Position playerPos = m_player->GetPosition();
    float moveSpeed = 100.0f; // Player move speed
    bool moving = false;

    // Handle movement based on keyboard state
    if (keyState[SDL_SCANCODE_UP]) {
        playerPos.Y -= moveSpeed * deltaTime;
        moving = true;
    }
    if (keyState[SDL_SCANCODE_DOWN]) {
        playerPos.Y += moveSpeed * deltaTime;
        moving = true;
    }
    if (keyState[SDL_SCANCODE_LEFT]) {
        playerPos.X -= moveSpeed * deltaTime;
        moving = true;
        m_player->SetRotation(180); // Rotate to face left
    }
    if (keyState[SDL_SCANCODE_RIGHT]) {
        playerPos.X += moveSpeed * deltaTime;
        moving = true;
        m_player->SetRotation(0); // Rotate to face right
    }

    m_player->SetPosition(playerPos);

    // Switch animations based on movement
    if (moving) {
        m_player->Update(EN_AN_RUN, deltaTime);
    }
    else {
        m_player->Update(EN_AN_IDLE, deltaTime);
    }

    // NPC movement and tagging logic remains unchanged
    for (auto it = m_npcs.begin(); it != m_npcs.end();) {
        SpriteSheet* npc = *it;
        Position npcPos = npc->GetPosition();
        float dist = Distance(playerPos, npcPos);

        if (dist < 30.0f) {
            // Tag NPC
            npc->SetTint(Color(255, 0, 0, 255)); // Red tint for tagged
            npc->Update(EN_AN_DEATH, deltaTime);

            // Play swoosh sound
            SoundEffect* swoosh = AudioController::Instance().LoadEffect("../Assets/Audio/Effects/Whoosh.wav");
            AudioController::Instance().Play(swoosh);

            // Remove NPC
            it = m_npcs.erase(it);
            IncrementNPCTaggedCount();
        }
        else {
            // NPC movement logic
            if (dist < 140.0f) {
                // Move away from player
                if (npcPos.X < playerPos.X) npcPos.X -= m_npcSpeed * deltaTime;
                if (npcPos.X > playerPos.X) npcPos.X += m_npcSpeed * deltaTime;
                if (npcPos.Y < playerPos.Y) npcPos.Y -= m_npcSpeed * deltaTime;
                if (npcPos.Y > playerPos.Y) npcPos.Y += m_npcSpeed * deltaTime;
            }
            else if (dist > 160.0f) {
                // Move toward player
                if (npcPos.X < playerPos.X) npcPos.X += m_npcSpeed * deltaTime;
                if (npcPos.X > playerPos.X) npcPos.X -= m_npcSpeed * deltaTime;
                if (npcPos.Y < playerPos.Y) npcPos.Y += m_npcSpeed * deltaTime;
                if (npcPos.Y > playerPos.Y) npcPos.Y -= m_npcSpeed * deltaTime;
            }

            // Rotate NPC to face player
            float angle = atan2(playerPos.Y - npcPos.Y, playerPos.X - npcPos.X) * (180.0f / M_PI);
            npc->SetRotation(angle);

            npc->SetPosition(npcPos);
            npc->Update(EN_AN_RUN, deltaTime); // NPC is always "running"
            ++it;
        }
    }

    // Quit game if no NPCs left
    if (m_npcs.empty()) {
        GameController::Instance().SetQuitFlag(true);
    }
}

void Level::Render() {
    // Render player
    m_player->Render();

    // Render NPCs
    for (auto npc : m_npcs) {
        npc->Render();
    }
}

void Level::Serialize(std::ostream& stream) {
    m_player->Serialize(stream);

    int npcCount = static_cast<int>(m_npcs.size());
    stream.write(reinterpret_cast<char*>(&npcCount), sizeof(npcCount));

    for (auto npc : m_npcs) {
        npc->Serialize(stream);
    }

    stream.write(reinterpret_cast<char*>(&m_npcSpeed), sizeof(m_npcSpeed));
    stream.write(reinterpret_cast<char*>(&m_npcTaggedCount), sizeof(m_npcTaggedCount));
}

void Level::Deserialize(std::istream& stream) {
    if (!m_player) {
        m_player = SpriteSheet::Pool->GetResource();
    }
    m_player->Deserialize(stream);

    int npcCount;
    stream.read(reinterpret_cast<char*>(&npcCount), sizeof(npcCount));

    for (auto npc : m_npcs) {
        SpriteSheet::Pool->ReleaseResource(npc);
    }
    m_npcs.clear();

    for (int i = 0; i < npcCount; ++i) {
        SpriteSheet* npc = SpriteSheet::Pool->GetResource();
        npc->Deserialize(stream);
        m_npcs.push_back(npc);
    }

    stream.read(reinterpret_cast<char*>(&m_npcSpeed), sizeof(m_npcSpeed));
    stream.read(reinterpret_cast<char*>(&m_npcTaggedCount), sizeof(m_npcTaggedCount));
}
