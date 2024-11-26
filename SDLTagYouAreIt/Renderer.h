#ifndef RENDERER_H
#define RENDERER_H

#include "StandardIncludes.h"
#include "Texture.h"
#include "Singleton.h"

class Asset;

class Renderer : public Singleton<Renderer>
{
public:
	// Constructors and Destructors
	Renderer();
	virtual ~Renderer();

	// Accessors
	SDL_Window* GetWindow() { return m_window; }
	SDL_Renderer* GetRenderer() { return m_renderer; }
	SDL_Texture* GetSDLTexture(Texture* _texture);
	vector<SDL_DisplayMode>& GetResolutions() { return m_resolution; }

	// Methods
	void Initialize(int _xResolution, int _yResolution);
	Point GetWindowSize();
	void SetViewport(Rect _viewport);
	void RenderTexture(Texture* _texture, Rect _rect);
	void SetDrawColor(Color _color);
	void ClearScreen();
	void RenderPoint(Point _position);
	void RenderLine(Rect _points);
	void RenderRectangle(Rect _rect);
	void RenderFillRectangle(Rect _rect);
	void RenderTexture(Texture* _texture, Point _point);
	void Shutdown();
	void RenderTexture(Texture* _texture, Rect _srcRect, Rect _destRect);
	void RenderTexture(SDL_Texture* _texture, Rect _srcRect, Rect _destRect, double _angle);
	//void Initialize();
	void EnumerateDisplayModes();
	void ChangeDisplayMode(SDL_DisplayMode* _mode);

private:
	// Members
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Rect m_destRect;
	SDL_Surface* m_surface;
	SDL_Rect m_srcRect;
	SDL_Rect m_viewPort;
	map<string, SDL_Texture*> m_textures;
	vector<SDL_DisplayMode> m_resolution;
};

#endif // !RENDERER_H



