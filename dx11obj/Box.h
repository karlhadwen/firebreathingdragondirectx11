#ifndef BOX_H
#define BOX_H

#include "OBJEntity.h"

/**
* test entity to see movement of objects
*/
class Box : public OBJEntity {
public:
	Box() : OBJEntity(L"Media\\Box\\box.obj") {
	}

	void Update() {
		this->Entity::Update();
	}
};

#endif // !BOX_H
