#include "GameController.h"
#include "Renderer.h"
#include "SpriteAnim.h"
#include "SpriteSheet.h"
#include "TTFont.h"
#include "RenderTarget.h"
#include "InputController.h"
#include "Keyboard.h"
#include "AudioController.h"
#include "SoundEffect.h"
#include <sstream>

GameController::GameController() //: m_mPos(0,0)
{
	m_sdlEvent = { };
	// Week 9 Code for Input Core
	m_quit = false;
	m_renderer = nullptr;
	m_fArial20 = nullptr;
	//m_text = "";
	m_input = nullptr;
	//m_smPos = "";
	// m_mPos = { };
	//m_ctInfo = "";
	// Code for Week 10 Audio Core
	memset(m_effects, 0, sizeof(SoundEffect*) * MaxEffectChannels);
	m_audio = nullptr;
	m_song = nullptr;
	m_wavDraw = nullptr;
	m_zoomY = 5;
}

GameController::~GameController()
{
	// Week 9 Code for Input Core
	ShutDown();
}

// Week 9 Code for Input Core
void GameController::Initialize()
{
	m_renderer = &Renderer::Instance();
	m_renderer->Initialize();
	m_fArial20 = new TTFont();
	m_fArial20->Initialize(20);
	m_input = &InputController::Instance();
	// Code for Week 10 Audio Core
	AssetController::Instance().Initialize(10000000);	// Allocate 10MB
	m_audio = &AudioController::Instance();
	m_effects[0] = m_audio->LoadEffect("../Assets/Audio/Effects/Whoosh.wav");
	//m_effects[1] = m_audio->LoadEffect("../Assets/Audio/Effects/BeeFlyingLoop.mp3");
	//m_effects[2] = m_audio->LoadEffect("../Assets/Audio/Effects/DistantGunshot.mp3");
	//m_effects[3] = m_audio->LoadEffect("../Assets/Audio/Effects/DrinkSipSwallow.mp3");
	//m_song = m_audio->LoadSong("../Assets/Audio/Music/Track1.mp3");
}

// Week 9 Code for Input Core
void GameController::ShutDown()
{
	delete m_fArial20;
	delete m_wavDraw;
}

// Code for Week 10 Audio Core
void GameController::HandleInput(SDL_Event _event)
{
	string temp;
	if ((m_sdlEvent.type == SDL_QUIT) ||
		(m_input->KB()->KeyUp(_event, SDLK_ESCAPE)))
	{
		m_quit = true;
	}
	/*else if (m_input->KB()->KeyUp(_event, SDLK_p))
	{
		m_audio->Play(m_effects[rand() % 4]);
	}*/
	else if (m_input->KB()->KeyUp(_event, SDLK_a))
	{
		//m_audio->Play(m_song);
		m_zoomY += 0.5f;
	}
	else if (m_input->KB()->KeyUp(_event, SDLK_s))
	{
		//m_audio->PauseMusic();
		m_zoomY -= 0.5f;
	}
	/*else if (m_input->KB()->KeyUp(_event, SDLK_d))
	{
		m_audio->ResumeMusic();
	}
	else if (m_input->KB()->KeyUp(_event, SDLK_f))
	{
		m_audio->StopMusic();
	}*/
}

void GameController::RunGame()
{
	AssetController::Instance().Initialize(10000000);	// Allocate 10MB
	Renderer* r = &Renderer::Instance();
	r->Initialize();

	TTFont* font = new TTFont();
	font->Initialize(20);
	std::string fps = "[D]ecrease Speed; [I]ncrease Speed; [S]ave; [L]oad; [ESC]Quit";
	font->Write(r->GetRenderer(), fps.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 0, 0 });
}

// Code for Week 10 Audio Core
//void GameController::RunGame()
//{
//	Initialize();
//
//	while (!m_quit)
//	{
//		m_renderer->SetDrawColor(Color(255, 255, 255, 255));
//		m_renderer->ClearScreen();
//
//		while (SDL_PollEvent(&m_sdlEvent) != 0)
//		{
//			HandleInput(m_sdlEvent);
//		}
//
//		m_wavDraw->DrawWave(m_effects[0]->GetData(), m_renderer, m_zoomY);
//
//		//string song = "Current Song: " + m_audio->GetMusicTitle();
//		//if (m_audio->GetMusicLength() != "")
//		//{
//		//	song += " " + to_string((int)m_audio->MusicPosition()) + "/" 
//		//		+ m_audio->GetMusicLength();
//		//}
//		//m_fArial20->Write(m_renderer->GetRenderer(), song.c_str(), 
//		//	{ 0, 0, 255 }, { 10, 10 });
//		////m_fArial20->Write(m_renderer->GetRenderer(), ("Current Effect: " + m_audio->GetCurrentEffect()).c_str(), { 0, 0, 255 }, { 10, 30 });
//
//		//for (int count = 0; count < MaxEffectChannels; count++)
//		//{
//		//	string eff = "Effect " + to_string(count) + "; ";
//		//	eff += m_audio->GetCurrentEffects()[count];
//		//	m_fArial20->Write(m_renderer->GetRenderer(), eff.c_str(),
//		//		{ 255, 0, 255 }, { 10, 30 + (count * 20) });
//		//}
//
//		SDL_RenderPresent(m_renderer->GetRenderer());
//	}
//}




// Week 9 Code for Input Core
/*
void GameController::HandleInput(SDL_Event _event)
{
	string temp;
	if ((m_sdlEvent.type == SDL_QUIT) ||
		(m_input->KB()->KeyUp(m_sdlEvent, SDLK_ESCAPE)))
	{
		m_quit = true;
	}
	else if ((temp = m_input->KB()->TextInput(_event)) != "")
	{
		m_text += temp;
	}
	else if (m_input->KB()->KeyUp(m_sdlEvent, SDLK_ESCAPE))
	{
		m_text = "";
	}

	else if (m_input->MS()->Moved(m_sdlEvent, m_mPos))
	{
		m_smPos = "Mouse Position [" + to_string(m_mPos.X) +
			"; " + to_string(m_mPos.Y) + "]";
	}

	else if ((m_input->CT()->Added(m_sdlEvent)) ||
		(m_input->CT()->Removed(m_sdlEvent)) ||
		(m_input->CT()->ProcessButtons(m_sdlEvent)) ||
		(m_input->CT()->ProcessMotion(m_sdlEvent)))
	{
		m_ctInfo = m_input->CT()->ToString();
	}
	m_input->MS()->ProcessButtons(_event);
}
*/

// Week 9 Code for Input Core
/*
void GameController::RunGame()
{
	Initialize();
	m_input->CT()->DetectControllers();

	while (!m_quit)
	{
		m_renderer->SetDrawColor(Color(255, 255, 255, 255));
		m_renderer->ClearScreen();

		while (SDL_PollEvent(&m_sdlEvent) != 0)
		{
			HandleInput(m_sdlEvent);
		}

		m_fArial20->Write(m_renderer->GetRenderer(), m_text.c_str(),
			SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 200 });

		m_fArial20->Write(m_renderer->GetRenderer(), m_smPos.c_str(),
			SDL_Color{ 0, 0, 255 }, SDL_Point{ 250, 220 });
		m_fArial20->Write(m_renderer->GetRenderer(), ("Left: " +
			to_string(m_input->MS()->GetButLDown())).c_str() ,
			SDL_Color{ 0, 0, 255 }, SDL_Point{ 250, 240 });
		m_fArial20->Write(m_renderer->GetRenderer(), ("Middle: " +
			to_string(m_input->MS()->GetButMDown())).c_str(),
			SDL_Color{ 0, 0, 255 }, SDL_Point{ 250, 260 });
		m_fArial20->Write(m_renderer->GetRenderer(), ("Right: " +
			to_string(m_input->MS()->GetButRDown())).c_str(),
			SDL_Color{ 0, 0, 255 }, SDL_Point{ 250, 280 });
		m_fArial20->Write(m_renderer->GetRenderer(), m_ctInfo.c_str(),
			SDL_Color{ 255, 0, 0 }, SDL_Point{ 250, 300 });

		SDL_RenderPresent(m_renderer->GetRenderer());
	}
}
*/

//void GameController::RunGame()
//{
//	AssetController::Instance().Initialize(10000000);	// Allocate 10MB
//	Renderer* r = &Renderer::Instance();
//	r->Initialize(800, 600);
//	Point ws = r->GetWindowSize();
//
//	Texture::Pool = new ObjectPool1<Texture>();
//	Texture* texture = Texture::Pool->GetResource();
//	texture->Load("../Assets/Textures/Warrior.tga");
//
//	while (m_sdlEvent.type != SDL_QUIT)
//	{
//		SDL_PollEvent(&m_sdlEvent);
//		r->SetDrawColor(Color(255, 255, 255, 255));
//		r->ClearScreen();
//		for (unsigned int count = 0; count < 6; count++)
//		{
//			unsigned int xPos = count * 69;
//			r->RenderTexture(texture, Rect(xPos, 0, xPos + 69, 44), Rect(xPos, 100, xPos + 69 * 2, 100 + 44 * 2));
//		}
//		SDL_RenderPresent(r->GetRenderer());
//	}
//
//	delete Texture::Pool;
//	r->Shutdown();
//}


//void GameController::RunGame()
//{
//	AssetController::Instance().Initialize(10000000);	// Allocate 10MB
//	Renderer* r = &Renderer::Instance();
//	Timing* t = &Timing::Instance();
//	//r->Initialize(800, 600);
//	r->Initialize();
//	r->EnumerateDisplayModes();
//	r->ChangeDisplayMode(&r->GetResolutions()[30]);		// 800x600
//
//	TTFont* font = new TTFont();
//	font->Initialize(20);
//
//	SpriteSheet::Pool = new ObjectPool1<SpriteSheet>();
//	SpriteAnim::Pool = new ObjectPool1<SpriteAnim>();
//	SpriteSheet* sheet = SpriteSheet::Pool->GetResource();
//	sheet->Load("../Assets/Textures/Warrior.tga");
//	sheet->SetSize(17, 6, 69, 44);
//	sheet->AddAnimation(EN_AN_IDLE, 0, 6, 6.0f);
//	sheet->AddAnimation(EN_AN_RUN, 6, 8, 6.0f);
//	//sheet->SetBlendMode(SDL_BLENDMODE_BLEND);
//	//sheet->SetBlendAlpha(128);
//	//Point ws = r->GetWindowSize();
//	RenderTarget* rt = new RenderTarget();
//	//rt->Create(ws.X, ws.Y);
//	rt->Create(NATIVE_XRES, NATIVE_YRES);	// Set to game's native resolution
//
//	while (m_sdlEvent.type != SDL_QUIT)
//	{
//		t->Tick();
//		rt->Start();
//
//		//SDL_PollEvent(&m_sdlEvent);
//		r->SetDrawColor(Color(255, 255, 255, 255));
//		r->ClearScreen();
//		/*
//		r->RenderTexture(sheet, sheet->Update(EN_AN_IDLE, t->GetDeltaTime()), Rect(0, 0, 69 * 3, 44 * 3));
//		*/
//		r->RenderTexture(sheet, sheet->Update(EN_AN_RUN, t->GetDeltaTime()), Rect(0, 150, 69 * 3, 150 + 44 * 3));
//
//		/*
//		std::string s = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_IDLE));
//		font->Write(r->GetRenderer(), s.c_str(), SDL_Color{0, 255, 0}, SDL_Point{250, 50});
//
//		s = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_RUN));
//		font->Write(r->GetRenderer(), s.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 200 });
//		*/
//
//		std::string fps = "Frames per second: " + std::to_string(t->GetFPS());
//		font->Write(r->GetRenderer(), fps.c_str(), SDL_Color{ 0, 0, 255 }, SDL_Point{ 0, 0 });
//
//		std::string s = "Frame number: " + std::to_string(sheet->GetCurrentClip(EN_AN_RUN));
//		font->Write(r->GetRenderer(), s.c_str(), SDL_Color{ 0, 255, 0 }, SDL_Point{ 250, 200 });
//
//		/*
//		vector<SDL_DisplayMode> modes = r->GetResolutions();
//		for (int count = 0; count < modes.size(); count++)
//		{
//			stringstream s;
//			s << modes[count].w << "x" << modes[count].h << " " << modes[count].refresh_rate << "hz";
//			font->Write(r->GetRenderer(), s.str().c_str(), SDL_Color{ 255, 128, 0 }, SDL_Point{ 0, 20 + count * 20 });
//		}
//		*/
//
//
//		rt->Stop();
//		r->SetDrawColor(Color(0, 0, 0, 255));
//		r->ClearScreen();
//		rt->Render(t->GetDeltaTime());	// Scale native resolution screen resolution
//		SDL_RenderPresent(r->GetRenderer());
//		t->CapFPS();
//	}
//
//	delete rt;
//	delete SpriteAnim::Pool;
//	delete SpriteSheet::Pool;
//
//	font->Shutdown();
//	r->Shutdown();
//}
