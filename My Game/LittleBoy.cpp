#include "LittleBoy.h"

LittleBoy::LittleBoy( const Vector2& v ) //Default Constructor
{
	m_fHealth = 10;
	m_nSpriteIndex = LILBOY;
	m_vPos = v;
	SetSpeed(400.0f);
	m_pRenderer->GetSize(m_nSpriteIndex, m_vRadius.x, m_vRadius.y);
	m_vRadius *= 0.5f;

	const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);

	m_Sphere.Radius = max(m_vRadius.x, m_vRadius.y);
	m_Sphere.Center = (Vector3)m_vPos;

	m_fGunTimer = m_pStepTimer->GetTotalSeconds();
	m_bStrafeBack = true;
	m_bStrafeRight = m_bStrafeLeft = false;
}


void LittleBoy::move() //Movement for LittleBoy
{
	// if boss is not in world, slowly descend at the speed of heavy enemies
	if (m_vPos.y > m_vWorldSize.y)
	{
		SetSpeed(50.0f);
	}
	// else change speed
	else
	{
		int speed_manip = rand() % 20;
		if (speed_manip < 3)
			SetSpeed(50.0f);
		else
			SetSpeed(400.0f);
	}

	m_vOldPos = m_vPos;
	const Vector2 pos = GetPos();
	const Vector2 front = GetViewVector();
	const float time = m_pStepTimer->GetElapsedSeconds();
	const Vector2 side_velocity = Vector2(front.y, -front.x);
	const float displacement = m_fSpeed * time;
	//Littleboy quickly moves back and forth nonstop
	if (m_bStrafeBack && pos.y <= 600) 
	{
		m_bStrafeLeft = true;
		m_bStrafeRight = m_bStrafeBack = false;
	}
	else if (m_bStrafeRight && pos.x >= 800)
	{
		m_bStrafeLeft = true;
		m_bStrafeRight = m_bStrafeBack = false;
	}
	else if (m_bStrafeLeft && pos.x <= 100) 
	{
		m_bStrafeRight = true;
		m_bStrafeLeft = m_bStrafeBack = false;
	}
	else {}

	if (m_bStrafeBack)
		m_vPos -= displacement * front;
	else if (m_bStrafeRight)
		m_vPos += displacement * side_velocity;
	else if (m_bStrafeLeft)
		m_vPos -= displacement * side_velocity;

	m_Sphere.Center = (Vector3)m_vPos;
}

CObject* LittleBoy::Attack1(const Vector2& v) //Signature move, LILBOMB
{
	Vector2 pos = GetPos() - 0.5f * GetViewVector() * m_pRenderer->GetWidth(m_nSpriteIndex);
	CObject* bomb = new CObject(LILBOMB, pos);
	bomb->SetVelocity(Vector2(0.0f, -200.0f));
	bomb->SetOrientation(360);
	return bomb;
}


CObject* LittleBoy::FireGun() //Littleboy fires gun
{
	int n1 = rand() % 10; //Number to determine color of bullets
	Vector2 pos = GetPos() - 0.5f * GetViewVector() * m_pRenderer->GetWidth(m_nSpriteIndex);
	CObject* bullet1;
	if (n1 < 5) //If n1 is less than 5 it is a blue bullet. It will be a red bullet otherwise.
		bullet1 = new CObject(BLUE_BULLET, pos);
	else
		bullet1 = new CObject(RED_BULLET,pos);

	Vector2 aim = m_pPlayer->GetPos() - GetPos();
	bullet1->SetVelocity((Vector2(0.0f, -700.0f) + aim) * .25);
	switch (nbullets) //Makes sure all 3 bullets dont spawn at the same place
	{
		case 1:
		{
			bullet1->SetVelocity(Vector2(bullet1->GetVelocity().x + 50.0f, bullet1->GetVelocity().y ));
			nbullets++;
		}
		break;
		case 2:
		{
			bullet1->SetVelocity(Vector2(bullet1->GetVelocity().x - 50.0f, bullet1->GetVelocity().y));
			nbullets = 0;
		}
		break;
		default:
			nbullets++;
		break;
	}
	return bullet1;
}