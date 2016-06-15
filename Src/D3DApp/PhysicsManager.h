#pragma once
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "D3DUtils.h"
#define PLANE PhysicsManager::Physics()->GetPlaneShape()
#define CAPSULE PhysicsManager::Physics()->GetCapsuleShape()
#define CONE PhysicsManager::Physics()->GetConeShape()
#define CYLINDER PhysicsManager::Physics()->GetCylinderShape()
#define BOX PhysicsManager::Physics()->GetBoxShape()
#define SPHERE PhysicsManager::Physics()->GetShpereShape()
#define WORLD PhysicsManager::Physics()->GetWorld()

#define btAssert(x) { if(!(x)){printf("Assert " __FILE__  ":%u ("#x")\n", __LINE__);__debugbreak();	}}

class PhysicsManager
{
public:

	static PhysicsManager* Physics()
	{
		if (instance == nullptr)
		{
			instance = new PhysicsManager();
		}
		return instance;
	}

	void Step(float deltaTime);
	void CreateEmptyWorld();
	virtual void exitPhysics();



	btDiscreteDynamicsWorld* GetWorld() { return world; }
	btCollisionShape* GetPlaneShape() { return planeShape; }
	btCollisionShape* GetCapsuleShape() { return capsuleShape; }
	btCollisionShape* GetConeShape() { return coneShape; }
	btCollisionShape* GetCylinderShape() { return cylinderShape; }
	btCollisionShape* GetBoxShape() { return boxShape; }
	btCollisionShape* GetShpereShape() { return sphereShape; }


	~PhysicsManager();
private:
	PhysicsManager();
	static PhysicsManager* instance;

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	btDiscreteDynamicsWorld* world;

	btCollisionShape* planeShape;
	btCollisionShape* capsuleShape;
	btCollisionShape* coneShape;
	btCollisionShape* cylinderShape;
	btCollisionShape* boxShape;
	btCollisionShape* sphereShape;
};

