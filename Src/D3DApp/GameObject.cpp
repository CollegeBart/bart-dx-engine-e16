#include "GameObject.h"

GameObject::GameObject()
	: Component()
	, body(nullptr)
	, needImpl(false)
{
	transform.setIdentity();
	D3DXMatrixIdentity(&rot);
	D3DXMatrixIdentity(&trans);
	D3DXMatrixScaling(&scale, 1.0f, -1.0f, 1.0);
	D3DXMatrixIdentity(&resultant);
}

GameObject::GameObject(const char * path)
	: Component()
	, body(nullptr)
	, needImpl(true)
{
	//HR(D3DXCreateTextureFromFile(gD3DDevice, path, &mTexture));
	HR(D3DXCreateTextureFromFileEx(gD3DDevice, path, 0, 0, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 255, 255), &info, NULL, &mTexture));
	transform.setIdentity();
	D3DXMatrixIdentity(&rot);
	D3DXMatrixIdentity(&trans);
	D3DXMatrixScaling(&scale, 1.0f, -1.0f, 1.0);
	D3DXMatrixIdentity(&resultant);
}

GameObject::GameObject(const char * path, float startX, float startY)
	: Component()
	, body(nullptr)
	, needImpl(true)
{
	//HR(D3DXCreateTextureFromFile(gD3DDevice, path, &mTexture));
	HR(D3DXCreateTextureFromFileEx(gD3DDevice, path, 0, 0, 1, 0,
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 255, 255), &info, NULL, &mTexture));

	mPosition.x = startX;
	mPosition.y = startY;
	transform.setIdentity();
	D3DXMatrixIdentity(&rot);
	D3DXMatrixIdentity(&trans);
	D3DXMatrixTranslation(&trans, startX, startY, 0);
	D3DXMatrixScaling(&scale, 1.0f, -1.0f, 1.0);
	D3DXMatrixIdentity(&resultant);
}

GameObject::~GameObject()
{
}

void GameObject::OnLostDevice()
{
}

void GameObject::OnResetDevice()
{
}

void GameObject::CreateBody(const btVector3 & pos, float mass, btCollisionShape * shape)
{
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	transform.setOrigin(pos);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);

	btScalar btMass = mass;


	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
	body = new btRigidBody(cInfo);
	WORLD->addRigidBody(body);
}

void GameObject::CreateBody(const btVector3 & pos, float mass, btCollisionShape * shape, short group, short mask)
{
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0, 0, 0);
	if (isDynamic)
		shape->calculateLocalInertia(mass, localInertia);

	transform.setOrigin(pos);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(transform);

	btScalar btMass = mass;


	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, myMotionState, shape, localInertia);
	body = new btRigidBody(cInfo);
	WORLD->addRigidBody(body, group, mask);
}

void GameObject::Start()
{
}

void GameObject::Update()
{
	MakeResultantMatrix();
}

void GameObject::Draw(ID3DXSprite* spriteBatch)
{
	if (needImpl)
	{

		HR(spriteBatch->Begin(
			D3DXSPRITE_ALPHABLEND |
			D3DXSPRITE_OBJECTSPACE |
			D3DXSPRITE_DONOTMODIFY_RENDERSTATE));
		{
			HR(spriteBatch->SetTransform(&resultant));
			HR(spriteBatch->Draw(mTexture, 0,
				&mCenter, 0, D3DCOLOR_XRGB(255, 255, 255)));
		}

		HR(spriteBatch->End());
	}
}

void GameObject::MakeResultantMatrix()
{
	if (body && body->getMotionState())
	{
		btQuaternion btQuat = body->getWorldTransform().getRotation();
		btVector3 btVec3 = body->getWorldTransform().getOrigin();

		D3DXQUATERNION quat{ btQuat.x(), btQuat.y(), btQuat.z(), btQuat.w() };

		D3DXMatrixRotationQuaternion(&rot, &quat);
		D3DXMatrixTranslation(&trans, 0.0f, 0.0f, 0.0f);
		D3DXMatrixTranslation(&trans, btVec3.x(), btVec3.y(), btVec3.z());
	}
	D3DXMatrixMultiply(&resultant, &rot, &trans);
	D3DXMatrixMultiply(&resultant, &resultant, &scale);
}
