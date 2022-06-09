#include "BlackJack.h"

BlackJack::BlackJack(const Vector2& v) //Default constructor
{
	m_fHealth = 30;
	m_nSpriteIndex = BLACK_JACK;
	m_vPos = v;

	m_pRenderer->GetSize(m_nSpriteIndex, m_vRadius.x, m_vRadius.y);
	m_vRadius *= 0.5f;

	const Vector2 topleft(-m_vRadius.x, m_vRadius.y);
	const Vector2 bottomrt(m_vRadius.x, -m_vRadius.y);

	m_Sphere.Radius = max(m_vRadius.x, m_vRadius.y);
	m_Sphere.Center = (Vector3)m_vPos;

	m_fCardTimer = m_fBlackHoleTimer = m_fForceFieldTimer = m_fChangeColorTimer = m_fGunTimer = m_pStepTimer->GetTotalSeconds();

}

void BlackJack::move() // Movement for BlackJack
{
	if (m_vPos.x <= 90) { //Black needs to respawn back inside if he is outside of the screen
		Respawn(true);
		return;
	}
	if (m_vPos.x >= 900) {
		Respawn(false);
		return;
	}
	int choose_speed = rand() % 500; //randomly chooses blackjacks speed whe he dodges bullet
	if (choose_speed < 105)
		SetSpeed(1500.0f);
	else
		SetSpeed(600.0f);

	m_vOldPos = m_vPos;
	const Vector2 front = GetViewVector(); //Blackjacks front view
	const Vector2 side = Vector2(front.y, -front.x); //velocity going side to side
	const float time = m_pStepTimer->GetElapsedSeconds(); //how much time has passed
	const float displacement = m_fSpeed * time; //distance covered is speed times the time taken

	//The force field will move along with blackjakc if it is activated
	if (m_bStrafeBack) { //Black is moving downwards, he is charging at the player
		m_vPos -= displacement * front;
		if (force_field) 
			force_field->m_vPos -= displacement * front;
	}
	else if (m_bStrafeLeft) { //Blackjack is moving left
		m_vPos -= displacement * side;
		if (force_field)
			force_field->m_vPos -= displacement * side;
	}
	else if (m_bStrafeRight) { //Blackjack is moving right
		m_vPos += displacement * side;
		if (force_field)
			force_field->m_vPos += displacement * side;
	}
	else if (m_bStrafeFoward) { //Blackjack is moving upwards until he is back at initial position before charging
		if (GetPos().y >= 600.0f) 
			m_bStrafeFoward = false;
		else {
			m_vPos += displacement * front;
			if (force_field)
				force_field->m_vPos += displacement * front;
		}
	}
	if (charging) { //If blackjack goes down low enough, he will go bakc to original position
		if (GetPos().y <= 200.0f) {
			m_bStrafeFoward = true;
			charging = false;
		}
	}
	if (!charging)//Make sure he is not moving back down if not charging
		m_bStrafeBack = false;

	m_Sphere.Center = (Vector3)m_vPos; //maintian position of blackjack
	if(ff_on)
		force_field->UpdatePos(); //updates m_sphere.center for the forcefield if active
	if (!charging && !m_bStrafeBack && !m_bStrafeFoward && !ff_on) { //black is only dodging an attack
		SetSpeed(0.0f);
		m_bStrafeBack = m_bStrafeLeft = m_bStrafeRight = false;
	}
}


CObject* BlackJack::FireGun() { //Black shoots his gun, he shoots up to 2 bullets
	CObject* bullet;
	Vector2 pos;
	if (nbullets == 0) {
		pos = GetPos() - 0.5f * m_pRenderer->GetWidth(m_nSpriteIndex) * GetViewVector();
		pos.x = pos.x - 30.0f;
	}
	else if (nbullets == 1) {
		pos = GetPos() - 0.5f * m_pRenderer->GetWidth(m_nSpriteIndex) * GetViewVector();
		pos.x = pos.x + 30.0f;
	}

	int colorb = rand() % 10; //randomly choose colors of the bullet
	if (colorb <= 5)
		bullet = new CObject(RED_BULLET, pos);
	else
		bullet = new CObject(BLUE_BULLET, pos);

	bullet->SetVelocity((Vector2(0.0f, -700.0f) + (m_pPlayer->GetPos() - GetPos())) * .25);

	if (nbullets == 1)
		nbullets = 0;
	else
		nbullets++;

	return bullet;
}

void BlackJack::Respawn(bool b) //Blackjack respawns base on where he is on the screen, to make sure he does not leave the screen
{
	Vector2 newPos;
	if (b) { //If he is too far left, repsawn him to the right
		newPos = Vector2(750.0f, 550.0f);
		m_pAudio->play(RESPAWN_SOUND);
		//spawn portal sprite
		CParticleDesc2D spawn;
		spawn.m_nSpriteIndex = LARGE_RESPAWN;
		spawn.m_vPos = newPos;
		spawn.m_fLifeSpan = 0.8f;
		spawn.m_fMaxScale = 0.8f;
		spawn.m_fScaleInFrac = 0.2f;
		spawn.m_fFadeOutFrac = 0.8f;
		spawn.m_fFadeInFrac = 0.8f;
		spawn.m_fScaleOutFrac = 0.2f;
		m_vPos = newPos;
		UpdatePos();
		m_pParticleEngine->create(spawn);
		if (force_field) {
			force_field->m_vPos = newPos;
			force_field->UpdatePos();
		}
	}
	else {//Respawn to the left
		newPos = Vector2(200.0f, 550.0f);
		m_pAudio->play(RESPAWN_SOUND);
		//Spawn portal sprite
		CParticleDesc2D spawn;
		spawn.m_nSpriteIndex = LARGE_RESPAWN;
		spawn.m_vPos = newPos;
		spawn.m_fLifeSpan = 0.8f;
		spawn.m_fMaxScale = 0.8f;
		spawn.m_fScaleInFrac = 0.2f;
		spawn.m_fFadeOutFrac = 0.8f;
		spawn.m_fFadeInFrac = 0.8f;
		spawn.m_fScaleOutFrac = 0.2f;
		m_vPos = newPos;
		UpdatePos();
		m_pParticleEngine->create(spawn);
		if (force_field) {
			force_field->m_vPos = newPos;
			force_field->UpdatePos();
		}
	}
}
