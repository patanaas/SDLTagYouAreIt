#include "Renderer.h"
#include "Asset.h"

Renderer::Renderer() {
    m_window = nullptr;
    m_renderer = nullptr;
    m_destRect = {};
    m_surface = nullptr;
    m_viewPort = {};
    m_srcRect = {};
}

Renderer::~Renderer() {
    Shutdown();
}

void Renderer::Shutdown() {
    for (auto it = m_textures.begin(); it != m_textures.end(); it++) {
        SDL_DestroyTexture(it->second);
    }
    m_textures.clear();

    if (m_renderer != nullptr) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }

    if (m_window != nullptr) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    SDL_Quit();
}

void Renderer::SetDrawColor(Color _color) {
    SDL_SetRenderDrawColor(m_renderer, _color.R, _color.G, _color.B, _color.A);
}

void Renderer::ClearScreen() {
    SDL_RenderClear(m_renderer);
}

void Renderer::RenderTexture(Texture* _texture, Rect _srcRect, Rect _destRect, double _angle, SDL_Color tint) {
    m_destRect.x = _destRect.X1;
    m_destRect.y = _destRect.Y1;
    m_destRect.w = _destRect.X2 - _destRect.X1;
    m_destRect.h = _destRect.Y2 - _destRect.Y1;

    m_srcRect.x = _srcRect.X1;
    m_srcRect.y = _srcRect.Y1;
    m_srcRect.w = _srcRect.X2 - _srcRect.X1;
    m_srcRect.h = _srcRect.Y2 - _srcRect.Y1;

    SDL_Texture* sdlTexture = GetSDLTexture(_texture);

    SDL_SetTextureColorMod(sdlTexture, tint.r, tint.g, tint.b);
    SDL_SetTextureAlphaMod(sdlTexture, tint.a);

    M_ASSERT(((SDL_RenderCopyEx(m_renderer, sdlTexture, &m_srcRect, &m_destRect, _angle, nullptr, SDL_FLIP_NONE)) >= 0),
        "Could not render texture");
}


void Renderer::RenderTexture(SDL_Texture* _texture, Rect _srcRect, Rect _destRect, double _angle, SDL_Color tint)
{
    m_destRect.x = _destRect.X1;
    m_destRect.y = _destRect.Y1;
    m_destRect.w = _destRect.X2 - _destRect.X1;
    m_destRect.h = _destRect.Y2 - _destRect.Y1;

    m_srcRect.x = _srcRect.X1;
    m_srcRect.y = _srcRect.Y1;
    m_srcRect.w = _srcRect.X2 - _srcRect.X1;
    m_srcRect.h = _srcRect.Y2 - _srcRect.Y1;

    // Set texture color and alpha modulation
    SDL_SetTextureColorMod(_texture, tint.r, tint.g, tint.b);
    SDL_SetTextureAlphaMod(_texture, tint.a);

    // Render the texture with rotation
    M_ASSERT((SDL_RenderCopyEx(m_renderer, _texture, &m_srcRect, &m_destRect, _angle, nullptr, SDL_FLIP_NONE) >= 0), "Could not render texture");
}

SDL_Texture* Renderer::GetSDLTexture(Texture* _texture) {
    Asset* asset = _texture->GetData();
    string guid = asset->GetGUID();

    if (m_textures.count(guid) == 0) {
        ImageInfo* ii = _texture->GetImageInfo();
        m_surface = SDL_CreateRGBSurfaceFrom(
            asset->GetData() + _texture->GetImageInfo()->DataOffset, ii->Width, ii->Height,
            ii->BitsPerPixel, ii->Width * ii->BitsPerPixel / 8, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
        m_surface = nullptr;
        m_textures[guid] = texture;
    }

    return m_textures[guid];
}

Point Renderer::GetWindowSize() {
    int w, h;
    SDL_GetWindowSize(m_window, &w, &h);
    return Point(w, h);
}

void Renderer::Initialize() {
    M_ASSERT((SDL_Init(SDL_INIT_EVERYTHING) >= 0), "");
    m_window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        1280, 720, SDL_WINDOW_SHOWN);
    M_ASSERT(m_window != nullptr, "Failed to initialize SDL window.");

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    M_ASSERT(m_renderer != nullptr, "Failed to initialize SDL renderer.");
}

void Renderer::RenderRectangle(Rect _rect) {
    m_destRect.x = _rect.X1;
    m_destRect.y = _rect.Y1;
    m_destRect.w = _rect.X2 - _rect.X1;
    m_destRect.h = _rect.Y2 - _rect.Y1;
    SDL_RenderDrawRect(m_renderer, &m_destRect);
}

void Renderer::EnumerateDisplayModes() {
    int display_count = SDL_GetNumVideoDisplays();
    for (int display_index = 0; display_index <= display_count; display_index++) {
        int modes_count = SDL_GetNumDisplayModes(display_index);
        for (int mode_index = 0; mode_index <= modes_count; mode_index++) {
            SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };
            if (SDL_GetDisplayMode(display_index, mode_index, &mode) == 0) {
                m_resolution.push_back(mode);
            }
        }
    }
}
