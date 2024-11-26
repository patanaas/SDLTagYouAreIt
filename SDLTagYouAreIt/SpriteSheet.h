#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include "Texture.h"
#include "SpriteAnim.h"
#include "BasicStructs.h"

enum AnimationNames
{
    EN_AN_IDLE = 0,
    EN_AN_RUN,
    EN_AN_TWO_COMBO_ATTACK,
    EN_AN_DEATH,
    EN_AN_HURT,
    EN_AN_JUMP_UP_FALL,
    EN_AN_EDGE_GRAB,
    EN_AN_EDGE_IDLE,
    EN_AN_WALL_SIDE,
    EN_AN_CROUCH,
    EN_AN_DASH,
    EN_AN_DASH_ATTACH,
    EN_AN_SLIDE,
    EN_AN_LADDDER_GRAB
};

class SpriteSheet : public Texture
{
public:
    // Constructors/Destructors
    SpriteSheet();
    virtual ~SpriteSheet();

    void SetPosition(const Position& pos) { m_position = pos; }
    Position GetPosition() const { return m_position; }

    void SetRotation(float angle) { m_rotation = angle; }
    float GetRotation() const { return m_rotation; }

    void SetTint(const Color& color) { m_tint = color; }
    Color GetTint() const { return m_tint; }

    void SetScale(float scale) { m_scale = scale; }
    float GetScale() const { return m_scale; }

    void Render();

    // Methods
    void Serialize(std::ostream& _stream) override;
    void Deserialize(std::istream& _stream) override;
    void ToString() override;

    void SetSize(byte _rows, byte _columns, byte _clipSizeX, byte _clipSizeY);
    void AddAnimation(AnimationNames _name, short _clipStart, short _clipCount, float _clipSpeed);
    Rect Update(AnimationNames _name, float _deltaTime);
    int GetCurrentClip(AnimationNames _name);

    // Members
    static ObjectPool1<SpriteSheet>* Pool;

private:
    // Members
    byte m_rows;
    byte m_columns;
    byte m_clipSizeX;
    byte m_clipSizeY;
    map<AnimationNames, SpriteAnim*> m_animations;
    float m_scale;
    float m_rotation;
    Color m_tint;
    Position m_position;
};

#endif // SPRITESHEET_H