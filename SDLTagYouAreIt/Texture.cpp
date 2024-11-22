#include "Texture.h"

ObjectPool1<Texture>* Texture::Pool = nullptr;

Texture::Texture()
{
    m_imageInfo = { };
    m_texture = nullptr;
    m_blendMode = SDL_BLENDMODE_BLEND;
    m_blendAlpha = 255;
}

Texture::~Texture()
{
}

void Texture::Load(string _guid)
{
    TGAReader r = TGAReader();
    m_texture = r.LoadTGAFromFile(_guid, &m_imageInfo);
}

void Texture::Serialize(std::ostream& _stream)
{
    SerializeAsset(_stream, m_texture);
    _stream.write(reinterpret_cast<char*>(&m_blendMode), sizeof(m_blendMode));
    _stream.write(reinterpret_cast<char*>(&m_blendAlpha), sizeof(m_blendAlpha));
}

void Texture::Deserialize(std::istream& _stream)
{
    TGAReader r = TGAReader();
    DeserializeAsset(_stream, m_texture);
    r.ProcessAsset(m_texture, &m_imageInfo);
    _stream.read(reinterpret_cast<char*>(&m_blendMode), sizeof(m_blendMode));
    _stream.read(reinterpret_cast<char*>(&m_blendAlpha), sizeof(m_blendAlpha));
}

void Texture::ToString()
{
    cout << "TEXTURE" << endl;
    m_texture->ToString();
    Resource::ToString();
}