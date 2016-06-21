//#pragma once
//#include "Engine.h"
//#include <vector>
//
//struct AABB
//{
//	// Initialize to an infinitely small AABB.
//	AABB()
//		: minPt(INFINITY, INFINITY, INFINITY),
//		maxPt(-INFINITY, -INFINITY, -INFINITY){}
//	D3DXVECTOR3 center()
//	{
//		return 0.5f*(minPt + maxPt);
//	}
//	D3DXVECTOR3 minPt;
//	D3DXVECTOR3 maxPt;
//};
//
//
//struct Particle
//{
//	D3DXVECTOR3 initialPos;
//	D3DXVECTOR3 initialVelocity;
//	float initialSize;   // In pixels
//	float initialTime;
//	float lifeTime;	// Time particle stays on screen
//	float mass;
//	D3DCOLOR initialColor;
//
//
//	static IDirect3DVertexDeclaration9* Decl;
//};
//
//
//
//class ParticleSystem
//{
//public:
//
//	ParticleSystem();
//
//	ParticleSystem(const std::string& fxName,
//				   const std::string& techName,
//				   const std::string& texName,
//				   const D3DXVECTOR3& accel,
//				   const AABB& box,
//				   int maxNumParticles,
//				   float timePerParticle);
//
//	~ParticleSystem();
//
//
//
//	float GetTime() { return mTime; }
//	void SetTime(float t) { mTime = t; }
//	const AABB& getAABB()const { return mBox; }
//
//	void SetWorldMtx(const D3DMATRIX& world);
//	void AddParticle();
//
//	virtual void OnLostDevice();
//	virtual void OnResetDevice();
//
//	virtual void InitParticle(Particle& out) = 0;
//	virtual void Update(float dt);
//	virtual void Draw();
//
//protected:
//
//	ID3DXEffect* mfx;
//	D3DXHANDLE mhTech;
//	D3DXHANDLE mhWVP;
//	D3DXHANDLE mhEyePosL;
//	D3DXHANDLE mhTex;
//	D3DXHANDLE mhTime;
//	D3DXHANDLE mhAccel;
//	D3DXHANDLE mhViewportHeight;
//
//	IDirect3DTexture9* mTex;
//	IDirect3DVertexBuffer9* mvb;
//	D3DXMATRIX mWorld;
//	D3DXMATRIX mInvWorld;
//	float mTime;
//	D3DXVECTOR3 mAccel;
//	AABB mBox;
//	int mMaxNumParticles;
//	float mTimePerParticle;
//
//
//	std::vector<Particle> mParticles;
//	std::vector<Particle*> mAliveParticles;
//	std::vector<Particle*> mDeadParticles;
//};
//
