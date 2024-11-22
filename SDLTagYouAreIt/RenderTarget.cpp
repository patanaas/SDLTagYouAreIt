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
	//m_rotation += 10 * _deltaTime;
	Point ws = Renderer::Instance().GetWindowSize();
	float perc = float(NATIVE_YRES - ws.Y) / NATIVE_YRES;	// e.g. 1080 - 600 = 480, therefore 480 = 0.4444444 of 1080
	int xRes = ws.X + (NATIVE_XRES * perc);		// now add 0.444444 * NATIVE_XRES to the current resolution
	// i.e. 0.444444 * 1920 = 853 + 800 = 1653
	// same as ws.Y + (NATIVE_YRES * perc) = 600 * 0.4444444 = 480 + 600 = 1080
	int yRes = NATIVE_YRES;
	Renderer::Instance().RenderTexture(m_texture, Rect(0, 0, xRes, yRes), Rect(0, 0, ws.X, ws.Y), m_rotation);
}