#include "PlatformPlayer.h"
#include "EventManager.h"
#include "SoundManager.h"
#include "TextureManager.h"
#include "StateManager.h"
#include <cmath>

PlatformPlayer::PlatformPlayer(SDL_Rect s, SDL_FRect d) :AnimatedSpriteObject(s, d),
m_state(STATE_JUMPING), m_isGrounded(false), m_isFacingLeft(false), m_isDying(false),
m_maxVelX(9.0), m_maxVelY(JUMPFORCE), m_grav(GRAVITY), m_drag(0.85)
{
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
	// SetAnimation(?,?,?,?); // Initialize jump animation. 
	// Idle spriteMin is 0 spriteMax is 1
	// Run spriteMin is 0 max is 8
	// Jump spritemin is 8, max is 9
	SetAnimation(2,8,9,0);
	
}

void PlatformPlayer::Update()
{
	if (timer > 0)
	{
		timer--;
	}

	switch (m_state) // Inside each case is the behaviour in and transitions from that state.
	{
	case STATE_IDLING:
		// Transition to run.
		if (EVMA::KeyPressed(SDL_SCANCODE_A) || EVMA::KeyPressed(SDL_SCANCODE_D))
		{
			m_state = STATE_RUNNING;
			// Idle spriteMin is 0 spriteMax is 1
			// // Run spriteMin is 0 max is 8
			// Jump spritemin is 8, max is 9
			SetAnimation(3, 0, 8, 0);
	
		}
		// Transition to jump.
		else if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_isGrounded)
		{
			// Set accely to negative jump force.
			m_accelY = -JUMPFORCE; // SetAccely(-JumpFORCE)
			m_isGrounded = false;
			// Set state to jumping.
			m_state = STATE_JUMPING;
			SetAnimation(1, 8, 9, 0);
			SOMA::PlaySound("jump");

		}
		else if (EVMA::KeyPressed(SDL_SCANCODE_S) && m_isGrounded)
		{
			m_state = STATE_ROLLING;
			SetAnimation(4, 0, 4, 128);
			SOMA::PlaySound("roll", -1,2);
		}
		else if (EVMA::KeyPressed(SDL_SCANCODE_1))
		{
			m_state = STATE_DEATH;
			SetAnimation(8, 4, 9, 128);
			timer = 44;
		}
		if (IsDying())
		{
			m_state = STATE_DEATH;
			timer = 45;
			SetAnimation(8, 4, 9, 128);

		}
		break;
	case STATE_RUNNING:
		// Move on ground or air 
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			// Set accelX to negative.
			m_accelX = -1.5;
			if (!m_isFacingLeft)
				m_isFacingLeft = true;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			// Set accelX to positive.
			m_accelX = 1.5;
			if (m_isFacingLeft)
				m_isFacingLeft = false;
		}
		// Transition to idle.
		if (!EVMA::KeyHeld(SDL_SCANCODE_A) && !EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			// Set state to idling.
			m_state = STATE_IDLING;

			// SetAnimation(?,?,?,?);
			// because the sprite does not have idle so I changed sprite max from 1 to 2 and frame to 24 to see the change in idle
			SetAnimation(1, 0, 1);
	
		}
		// Transition to jump.
		else if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_isGrounded)
		{
			// Set accely to negative jump force.
			m_accelY = -JUMPFORCE;
			m_isGrounded = false;
			// Set state to jumping.
			m_state = STATE_RUNNING;

			// SetAnimation(?,?,?,?);
			SetAnimation(1, 8, 9, 0);
			SOMA::PlaySound("jump");
		
		}
		if (EVMA::KeyPressed(SDL_SCANCODE_S) && m_isGrounded)
		{
			m_state = STATE_ROLLING;
			SetAnimation(4, 0, 4, 128);
			SOMA::PlaySound("roll", -1, 2);
		}
		break;
		if (IsDying())
		{
			m_state = STATE_DEATH;
			timer = 45;
			SetAnimation(8, 4, 9, 128);

		}
	case STATE_JUMPING:
		// Move in mid-air.
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			// Set accelX to negative.
			m_accelX = -1.5;
			if (!m_isFacingLeft)
				m_isFacingLeft = true;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			// Set accelX to positive.
			m_accelX = 1.5;
			if (m_isFacingLeft)
				m_isFacingLeft = false;
		}
		// Transition to run.
		if (m_isGrounded)
		{
			// Set state to running.
			m_state = STATE_RUNNING;

			// SetAnimation(?,?,?,?);
			SetAnimation(2, 8, 9, 0);
		}
		if (IsDying())
		{
			m_state = STATE_DEATH;
			timer = 45;
			SetAnimation(8, 4, 9, 128);

		}
		break;
	case STATE_ROLLING:
		if (EVMA::KeyReleased(SDL_SCANCODE_S))
		{
			m_state = STATE_IDLING;
			SetAnimation(1, 0, 1);
			SOMA::StopSound(2);
		}
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			// Set accelX to negative.
			m_accelX = -1.5;
			if (!m_isFacingLeft)
				m_isFacingLeft = true;
			if (EVMA::KeyReleased(SDL_SCANCODE_S))
			{
				m_state = STATE_RUNNING;
				SetAnimation(3, 0, 8, 0);
			}
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			// Set accelX to positive.
			m_accelX = 1.5;
			if (m_isFacingLeft)
				m_isFacingLeft = false;
			if (EVMA::KeyReleased(SDL_SCANCODE_S))
			{
				m_state = STATE_RUNNING;
				SetAnimation(3, 0, 8, 0);
			}
		}
		if (IsDying())
		{
			m_state = STATE_DEATH;
			timer = 45;
			SetAnimation(8, 4, 9, 128);

		}
		break;
	case STATE_DEATH:
		
		
		if (timer <= 0)
		{
			STMA::ChangeState(new EndState());
		}
		break;

	}
	// Player movement. Universal for all states. X-axis first.
	m_velX += m_accelX;
	m_velX *= (m_isGrounded ? m_drag : 1.0); // Could replace 1.0 with air friction
	m_velX = std::min(std::max(m_velX, -m_maxVelX), m_maxVelX);
	m_dst.x += static_cast<float>(m_velX);

	// Wrap the player on-screen. For you to do.
	if (m_dst.x >= WIDTH -50 )
	{
		StopX();
		m_dst.x = WIDTH - 50;
	}
	if (m_dst.x <= 0 )
	{
		StopX();
		m_dst.x =0;
	}
	// if m_dst.x >= || m_dst.x <= 
	// return m_dst.x 
	// Y-axis now.
	m_velY += m_accelY + m_grav;
	m_velY = std::min(std::max(m_velY, -m_maxVelY), m_maxVelY);
	m_dst.y += static_cast<float>(m_velY);
	// reset acceleration . Accel is a one shot force  and only used in one frame
	m_accelX = m_accelY = 0.0;

	// Boundary check so we don't fall off the screen
	if (m_dst.y >= HEIGHT - 200)
	{
		StopY();
		m_dst.y = HEIGHT - 200;
		m_isGrounded = true;
	}

	 Animate();

}

void PlatformPlayer::Render()
{
	// To animate the sprite, use SDL_RenderCopyExF() 
	// and you will have to access the TextureManager 
	// and pass in a key such as "player".
	// But for this part, we're just going to use a colored square.
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 0, 255);
	//SDL_RenderFillRectF(Engine::Instance().GetRenderer(), &m_dst);
	// SDL_RenderCopyExF() for animated character. Dont forget facing

	if (m_isFacingLeft)
	{
		SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("player"),
			&m_src, &m_dst, NULL, NULL, SDL_FLIP_HORIZONTAL);
	}
	else
	{
		SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("player"),
			&m_src, &m_dst, NULL, NULL, SDL_FLIP_NONE);
	}

	
}

void PlatformPlayer::Stop()
{
	StopX();
	StopY();
}

void PlatformPlayer::StopX() { m_velX = 0.0; }

void PlatformPlayer::StopY() { m_velY = 0.0; }

void PlatformPlayer::SetAccelX(double a) { m_accelX = a; }

void PlatformPlayer::SetAccelY(double a) { m_accelY = a; }

void PlatformPlayer::SetX(float x) { m_dst.x = x; }

void PlatformPlayer::SetY(float y) { m_dst.y = y; }

void PlatformPlayer::SetGrounded(bool g) { m_isGrounded = g; }

bool PlatformPlayer::IsGrounded() { return m_isGrounded; }

double PlatformPlayer::GetVelX() { return m_velX; }

double PlatformPlayer::GetVelY() { return m_velY; }

PlayerState PlatformPlayer::GetState()
{
	return m_state;
}

bool PlatformPlayer::IsDying()
{
	return m_isDying;
}

void PlatformPlayer::SetState(PlayerState state)
{
	m_state = state;
}

void PlatformPlayer::Dying(bool dying)
{
	m_isDying = dying;
}
