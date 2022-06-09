#pragma once

#include "Object.h"

class CEnemyObject : public CObject
{
private:
	int path_key = -1;
	bool switchMovement = false;
public:
	CEnemyObject(const Vector2& pos, char color, int path ); // constructor
	virtual void move(); // move enemy
	virtual CObject* FireGun(); //The enemy shoots its gun
	void Path_1(); // Flight path for enemy. Down and stop
	void Path_2(); //Flight path 2. Down, stop, and left
	void Path_3();	// diagonal down and left
	void Path_4();	// diagonal down and right
	void Path_5();	//Flight path 5. Down, stop, and right
	void Path_6();	//Flight path 6. Down, stop, and left
	void Path_7();	//Flight path 7. zig zag horizontal right
	void Path_8();	// Constantly move up and down. Meant for RED_LINE and BLUE_LINE
	void Path_9();	// Quickly move down. Meant for RED_LINE and BLUE_LINE
	void Path_10(); // Moves down
};
