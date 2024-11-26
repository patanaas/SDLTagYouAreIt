#ifndef PLAYER_H
#define PLAYER_H

#include "Texture.h"
#include "BasicStructs.h"

class Player {
public:
    Player();
    ~Player();

    void Load(const std::string& filePath);
    void Update(float deltaTime);
    void Serialize(std::ostream& stream);
    void Deserialize(std::istream& stream);

    Texture* GetTexture() const { return texture; }
    Rect GetRect() const;

    Point position;

private:
    Texture* texture;
};

#endif // PLAYER_H
