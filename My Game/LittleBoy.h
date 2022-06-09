#include "Object.h"

class LittleBoy : public CObject 
{
private:
	int nbullets = 0;
public:
	LittleBoy( const Vector2& v ); //Default Constructor
	virtual void move(); //Movement for LittleBoy
	virtual CObject* Attack1(const Vector2& v); //Signature move
	virtual CObject* FireGun(); //Fire regular weapon
};
