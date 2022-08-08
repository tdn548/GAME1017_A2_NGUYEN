#pragma once
#ifndef _STATES_H_
#define _STATES_H_

#include "TiledLevel.h"
#include "GameObject.h"
#include "Primitives.h"
#include "FontManager.h"
#include "Timer.h"
#include "Background.h"
#include "Obstacles.h"

class State // This is the abstract base class for all states
{
public:
	virtual void Enter() = 0; // = 0 means pure virtual - must be defined in subclass
	virtual void Update() = 0; 
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume(); 
	virtual ~State() {} // or = default;
	GameObject* GetGo(const std::string& s); //Gets gameobject* pointer by key name
	auto GetIt(const std::string& s); // get the interator in the vector for a gameobject*
protected: // Private but inherited
	State() {} // What does this prevent?

	vector<std::pair<std::string, GameObject*>> m_objects; // hold all the gameobjects in the state
	//Background vector
	vector<Background*> m_bg;
	vector<Obstacle*> m_obstacle;
	GameObject* m_pObstacle;
	Label* m_label;
	Label* m_highestscore;
	Timer m_timer;
};

class TitleState : public State
{
public:
	TitleState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

class GameState : public State
{
public:
	int score;
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();
};

class PauseState : public State
{
public: // Public methods.
	PauseState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

class EndState : public State
{
public:
	EndState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};


#endif

