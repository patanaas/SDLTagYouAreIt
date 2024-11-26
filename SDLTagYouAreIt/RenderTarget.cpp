#include "RenderTarget.h"
#include "Renderer.h"
#include "BasicStructs.h"

RenderTarget::RenderTarget()
{
	m_texture = nullptr;
	m_width = 0;
	m_height = 0;
	m_rotation = 0;
}

RenderTarget::~RenderTarget()
{
	if (m_texture != nullptr)
	{
		SDL_DestroyTexture(m_texture);
		m_texture = nullptr;
	}
}

void RenderTarget::Create(unsigned int _width, unsigned int _height)
{
	m_width = _width;
	m_height = _height;

	m_texture = SDL_CreateTexture(Renderer::Instance().GetRenderer(),
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_width, m_height);
	M_ASSERT(m_texture != nullptr, "Unable to create render target");
}

void RenderTarget::Start()
{
	SDL_SetRenderTarget(Renderer::Instance().GetRenderer(), m_texture);
}

void RenderTarget::Stop()
{
	SDL_SetRenderTarget(Renderer::Instance().GetRenderer(), nullptr);
}

void RenderTarget::Render(float _deltaTime)
{
	// Calculate scaling based on window size
	Point ws = Renderer::Instance().GetWindowSize();
	float perc = float(NATIVE_YRES - ws.Y) / NATIVE_YRES;
	int xRes = ws.X + int(NATIVE_XRES * perc);
	int yRes = NATIVE_YRES;

	// Create source and destination rectangles
	Rect srcRect = Rect(0, 0, xRes, yRes);
	Rect destRect = Rect(0, 0, ws.X, ws.Y);

	// Use a function that works with SDL_Texture* (added a new function to Renderer if needed)
	Renderer::Instance().RenderTexture(m_texture, srcRect, destRect, m_rotation);
}
