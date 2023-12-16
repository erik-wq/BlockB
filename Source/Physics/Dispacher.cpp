#include "Physics/Dispacher.h"
#include "Objects/BaseObject.h"

Dispacher::Dispacher(btCollisionConfiguration* config) : btCollisionDispatcher(config)
{
}

Dispacher::~Dispacher()
{
}

void Dispacher::CheckEvents()
{
	CollisionPairs pairs;

	for (int i = 0; i < getNumManifolds(); ++i)
	{
		btPersistentManifold* pManifold = getManifoldByIndexInternal(i);

		if (pManifold->getNumContacts() > 0) {
			const btCollisionObject* pBody0 = pManifold->getBody0();
			const btCollisionObject* pBody1 = pManifold->getBody1();

			bool const swapped = pBody0 > pBody1;
			const btCollisionObject* pSortedBodyA = swapped ? pBody1 : pBody0;
			const btCollisionObject* pSortedBodyB = swapped ? pBody0 : pBody1;

			Pair thisPair = std::make_pair(pSortedBodyA, pSortedBodyB);

			pairs.insert(thisPair);

			if (collisionPairs.find(thisPair) == collisionPairs.end()) {
				ResolveEvents(pBody0, pBody1, true);
			}
		}
	}

	CollisionPairs removedPairs;

	std::set_difference(collisionPairs.begin(), collisionPairs.end(),
		pairs.begin(), pairs.end(),
		std::inserter(removedPairs, removedPairs.begin()));

	for (CollisionPairs::const_iterator iter = removedPairs.begin(); iter != removedPairs.end(); ++iter) {
		ResolveEvents(iter->first, iter->second, false);
	}

	collisionPairs = pairs;
}

void Dispacher::ResolveEvents(const btCollisionObject* pBody0, const btCollisionObject* pBody1, bool start)
{
	BaseObject* object0 =  (BaseObject*)pBody0->getUserPointer();
	BaseObject* object1 =  (BaseObject*)pBody1->getUserPointer();
	if (!object0 || !object1) return;

	int flags0 = pBody0->getCollisionFlags();
	int flags1 = pBody1->getCollisionFlags();

	bool trigger = (flags0 & btCollisionObject::CF_NO_CONTACT_RESPONSE) | 
				   (flags1 & btCollisionObject::CF_NO_CONTACT_RESPONSE);
	if (trigger)
	{
		start ? OverlapEnter(object0, object1) : OverlapExit(object0, object1);
		return;
	}

	start ? CollisionEnter(object0, object1) : CollisionExit(object0, object1);
}

void Dispacher::CollisionEnter(BaseObject* object0, BaseObject* object1)
{
	object0->OnCollisionEnter(object1);
	object1->OnCollisionEnter(object0);
}

void Dispacher::CollisionExit(BaseObject* object0, BaseObject* object1)
{
	object0->OnCollisionExit(object1);
	object1->OnCollisionExit(object0);
}

void Dispacher::OverlapEnter(BaseObject* object0, BaseObject* object1)
{
	object0->OnOverlapEnter(object1);
	object1->OnOverlapEnter(object0);
}

void Dispacher::OverlapExit(BaseObject* object0, BaseObject* object1)
{
	object0->OnOverlapExit(object1);
	object1->OnOverlapExit(object0);
}

