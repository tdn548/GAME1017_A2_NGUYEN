#include "States.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "Engine.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"
//#include "Primitives.h"
#include "Button3.h"
#include "PlatformPlayer.h"

#include "tinyxml2.h"
#include <fstream>
#include <iostream>
using namespace std;
using namespace tinyxml2;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

void State::Resume(){}

GameObject* State::GetGo(const std::string& s)
{ // Using 'std' just to show origin.
	auto it = std::find_if(m_objects.begin(), m_objects.end(), 
		// Lambda expression/function. An in-function function.
		[&](const std::pair<std::string, GameObject*>& element)
		{
			return element.first == s;
		} 
	); // End of find_if.
	if (it != m_objects.end())
		return it->second;
	else return nullptr;
}

auto State::GetIt(const std::string& s)
{ 
	auto it = std::find_if(m_objects.begin(), m_objects.end(),
		[&](const std::pair<std::string, GameObject*>& element)
		{
			return element.first == s;
		});
	return it;
}

// Begin TitleState
TitleState::TitleState(){}

void TitleState::Enter()
{
	SOMA::Load("Img/Music/sci_fi_platformer02.mp3", "sci_fi", SOUND_MUSIC);
	SOMA::SetMusicVolume(20);
	SOMA::PlayMusic("sci_fi", -1, 2000);


	//Add background

	TEMA::Load("Img/bg-2.png", "bg");
	m_objects.push_back(pair<string, GameObject*>("bg",
		new Image({ 0, 0, 1920, 1200 }, { 0, 0, 1024, 768 }, "bg")));

	TEMA::Load("Img/title-screen.png", "title");
	m_objects.push_back(pair<string, GameObject*>("title",
		new Image({ 0, 0, 1920, 1200 }, { 250, 300, 501,159 }, "title")));

	TEMA::Load("Img/play2.png", "play");
	m_objects.push_back(pair<string, GameObject*>("play",
		new PlayButton({ 0, 0, 396, 100 }, { 412,500, 200, 50 }, "play")));
}

void TitleState::Update()
{
	for (auto const& i : m_objects)
	{
		i.second->Update();
		if (STMA::StateChanging()) return;
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 64, 0, 16, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	for (auto const& i : m_objects)
		i.second->Render();
	State::Render();
}

void TitleState::Exit()
{
	SOMA::StopMusic();
	SOMA::Unload("sci_fi", SOUND_MUSIC);

	TEMA::Unload("play");
	TEMA::Unload("title");
	TEMA::Unload("bg");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
	m_objects.shrink_to_fit();
}
// End TitleState

// Begin GameState
GameState::GameState(){}

void GameState::Enter() // Used for initialization.
{
	TEMA::Load("Img/Tiles.png", "tiles");
	TEMA::Load("Img/Player.png", "player");
	FOMA::Load("Img/ltype.TTF", "Label", 24);
	TEMA::Load("Img/Vehicles.png", "vehicles");
	TEMA::Load("Img/bg.png", "background");

	SOMA::Load("Img/sound/SFX_Jump.wav", "jump", SOUND_SFX);
	SOMA::Load("Img/sound/Rolling.mp3", "roll", SOUND_SFX);
	SOMA::Load("Img/sound/explosion.wav", "explosion", SOUND_SFX);
	SOMA::Load("Img/Music/cyberpunk-street.mp3", "cyberpunk", SOUND_MUSIC);
	SOMA::SetSoundVolume(20);
	SOMA::SetMusicVolume(10);
	SOMA::PlayMusic("cyberpunk", -1, 2000);

	XMLDocument xmlDoc; // create the DOM

	std::ifstream fileInput("score.xml"); // check whether xml file existing or not

	if (fileInput.fail())
	{
		std::cout << "Failed to open this file!" << std::endl;
		XMLNode* pRoot = xmlDoc.NewElement("Root");
		xmlDoc.InsertEndChild(pRoot);
		xmlDoc.SaveFile("score.xml");
		xmlDoc.LoadFile("score.xml");
	}
	else
	{
		xmlDoc.LoadFile("score.xml");
		XMLNode* pRoot = xmlDoc.FirstChildElement();

		XMLElement* pElement = pRoot->FirstChildElement();
		while (pElement != nullptr) 	// Iterate through the Turret elements in the file and push_back new Turrets into the m_turrets vector.
		{
			score = pElement->IntAttribute("score");
			pElement = pElement->NextSiblingElement();
		}
	}
	string str_score = to_string(score);
	string newTime2 = "Highest Score: " + str_score;

	//m_highestscore->SetText(str_score.c_str());

	m_objects.push_back(pair<string, GameObject* >("player",
		new PlatformPlayer({ 0,0,125,130 }, {50,400,128,128 })));
	m_label = new Label("Label", WIDTH/2 - 300, 40, "Time: 0");
	m_highestscore = new Label("Label", WIDTH / 2 + 80, 40, newTime2.c_str());

	m_timer.Start();

	m_obstacle.push_back(new Obstacle({ 163, 0, 96, 60 }, { WIDTH + 96 ,600,130,100 }));
	

	//Loading backgrounds into vector
	m_bg.reserve(6);

	m_bg.push_back(new Background({ 0,192,255,192 }, { 0,0,WIDTH,HEIGHT }, 1));
	m_bg.push_back(new Background({ 0,192,255,192 }, { WIDTH,0,WIDTH,HEIGHT }, 1));

	m_bg.push_back(new Background({ 0,0,255,192 }, { 0,0,WIDTH,HEIGHT }, 2));
	m_bg.push_back(new Background({ 0,0,255,192 }, { WIDTH,0,WIDTH,HEIGHT }, 2));
	
	m_bg.push_back(new Background({ 257,0,355,192 }, { 0,0,WIDTH,HEIGHT }, 3));
	m_bg.push_back(new Background({ 257,0,355,192 }, { WIDTH,0,WIDTH,HEIGHT }, 3));
}

void GameState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_P))
	{
		STMA::PushState(new PauseState());
		m_timer.Pause();
	}

	if (EVMA::KeyPressed(SDL_SCANCODE_X))
	{
		STMA::ChangeState(new TitleState());
		return;
	}

	for (auto &i : m_obstacle)
	{
		if (i->GetDst()->x <= WIDTH/2 && !i->getPassedMid())
		{
			i->setPassedMid(true);

			switch (rand() % 3 + 1)
			{
			case 1:
				m_obstacle.push_back(new Obstacle({ 163, 0, 96, 60 }, { WIDTH + 96 ,600,130,100 }));
				break;
			case 2:
				m_obstacle.push_back(new Obstacle({ 0, 0, 163, 60 }, { WIDTH + 163 ,600,183,80 }));
				break;
			case 3:
				m_obstacle.push_back(new Obstacle({ 0, 60, 96, 60 }, { WIDTH + 96 ,400,230,200 }));
				break;

			}
			
		}
	}
	
	for (auto const& i : m_objects)
	{
		//i.first is string key
		// i.second is the value in this case the gameobject*
		i.second->Update();
		if (STMA::StateChanging()) return;
	}
	m_timer.Update();


	for (auto &i : m_obstacle)
	{
		i->Update();
	}
	
	// check collision
	PlatformPlayer* pObj = dynamic_cast<PlatformPlayer*>(GetGo("player"));
	SDL_FRect pHitbox = { pObj->GetDst()->x + 30,pObj->GetDst()->y , pObj->GetDst()->w - 60, pObj->GetDst()->h };
	SDL_FRect* pObsBox;
	SDL_FRect pRollingBox = { (pObj->GetDst()->x) + 35 , (pObj->GetDst()->y) + 60, (pObj->GetDst()->w / 2), (pObj->GetDst()->h / 2) };


	for (auto& i: m_obstacle)
	{	
		pObsBox = i->GetDst();

		if (pObj->GetState() != STATE_ROLLING)
		{
			if (COMA::AABBCheck(pHitbox,*pObsBox))
			{
				pObj->Dying(true);
			}
		}
		else
		{
			if (COMA::AABBCheck(pRollingBox, *pObsBox))
			{
				pObj->Dying(true);
			}
		}
		
	}

	for (auto& i : m_obstacle)
	{
		if (i->GetDst()->x < 0 - i->GetDst()->w)
		{
			m_obstacle.erase(std::remove(m_obstacle.begin(), m_obstacle.end(), i), m_obstacle.end());
		}
	}

	// Only move bg while player is still alive
	if (!pObj->IsDying())
	{
		for (size_t i = 0; i < m_bg.size(); i++)
		{
			m_bg[i]->Update();
		}
	}

	pObj = nullptr;
	delete pObj;
	pObsBox = nullptr;
	delete pObsBox;
}


void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	for (size_t i = 0; i < m_bg.size(); i++)
	{
		m_bg[i]->Render();
	}
	for (auto const& i : m_objects)
		i.second->Render();
	if ( dynamic_cast<GameState*>(STMA::GetStates().back()) ) 
		State::Render();
	if (m_timer.HasChanged())
	{
		string newTime = "Time: " + m_timer.GetTime();
		m_label->SetText(newTime.c_str());
	}
	m_label->Render();
	//Highest Score render
	m_highestscore->Render();

	for (auto& i : m_obstacle)
	{
		i->Render();
	}

	// Draw anew.
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

void GameState::Exit()
{
	int die = m_timer.GetLastTime();
	XMLDocument xmlDoc; // create the DOM
	XMLNode* pRoot = xmlDoc.NewElement("Root");
	xmlDoc.InsertEndChild(pRoot);
	if (m_timer.GetLastTime() > score)
	{
		XMLElement* pElement = xmlDoc.NewElement("timer");
		pElement->SetAttribute("score", m_timer.GetLastTime());
		pElement->SetAttribute("die", m_timer.GetLastTime());
		pRoot->InsertEndChild(pElement);
	}
	else
	{
		XMLElement* pElement = xmlDoc.NewElement("timer");
		pElement->SetAttribute("score", score);
		pElement->SetAttribute("die", m_timer.GetLastTime());
		pRoot->InsertEndChild(pElement);
	}
	// Make sure to save to the XML file.
	xmlDoc.SaveFile("score.xml");

	TEMA::Unload("tiles");
	TEMA::Unload("background");
	TEMA::Unload("vehicles");
	FOMA::Quit();
	SOMA::StopMusic();
	SOMA::StopSound();
	SOMA::Unload("cyberpunk", SOUND_MUSIC);
	SOMA::Unload( "jump", SOUND_SFX);
	SOMA::Unload("roll", SOUND_SFX);
	SOMA::Unload("explosion", SOUND_SFX);

	for (auto& i : m_objects)
	{
		delete i.second; 
		i.second = nullptr;
	}

	for (auto& i: m_obstacle)
	{
		delete i;
		i = nullptr;
	}

	m_objects.clear();
	m_objects.shrink_to_fit();

}

void GameState::Resume(){ m_timer.Resume(); }
// End GameState

PauseState::PauseState() {}

void PauseState::Enter()
{
	TEMA::Load("img/resume2.png", "resume");
	m_objects.push_back(pair<string, GameObject*>("resume",
		new ResumeButton({ 0, 0, 396, 200 }, { 400, 380, 200, 50 }, "resume")));
	

	TEMA::Load("img/exit2.png", "exit");
	m_objects.push_back(pair<string, GameObject*>("exit",
		new EndButton({ 0, 0, 396, 200 }, { 400, 480, 200, 50 }, "exit")));
}

void PauseState::Update()
{
	for (auto const& i : m_objects)
	{
		if (i.first == "resume" || i.first == "exit")
		{
			i.second->Update();
			if (STMA::StateChanging()) return;
		}

	}

	if (EVMA::KeyPressed(SDL_SCANCODE_R))
	{
		STMA::PopState();
		
	}
}

void PauseState::Render()
{
	// First render the GameState.
	STMA::GetStates().front()->Render();
	// Now render the rest of PauseState.
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 255, 128);
	//SDL_Rect rect = { 255, 128, 512, 512 };
	//SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);

	for (auto const& i : m_objects)
		i.second->Render();
	State::Render();
}

void PauseState::Exit()
{
	TEMA::Unload("resume");
	TEMA::Unload("exit");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
	m_objects.shrink_to_fit();
}

//Begin EndState

EndState::EndState()
{
}



void EndState::Enter()
{
	
	SOMA::Load("Img/Music/sci_fi_platformer02.mp3", "sci_fi", SOUND_MUSIC);
	SOMA::SetMusicVolume(20);
	SOMA::PlayMusic("sci_fi", -1, 2000);

	FOMA::Load("Img/ltype.TTF", "Label", 24);

	XMLDocument xmlDoc; // create the DOM

	std::ifstream fileInput("score.xml"); // check whether xml file existing or not

		xmlDoc.LoadFile("score.xml");
		XMLNode* pRoot = xmlDoc.FirstChildElement();

		XMLElement* pElement = pRoot->FirstChildElement();
		while (pElement != nullptr) 	// Iterate through the Turret elements in the file and push_back new Turrets into the m_turrets vector.
		{
			die = pElement->IntAttribute("die");
			pElement = pElement->NextSiblingElement();
		}

	string die_label = "YOU DIED ! YOUR SCORE IS : " + to_string(die);;
	m_DieScore = new Label("Label", WIDTH /2 - 170, 170, die_label.c_str());
	m_DieScore->SetColor({ 0,0,255, 255 });



	TEMA::Load("Img/foreground.png", "end");
	m_objects.push_back(pair<string, GameObject*>("end",
		new Image({ 0, 0, 1920, 1200 }, { 0, 0, 1024, 768 }, "end")));

	TEMA::Load("Img/go.png", "gameover");
	m_objects.push_back(pair<string, GameObject*>("gameover",
		new Image({ 0, 0, 250, 200 }, { 370, 200, 250, 200 }, "gameover")));

	TEMA::Load("Img/credits-text.png", "credit");
	m_objects.push_back(pair<string, GameObject*>("credit",
		new Image({ 0, 0, 1920, 1200 }, { 250, 500, 500, 44 }, "credit")));

	TEMA::Load("Img/menu2.png", "menu");
	m_objects.push_back(pair<string, GameObject*>("menu",
		new MenuButton({ 0, 0, 396, 200 }, { 240, 420, 200, 50 }, "menu")));

	TEMA::Load("Img/quit2.png", "quit");
	m_objects.push_back(pair<string, GameObject*>("quit",
		new QuitButton({ 0, 0,396, 200 }, { 560, 420, 200, 50 }, "quit")));
}

void EndState::Update()
{
	for (auto const& i : m_objects)
	{
		i.second->Update();
		if (STMA::StateChanging()) return;
	}
}

void EndState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 64, 0, 16, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	
	for (auto const& i : m_objects)
		i.second->Render();
	m_DieScore->Render();
	State::Render();
}

void EndState::Exit()
{
	SOMA::StopMusic();
	SOMA::Unload("sci_fi", SOUND_MUSIC);
	TEMA::Unload("end");
	TEMA::Unload("menu");
	TEMA::Unload("quit");
	TEMA::Unload("gameover");
	TEMA::Unload("credit");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
	m_objects.shrink_to_fit();
}
// End TitleState