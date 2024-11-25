#pragma once
#include <SDL.h>
#include "BasicStructs.h"
#include "Serializable.h"

class PlayerWarrior : public Serializable 
{
private:
    Warrior position;
    Point position1;
    float speed;
    SDL_Texture* texture;
    SDL_Renderer* renderer;

public:
    PlayerWarrior(SDL_Renderer* renderer);
    virtual ~PlayerWarrior();
    void update();
    void render();
    void Serialize(std::ostream& _stream);
    void Deserializable(std::istream& _stream);

    Point getPosition() const {
        return position1;
    }

    void setPosition(float x, float y) {
        position1.X =  x;
        position1.Y = y;
    }
};
