#pragma once
#include "btBulletDynamicsCommon.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btAlignedObjectArray.h"
#include "BulletCollision/CollisionShapes/btBox2dShape.h"
#include "BulletCollision/CollisionDispatch/btEmptyCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btBox2dBox2dCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btConvex2dConvex2dAlgorithm.h"
#include "BulletCollision/CollisionShapes/btBox2dShape.h"
#include "BulletCollision/CollisionShapes/btConvex2dShape.h"
#include "BulletCollision/NarrowPhaseCollision/btMinkowskiPenetrationDepthSolver.h"
#include "D3DUtils.h"

#define CAPSULE PhysicsManager::Physics()->GetCapsuleShape()
#define CONE PhysicsManager::Physics()->GetConeShape()
#define CYLINDER PhysicsManager::Physics()->GetCylinderShape()
#define BOX PhysicsManager::Physics()->GetBoxShape()
#define SPHERE PhysicsManager::Physics()->GetShpereShape()
#define WORLD PhysicsManager::Physics()->GetWorld()

#define btAssert(x) { if(!(x)){printf("Assert " __FILE__  ":%u ("#x")\n", __LINE__);__debugbreak();	}}
#define SCALING 1

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
	btCollisionShape* GetCapsuleShape() { return capsuleShape; }
	btCollisionShape* GetConeShape() { return coneShape; }
	btCollisionShape* GetCylinderShape() { return cylinderShape; }
	btCollisionShape* GetBoxShape() { return boxShape; }
	btCollisionShape* GetShpereShape() { return sphereShape; }


	~PhysicsManager();
private:
	PhysicsManager();
	static PhysicsManager* instance;

	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
	btConvex2dConvex2dAlgorithm::CreateFunc*	m_convexAlgo2d;
	btBox2dBox2dCollisionAlgorithm::CreateFunc* m_box2dbox2dAlgo;
	btVoronoiSimplexSolver* m_simplexSolver;
	btMinkowskiPenetrationDepthSolver* m_pdSolver;

	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;

	btDiscreteDynamicsWorld* world;

	btCollisionShape* capsuleShape;
	btCollisionShape* coneShape;
	btCollisionShape* cylinderShape;
	btCollisionShape* boxShape;
	btCollisionShape* sphereShape;

	btConvexShape* childShape0;
	btConvexShape* box2DShape;
	btConvexShape* childShape1;
	btConvexShape* convexHull2DShape;
	btConvexShape* childShape2;
	btConvexShape* cylinder2DShape;
};

