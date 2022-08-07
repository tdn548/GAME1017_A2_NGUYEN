#include "Obstacles.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "MathManager.h"
#include <time.h>

Obstacle::Obstacle(SDL_Rect s, SDL_FRect d) : SpriteObject(s, d),
m_angle(0.0)
{
	/*m_center = { (m_dst.x + m_dst.w / 2.0f), (m_dst.y + m_dst.h / 2.0f) }; */
	/*MAMA::SetDeltas(MAMA::Deg2Rad((rand() % 360) - 90.0), m_dx, m_dy, 2.0f, 2.0f);*/
}

void Obstacle::Update()
{
	m_dst.x -= kScollSpeed;
	//m_dst.x = m_center.x;
	//m_dst.y = 600.0f ;


}

void Obstacle::Render()
{
	/*SDL_SetTextureColorMod(TEMA::GetTexture("sprites"), 255-m_rMod, 255-m_gMod, 255-m_bMod);*/
	SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("sprites"),
		&m_src, &m_dst, m_angle, nullptr, SDL_FLIP_NONE);
	//SDL_SetTextureColorMod(TEMA::GetTexture("sprites"), 255, 255, 255); // Resetting it so not all sprites are tinted this way.
}

//void Obstacle::SetColMods(Uint8 r, Uint8 g, Uint8 b)
//{
//	m_rMod = r; m_gMod = g; m_bMod = b;
//}

ObstacleField::ObstacleField(unsigned int sz) :GameObject({ 0,0,0,0 }), m_size(sz)
{
	for (int i = 0; i < m_size; i++)
	{
		m_obstacle.push_back(new Obstacle({ 539, 0, 61, 66 },
			{ 1000.0f + rand() % 900, 
			(i % 2 == 0 ? 550.0f : 600.0f) ,//(i % 2 == 0 ? 25.0f : 600.0f) + (rand() % 76),
			61.0f, 66.0f }));

		/*m_obstacle.back()->SetColMods((rand() % 129), (rand() % 129), (rand() % 129));*/

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


