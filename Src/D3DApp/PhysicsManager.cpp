#include "PhysicsManager.h"

PhysicsManager* PhysicsManager::instance = 0;

PhysicsManager::PhysicsManager()
	: broadphase(nullptr), collisionConfiguration(nullptr)
	, dispatcher(nullptr), solver(nullptr), world(nullptr)
	, capsuleShape(nullptr)
	, coneShape(nullptr), cylinderShape(nullptr)
	, boxShape(nullptr), sphereShape(nullptr)
{

}

void PhysicsManager::Step(float deltaTime)
{
	world->stepSimulation(deltaTime);
}

void PhysicsManager::CreateEmptyWorld()
{
	///collision configuration contains default setup for memory, collision setup
	collisionConfiguration = new btDefaultCollisionConfiguration();


	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	broadphase = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	solver = sol;

	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	world->setGravity(btVector3(0, 10, 0));

	boxShape = new btBoxShape(btVector3(1, 1, 1));
	sphereShape = new btSphereShape(10);
	capsuleShape = new btCapsuleShape(1, 2);
	coneShape = new btConeShape(1, 2);
	cylinderShape = new btCylinderShape(btVector3(1, 1, 1));

	m_collisionShapes.push_back(boxShape);
	m_collisionShapes.push_back(sphereShape);
	m_collisionShapes.push_back(capsuleShape);
	m_collisionShapes.push_back(coneShape);
	m_collisionShapes.push_back(cylinderShape);


	btScalar u = btScalar(1 * SCALING - 0.04);
	btVector3 points[3] = { btVector3(0,u,0),btVector3(-u,-u,0),btVector3(u,-u,0) };
	childShape0 = new btBoxShape(btVector3(btScalar(SCALING * 1), btScalar(SCALING * 1), btScalar(0.04)));
	box2DShape = new btConvex2dShape(childShape0);
	childShape1 = new btConvexHullShape(&points[0].getX(), 3);
	convexHull2DShape = new btConvex2dShape(childShape1);
	childShape2 = new btCylinderShapeZ(btVector3(btScalar(SCALING * 1), btScalar(SCALING * 1), btScalar(0.04)));
	cylinder2DShape = new btConvex2dShape(childShape2);

	m_collisionShapes.push_back(box2DShape);
	m_collisionShapes.push_back(convexHull2DShape);
	m_collisionShapes.push_back(cylinder2DShape);
	m_collisionShapes.push_back(childShape0);
	m_collisionShapes.push_back(childShape1);
	m_collisionShapes.push_back(childShape2);
}

void PhysicsManager::exitPhysics()
{
	if (world)
	{

		int i;
		for (i = world->getNumConstraints() - 1; i >= 0; i--)
		{
			world->removeConstraint(world->getConstraint(i));
		}
		for (i = world->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = world->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			world->removeCollisionObject(obj);
			delete obj;
		}
	}
	delete world;
	world = 0;
	delete solver;
	solver = 0;
	delete dispatcher;
	dispatcher = 0;
	delete collisionConfiguration;
	collisionConfiguration = 0;
	delete broadphase;
	broadphase = 0;
}



PhysicsManager::~PhysicsManager()
{
	delete sphereShape;
	delete boxShape;
	delete cylinderShape;
	delete coneShape;
	delete capsuleShape;

	delete world;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;
}
