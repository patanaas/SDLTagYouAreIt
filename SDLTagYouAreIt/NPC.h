#ifndef NPC_H
#define NPC_H

#include "Texture.h"
#include "BasicStructs.h"

class NPC {
public:
    NPC();
    ~NPC();

    void Load(const std::string& filePath);
    void Update(const Point& playerPosition, float deltaTime);
    void Serialize(std::ostream& stream);
    void Deserialize(std::istream& stream);

    Texture* GetTexture() const { return texture; }
    Rect GetRect() const;

    Point position;
    bool isTagged;

private:
    Texture* texture;
    float speed;
};

#endif // NPC_H
