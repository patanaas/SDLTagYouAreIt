#ifndef TEXTURE_H
#define TEXTURE_H

#include "Resource.h"
#include "TGAReader.h"

class Asset;

class Texture : public Resource
{
public:
    // Constructors/Destructors
    Texture();
    virtual ~Texture();

    // Accessors
    Asset* GetData() { return m_texture; }
    ImageInfo* GetImageInfo() { return &m_imageInfo; }
    SDL_BlendMode GetBlendMode() { return m_blendMode; }
    void SetBlendMode(SDL_BlendMode _blendMode) { m_blendMode = _blendMode; }
    byte GetBlendAlpha() { return m_blendAlpha; }
    void SetBlendAlpha(byte _blendAlpha) { m_blendAlpha = _blendAlpha; }

    // Methods
    void Serialize(std::ostream& stream) override;
    void Deserialize(std::istream& stream) override;
    void ToString() override;
    void Load(string guid);

    // Members
    static ObjectPool1<Texture>* Pool;

private:
    ImageInfo m_imageInfo;
    Asset* m_texture;
    SDL_BlendMode m_blendMode;
    byte m_blendAlpha;
};

#endif // TEXTURE_H