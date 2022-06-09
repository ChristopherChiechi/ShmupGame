#include "HotShot.h"

HotShot::HotShot(const Vector2& loc) // Default Constructor
{
	m_fHealth = 24;
	m_nSpriteIndex = HOTSHOT;
	m_vPos = loc;

	m_pRenderer->GetSize(m_nSpriteIndex, m_vRadius.x, m_vRadius.y);
	m_vRadius *= 0.5f;

	m_Sphere.Radius = max(m_vRadius.x, m_vRadius.y);
	m_Sphere.Center = (Vector3)m_vPos;

	m_fGunTimer = m_pStepTimer->GetTotalSeconds();

	SetSpeed(80.0f);

	m_bStrafeLeft = m_bStrafeRight = false;
	m_bStrafeBack = true;
}

void HotShot::move()  //HotShot Moving
{
	const float time = m_pStepTimer->GetElapsedSeconds();
	const Vector2 front = GetViewVector();
	const Vector2 normal_velocity(front.y, -front.x);
	const float displacement = m_fSpeed * time;
	const Vector2 pos = GetPos();
	//Hotshot just moves slowly form left to right
	if (m_bStrafeBack && pos.y <= 600.0f)
	{
		m_bStrafeLeft = true;
		m_bStrafeBack = m_bStrafeRight = false;
	}
	else if (m_bStrafeLeft && pos.x <= 100.0f)
	{
		m_bStrafeRight = true;
		m_bStrafeBack = m_bStrafeLeft = false;
	}
	else if (m_bStrafeRight && pos.x >= 800.0f) 
	{
		m_bStrafeLeft = true;
		m_bStrafeBack = m_bStrafeRight = false;
	}
	else {}

	if (m_bStrafeBack)
		m_vPos -= displacement * front;
	else if (m_bStrafeLeft)
		m_vPos -= displacement * normal_velocity;
	else if (m_bStrafeRight)
		m_vPos += displacement * normal_velocity;

	m_Sphere.Center = (Vector3)m_vPos;

	// Object animation. From Ned's Turkey Farm
	const size_t nFrameCount = m_pRenderer->GetNumFrames(m_nSpriteIndex); // nFrameCount = number of sprite's frames
	const float dt = 1000.0f * m_fFrameInterval / (1500.0f + fabsf(m_vVel.x));    // calculates animation speed

	// calculates current frame
	if (nFrameCount > 1 && m_pStepTimer->GetTotalSeconds() > m_fFrameTimer + dt) {
		m_fFrameTimer = m_pStepTimer->GetTotalSeconds();
		m_nCurrentFrame = (m_nCurrentFrame + 1) % nFrameCount;
	}
}


CObject* HotShot::Attack1(const Vector2& loc) //Signature move, returns his firetrap
{
	m_pAudio->play(FIRETRAP_SOUND);
	int which = rand() % 100;
	CObject* trap;
	trap = new CObject(REDFIRE, loc);
	return trap;
}

CObject* HotShot::FireGun()  //Normal attack, returns his fireball
{
	m_pAudio->play(FIREBALL_SOUND);
	const Vector2 pos = GetPos() - 0.5f * m_pRenderer->GetWidth(m_nSpriteIndex) * GetViewVector();
	
	CObject* fire_ball = new CObject(FIREBALL, pos);
	
	const Vector2 aim = m_pPlayer->GetPos() - GetPos();
	fire_ball->SetVelocity((Vector2(0.0f, -220.0f) + aim) * .50f);
	fire_ball->SetOrientation(GetOrientation());
	return fire_ball;
}
