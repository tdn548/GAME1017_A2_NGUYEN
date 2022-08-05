#include "States.h"
#include "StateManager.h"
#include "TextureManager.h"
#include "Engine.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include "SoundManager.h"
#include "Primitives.h"
#include "Button3.h"
#include "PlatformPlayer.h"

#include <iostream>
using namespace std;

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
	TEMA::Load("Img/button.png", "play");
	m_objects.push_back(pair<string, GameObject*>("play",
		new PlayButton({ 0, 0, 400, 100 }, { 412, 350, 200, 50 }, "play")));
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
	TEMA::Unload("play");
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
	//Add player object to objects vector
	/*m_objects.push_back(pair<string , GameObject* >("level",
		new TiledLevel(24,32,32,32,"Dat/Tiledata.txt", "Dat/Level1.txt", "tiles")));*/
	m_objects.push_back(pair<string, GameObject* >("player",
		new PlatformPlayer({ 0,0,125,130 }, {400,400,128,128 })));

	//m_objects.shrink_to_fit();
}

void GameState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_P))
	{
		STMA::PushState(new PauseState());
	}

	if (EVMA::KeyPressed(SDL_SCANCODE_X))
	{
		STMA::ChangeState(new TitleState());
		return;
	}
	for (auto const& i : m_objects)
	{
		//i.first is string key
		// i.second is the value in this case the gameobject*
		i.second->Update();
		if (STMA::StateChanging()) return;
	}
	// check collision
	/*PlatformPlayer* pObj = dynamic_cast<PlatformPlayer*>(GetGo("player"));
	SDL_FRect* pBound = pObj->GetDst();
	SDL_FRect* pLevelB;
	SDL_FRect pLevel = { 0,700,1024,10 };*/

	//for (unsigned i = 0; i < pLevel->GetObstacles().size(); i++)
	//{
	//	SDL_FRect* pTile = pLevel->GetObstacles()[i]->GetDst();
	////	// do collision check Reverse if chck to cut down on nesting
	//	if (!COMA::AABBCheck(*pBound, pLevel)) continue;
	////	//if colliding with top side of tile
	//		if ((pBound->y + pBound->h) - (float)pObj->GetVelY() >= 700 && !COMA::AABBCheck(*pBound, pLevel))
	//		{
	//			pObj->StopY();
	//			pObj->SetY(700 - pBound->h);
	//			pObj->SetGrounded(true);
			/*}*/
		////	// if colliding with bottom side of tile
		//	else if ((pBound->y)  -(float)pObj->GetVelY() >= (pTile->y + pTile->h ))
		//	{
		//		pObj->StopY();
		//		pObj->SetY(pTile->y +  pTile->h);
		//	}
		////	// if colliding with left side of tile
		//	else if ((pBound->x + pBound->w) - (float)pObj->GetVelX() <= pTile->x)
		//	{
		//		pObj->StopX();
		//		pObj->SetX(pTile->x - pBound->w);
		//	}
		////	// if collising with right side of tile
		//	else if ((pBound->x) - (float)pObj->GetVelX() >= (pTile->x + pTile->w))
		//	{
		//		pObj->StopX();
		//		pObj->SetX(pTile->x + pTile->w);
		//	}
		////{

		////}
	/*}*/
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	for (auto const& i : m_objects)
		i.second->Render();
	if ( dynamic_cast<GameState*>(STMA::GetStates().back()) ) 
		State::Render();
}

void GameState::Exit()
{
	TEMA::Unload("tiles");

	for (auto& i : m_objects)
	{
		delete i.second; 
		i.second = nullptr;
	}
	m_objects.clear();
	m_objects.shrink_to_fit();
}

void GameState::Resume(){}
// End GameState

PauseState::PauseState() {}

void PauseState::Enter()
{
	TEMA::Load("img/resume.png", "resume");
	m_objects.push_back(pair<string, GameObject*>("resume",
		new ResumeButton({ 0, 0, 200, 100 }, { 510, 380, 200, 50 }, "resume")));

	TEMA::Load("img/exit.png", "exit");
	m_objects.push_back(pair<string, GameObject*>("exit",
		new EndButton({ 0, 0, 400, 100 }, { 510, 480, 200, 50 }, "exit")));
}

void PauseState::Update()
{
	for (auto const& i : m_objects)
	{
		i.second->Update();
		if (STMA::StateChanging()) return;
	}

	if (EVMA::KeyPressed(SDL_SCANCODE_R))
		STMA::PopState();
}

void PauseState::Render()
{
	// First render the GameState.
	STMA::GetStates().front()->Render();
	// Now render the rest of PauseState.
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 255, 128);
	SDL_Rect rect = { 255, 128, 512, 512 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);

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
	TEMA::Load("Img/button.png", "play");
	m_objects.push_back(pair<string, GameObject*>("play",
		new PlayButton({ 0, 0, 400, 100 }, { 412, 350, 200, 50 }, "play")));
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
	State::Render();
}

void EndState::Exit()
{
	TEMA::Unload("play");
	for (auto& i : m_objects)
	{
		delete i.second;
		i.second = nullptr;
	}
	m_objects.clear();
	m_objects.shrink_to_fit();
}
// End TitleState