#include "Obstacles.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "MathManager.h"
#include <time.h>

Obstacle::Obstacle(SDL_Rect s, SDL_FRect d) : SpriteObject(s, d),
m_angle(0.0), m_radius(33.0),m_rotSpeed(0)
{
	m_center = { (m_dst.x + m_dst.w / 2.0f), (m_dst.y + m_dst.h / 2.0f) }; 
	MAMA::SetDeltas(MAMA::Deg2Rad((rand() % 360) - 90.0), m_dx, m_dy, 2.0f, 2.0f);
}

void Obstacle::Update()
{
	m_center.x -= kScollSpeed;


	// Rotate and move object.
	//m_center.x -= (float)m_dx;
	//m_center.y += (float)m_dy;

	// Wrap on screen.
	//if (m_center.x < -m_dst.w / 2) m_center.x = WIDTH + m_dst.w / 2;
	//else if (m_center.x > WIDTH + m_dst.w / 2) m_center.x = 0 - m_dst.w / 2;
	//else if (m_center.y < -m_dst.h / 2) m_center.y = HEIGHT + m_dst.h / 2;
	//else if (m_center.y > HEIGHT + m_dst.h / 2) m_center.y = 0 - m_dst.h / 2;
	 //Update dest rectangle.

	m_dst.x = m_center.x;
	m_dst.y = 600.0f ;
	//m_dst.x = m_center.x - m_dst.w / 2;

	//m_dst.y = m_center.y - m_dst.h / 2;

	// Check if first column of main vector goes out of bounds.

}

void Obstacle::Render()
{
	SDL_SetTextureColorMod(TEMA::GetTexture("sprites"), 255-m_rMod, 255-m_gMod, 255-m_bMod);
	SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("sprites"),
		&m_src, &m_dst, m_angle, nullptr, SDL_FLIP_NONE);
	SDL_SetTextureColorMod(TEMA::GetTexture("sprites"), 255, 255, 255); // Resetting it so not all sprites are tinted this way.
}

void Obstacle::SetColMods(Uint8 r, Uint8 g, Uint8 b)
{
	m_rMod = r; m_gMod = g; m_bMod = b;
}

ObstacleField::ObstacleField(unsigned int sz) :GameObject({ 0,0,0,0 }), m_size(sz)
{
	for (int i = 0; i < m_size; i++)
	{
		
		m_obstacle.push_back(new Obstacle({ 539, 0, 61, 66 },
			{ 900.0f + rand() % 200, (i % 2 == 0 ? 25.0f : 600.0f) + (rand() % 76),
			61.0f, 66.0f }));

		m_obstacle.back()->SetColMods((rand() % 129), (rand() % 129), (rand() % 129));

	}
	m_obstacle.shrink_to_fit();
}

ObstacleField::~ObstacleField()
{
	for (auto a : m_obstacle)
	{
		delete a;
		a = nullptr; // ;)
	}
	m_obstacle.clear();
}

void ObstacleField::Update()
{
	for (const auto a : m_obstacle)
		a->Update();

}

void ObstacleField::Render()
{
	for (const auto a : m_obstacle)
		a->Render();
}


