#pragma once
#ifndef _OBSTACLES_
#define _OBSTACLES_

#include "GameObject.h"
#include "SDL.h"
#include <vector>

class Obstacle : public SpriteObject
{
public:
	Obstacle(SDL_Rect s, SDL_FRect d);
	void Update();
	void Render();
	/*const SDL_FPoint& GetCenter() { return m_center; }
	const double& GetRadius() { return m_radius; }*/

	/*Uint8 GetRed() { return m_rMod; };
	Uint8 GetGreen() { return m_gMod; };
	Uint8 GetBlue() { return m_bMod; };*/
	double& GetDx() { return m_dx; }
	double& GetDy() { return m_dy; }
	bool getPassedMid() { return passedMid; }
	void setPassedMid(bool passed);

	/*void SetColMods(Uint8 r, Uint8 g, Uint8 b);*/
private:
	static const int kScollSpeed = 5;
	//SDL_FPoint m_center;
	double m_angle,
		m_dx, m_dy;
	bool passedMid;
		/*m_radius,
		m_rotSpeed;*/
	/*Uint8 m_rMod, m_gMod, m_bMod;*/
};

//class ObstacleField : public GameObject
//{ // Manager class for the individual asteroids.
//public:
//	ObstacleField(unsigned int sz = 6);
//	~ObstacleField();
//	void Update();
//	void Render();
//	vector<Obstacle*>& GetObstacle() { return m_obstacle; }
//	const unsigned int GetSize() { return m_size; }
//
//private:
//	vector<Obstacle*> m_obstacle;
//	int m_size;
//};
#endif