#ifndef INTERFACES_H
#define INTERFACES_H

// year 2 all over again with the design patterns
class IRenderable {
public:
	virtual void Render() = 0;
};

class IUpdateable {
public:
	virtual void Update() = 0;
};

class IResource {
public:
	virtual void Release() = 0;
};

#endif