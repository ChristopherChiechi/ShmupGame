#include "Enemy.h"
#include "Sndlist.h"
#include "Helpers.h"
#include "Abort.h"

// constructor for CEenemyObject
CEnemyObject::CEnemyObject(const Vector2& pos, char color, int path )//: CObject(BLUE_LIGHT_ENEMY, pos)
{
	m_vPos = pos;
	m_fHealth = 3;
	path_key = path;
	SetSpeed(50.0f);
	orig_speed = m_fSpeed;

	if (color == 'r') //Checks for which color enemy is desired and will assign the appropraite sprite
	{
		m_nSpriteIndex = RED_LIGHT_ENEMY;
		SetSpeed(100.0f);
	}
	else if (color == 'b')
	{
		m_nSpriteIndex = BLUE_LIGHT_ENEMY;
		SetSpeed(100.0f);
	}
	else if (color == 'x')
	{
		m_nSpriteIndex = RED_HEAVY_ENEMY;
		m_fHealth = 6;
	}
	else if (color == 'y')
	{
		m_nSpriteIndex = BLUE_HEAVY_ENEMY;
		m_fHealth = 6;
	}
	else if (color == 'i')
	{
		m_nSpriteIndex = RED_LINE;
	}
	else // (color == 'j')
	{
		m_nSpriteIndex = BLUE_LINE;
	}

	m_pRenderer->GetSize(m_nSpriteIndex, m_vRadius.x, m_vRadius.y);
	m_vRadius *= 0.5f;
	m_Sphere.Radius = max(m_vRadius.x, m_vRadius.y);
	m_Sphere.Center = (Vector3)pos;
	m_fGunTimer = m_pStepTimer->GetTotalSeconds();
}

void CEnemyObject::move() //Enemy is moving, selecting tis path
{
	switch (path_key) 
	{
	case 1:
		Path_1();
		break;
	case 2:
		Path_2();
		break;
	case 3:
		Path_3();
		break;
	case 4:
		Path_4();
		break;
	case 5:
		Path_5();
		break;
	case 6:
		Path_6();
		break;
	case 7:
		Path_7();
		break;
	case 8:
		Path_8();
		break;
	case 9:
		Path_9();
		break;
	case 10:
		Path_10();
		break;
	default:
		break;
	}

	// Object animation. From Ned's Turkey Farm
	const size_t nFrameCount = m_pRenderer->GetNumFrames(m_nSpriteIndex); // nFrameCount = number of sprite's frames
	const float dt = 1000.0f * m_fFrameInterval / (1500.0f + fabsf(m_vVel.x));    // calculates animation speed

	// calculates current frame
	if (nFrameCount > 1 && m_pStepTimer->GetTotalSeconds() > m_fFrameTimer + dt) {
		m_fFrameTimer = m_pStepTimer->GetTotalSeconds();
		m_nCurrentFrame = (m_nCurrentFrame + 1) % nFrameCount;
	}
}

CObject* CEnemyObject::FireGun() //Enemy is firing its gun
{
	m_pAudio->play(ENEMYGUN_SOUND);
	const Vector2 pos = GetPos() - 0.5f * m_pRenderer->GetWidth(m_nSpriteIndex) * GetViewVector();
	CObject* enemy_bullet;
	if (m_nSpriteIndex == BLUE_LIGHT_ENEMY)
		enemy_bullet = new CObject(BLUE_BULLET, pos);
	else if (m_nSpriteIndex == RED_LIGHT_ENEMY)
		enemy_bullet = new CObject(RED_BULLET, pos);
	else if (m_nSpriteIndex == RED_HEAVY_ENEMY)
		enemy_bullet = new CObject(RED_BULLET, pos);
	else //(m_nSpriteIndex == BLUE_HEAVY_ENEMY)
		enemy_bullet = new CObject(BLUE_BULLET, pos);

	const Vector2 aim = m_pPlayer->GetPos() - GetPos();

	enemy_bullet->SetVelocity((Vector2(0.0f, -700.0f) + aim) * .25f);
	//enemy_bullet->SetVelocity((Vector2(0.0f, -1000.0f)) * .25f);
	enemy_bullet->SetOrientation(GetOrientation());
	return enemy_bullet;
}

void CEnemyObject::Path_1() // Moved down and stops at y coordinate 512
{
	m_vOldPos = m_vPos;
	if (m_vPos.y <= 512.0f)
		m_fSpeed = 0.0f;
	const float time = m_pStepTimer->GetElapsedSeconds();
	const Vector2 front = GetViewVector();
	const float displacement = m_fSpeed * time;
	m_vPos -= displacement * front;
	m_Sphere.Center = (Vector3)m_vPos;
}

void CEnemyObject::Path_2() //Flight path 2, descend on the right side, and then shift left
{
	m_vOldPos = m_vPos;
	const float time = m_pStepTimer->GetElapsedSeconds();
	const Vector2 front = GetViewVector();
	const float displacement = m_fSpeed * time;
	const Vector2 side_velocity = Vector2(front.y, -front.x);

	if (m_vPos.y <= 500.0f || m_vPos.x >= 750.0f)
		m_vPos -= side_velocity * displacement;
	else if (m_vPos.x <= 100.0f)
		m_vPos += side_velocity * displacement;
	else
		m_vPos -= displacement * front;

	m_Sphere.Center = (Vector3)m_vPos;
}

// descends to top of screen, then moves diagonal down and left
void CEnemyObject::Path_3()
{
	m_vOldPos = m_vPos;
	const float time = m_pStepTimer->GetElapsedSeconds();
	const Vector2 front = GetViewVector();
	const float displacement = m_fSpeed * time;
	const Vector2 side_velocity = Vector2(front.y, -front.x);

	// if y position is greater than worldsize.y
	if (m_vPos.y >= 1024.0f)
	{
		m_vPos.y -= displacement;	// move down
	}
	else
	{
		m_vPos.x -= displacement;	// left
		m_vPos.y -= displacement;	// move down
	}
	
	m_Sphere.Center = (Vector3)m_vPos;	// update hitbox
}

// descends to top of screen, then moves diagonal down and right
void CEnemyObject::Path_4()
{
	m_vOldPos = m_vPos;
	const float time = m_pStepTimer->GetElapsedSeconds();
	const Vector2 front = GetViewVector();
	const float displacement = m_fSpeed * time;
	const Vector2 side_velocity = Vector2(front.y, -front.x);

	// if y position is greater than worldsize.y
	if (m_vPos.y >= 1024.0f)
	{
		m_vPos.y -= displacement;	// move down
	}
	else
	{
		m_vPos.x += displacement;	// move right
		m_vPos.y -= displacement;	// move down
	}

	m_Sphere.Center = (Vector3)m_vPos;	// update hitbox
}

void CEnemyObject::Path_5() //Flight path 5, descend and then shift right
{
	m_vOldPos = m_vPos;
	const float time = m_pStepTimer->GetElapsedSeconds();
	const Vector2 front = GetViewVector();
	const float displacement = m_fSpeed * time;
	const Vector2 side_velocity = Vector2(front.y, -front.x);

	// if y position is greater than worldsize.y
	if ((m_vPos.y >= 1024.0f) || (m_vPos.y >= 400.0f))
	{
		m_vPos.y -= displacement;	// move down
	}
	else
	{
		m_vPos.x += displacement;	// move right
	}

	m_Sphere.Center = (Vector3)m_vPos;
}
void CEnemyObject::Path_6() //Flight path 6, descend and then shift left
{
	m_vOldPos = m_vPos;
	const float time = m_pStepTimer->GetElapsedSeconds();
	const Vector2 front = GetViewVector();
	const float displacement = m_fSpeed * time;
	const Vector2 side_velocity = Vector2(front.y, -front.x);

	// if y position is greater than worldsize.y
	if ((m_vPos.y >= 1024.0f) || (m_vPos.y >= 400.0f))
	{
		m_vPos.y -= displacement;	// move down
	}
	else
	{
		m_vPos.x -= displacement;	// move right
	}

	m_Sphere.Center = (Vector3)m_vPos;
}
void CEnemyObject::Path_7() //Flight path 7. zig zag horizontal right
{
	m_vOldPos = m_vPos;
	const float time = m_pStepTimer->GetElapsedSeconds();
	const Vector2 front = GetViewVector();
	const float displacement = m_fSpeed * time;
	const Vector2 side_velocity = Vector2(front.y, -front.x);

	// if y position is greater than worldsize.y
	if (m_vPos.y >= 1024.0f)
	{
		m_vPos.y -= displacement;	// move down
	}
	else if (m_vPos.y >= 824.0f)
	{
		m_vPos.y -= displacement;	// move down
		m_vPos.x += displacement * 2;	// move right
	}
	else if (m_vPos.y >= 624.0f)
	{
		m_vPos.y -= displacement;	// move down
		m_vPos.x -= displacement *2;	// move left
	}
	else if (m_vPos.y >= 424.0f)
	{
		m_vPos.y -= displacement;	// move down
		m_vPos.x += displacement *2;	// move right
	}
	else if (m_vPos.y >= 224.0f)
	{
		m_vPos.y -= displacement;	// move down
		m_vPos.x -= displacement*2;	// move left
	}
	else
	{
		m_vPos.y -= displacement;	// move down
	}
	m_Sphere.Center = (Vector3)m_vPos;	// update hitbox
}

// Constantly move up and down. Meant for RED_LINE and BLUE_LINE
void CEnemyObject::Path_8()
{
	m_vOldPos = m_vPos;
	const float time = m_pStepTimer->GetElapsedSeconds();
	const float displacement = m_fSpeed * time;

	if (switchMovement == false)
	{
		m_vPos.y -= displacement * 3;	// move down
	}
	if (switchMovement == true)
	{
		m_vPos.y += displacement * 3;	// move up
	}

	// If enemy hits top or bottom of world, switch direction
	if (m_vPos.y >= m_vWorldSize.y)
	{
		switchMovement = false;
	}
	if (m_vPos.y <= 0)
	{
		switchMovement = true;
	}

	// if enemy is RED_LINE and player is BLUE_SHIP, and the player's y position is within the enemies y position
	if (m_nSpriteIndex == RED_LINE && m_pPlayer->m_nSpriteIndex == BLUE_SHIP)
	{
		if (m_pPlayer->m_vPos.y > m_vPos.y - 15 && m_pPlayer->m_vPos.y < m_vPos.y + 15)
		{
			hit();	// hit player
		}
	}

	// if enemy is BLUE_LINE and player is RED_SHIP, and the player's y position is within the enemies y position
	if (m_nSpriteIndex == BLUE_LINE && m_pPlayer->m_nSpriteIndex == RED_SHIP)
	{
		if (m_pPlayer->m_vPos.y > m_vPos.y - 15 && m_pPlayer->m_vPos.y < m_vPos.y + 15)
		{
			hit();	// hit player
		}
	}
}

// Quickly move down. Meant for RED_LINE and BLUE_LINE
void CEnemyObject::Path_9()
{
	m_vOldPos = m_vPos;
	const float time = m_pStepTimer->GetElapsedSeconds();
	const float displacement = m_fSpeed * time;

	m_vPos.y -= displacement * 6;	// move down quickly

	// if enemy is RED_LINE and player is BLUE_SHIP, and the player's y position is within the enemies y position
	if (m_nSpriteIndex == RED_LINE && m_pPlayer->m_nSpriteIndex == BLUE_SHIP)
	{
		if (m_pPlayer->m_vPos.y > m_vPos.y - 15 && m_pPlayer->m_vPos.y < m_vPos.y + 15)
		{
			hit();	// hit player
		}
	}

	// if enemy is BLUE_LINE and player is RED_SHIP, and the player's y position is within the enemies y position
	if (m_nSpriteIndex == BLUE_LINE && m_pPlayer->m_nSpriteIndex == RED_SHIP)
	{
		if (m_pPlayer->m_vPos.y > m_vPos.y - 15 && m_pPlayer->m_vPos.y < m_vPos.y + 15)
		{
			hit();	// hit player
		}
	}
}

// Moves down
void CEnemyObject::Path_10()
{
	m_vOldPos = m_vPos;
	const float time = m_pStepTimer->GetElapsedSeconds();
	const float displacement = m_fSpeed * time;

	m_vPos.y -= displacement;	// move down

	m_Sphere.Center = (Vector3)m_vPos;	// update hitbox
}