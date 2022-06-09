#include "Object.h"

class BlackJack : public CObject 
{
private:
	int nbullets = 0;
	public:
		BlackJack(const Vector2& v);
		virtual void move(); //Blackjack is moving
		virtual CObject* FireGun(); //BlackJack is shooting
		void Respawn(bool b); //BlackJack repositions
};
