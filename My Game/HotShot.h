#include "Object.h"

class HotShot : public CObject
{
private:
public:
	HotShot(const  Vector2& loc); // Default Constructer
	virtual void move(); // Movement for Hotshot
	virtual CObject* FireGun(); //Normal attack
	virtual CObject* Attack1( const Vector2& loc ); //Signature move
};

