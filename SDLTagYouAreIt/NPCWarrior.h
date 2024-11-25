#ifndef NPCWARRIOR_H
#define NPCWARRIOR_H

#include <SDL.h>
#include "BasicStructs.h"
#include "Serializable.h"
#include <cstdlib> // for rand()

class NPCWarrior : public Serializable {
private:
    Point position; // NPC's position
    bool tagged;    // If NPC is tagged
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    float speed;

public:

    // Constructor
    NPCWarrior(SDL_Renderer* renderer)
        : renderer(renderer), position(0, 0), speed(0.0f), texture(nullptr) {}
    virtual ~NPCWarrior();

    virtual void update(float speed) = 0; // Update NPC movement
    virtual void render() = 0;            // Render NPC
    bool isTagged() const { return tagged; } // Check if tagged
    void setTagged(bool state) { tagged = state; }

    Point getPosition() const { return position; }
    void setPosition(float x, float y) { position.X = x; position.Y = y; }

    void Serialize(std::ofstream& _stream);
    void Deserializable(std::ifstream& _stream) ;
};



#endif // !NPCWARRIOR_H
