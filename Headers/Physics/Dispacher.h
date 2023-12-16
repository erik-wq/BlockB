#pragma once
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <iterator>

class BaseObject;

typedef std::pair<const btCollisionObject*, const btCollisionObject*> Pair;
typedef std::set<Pair> CollisionPairs;

class Dispacher : public btCollisionDispatcher 
{
public:
	Dispacher(btCollisionConfiguration* config);
	~Dispacher();

	void CheckEvents();
private:
	void ResolveEvents(const btCollisionObject* pBody0,const btCollisionObject* pBody1, bool start);
	void CollisionEnter(BaseObject*, BaseObject*);
	void CollisionExit(BaseObject*, BaseObject*);
	void OverlapEnter(BaseObject*, BaseObject*);
	void OverlapExit(BaseObject*, BaseObject*);

	CollisionPairs collisionPairs;
};