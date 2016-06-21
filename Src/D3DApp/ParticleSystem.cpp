//#include "ParticleSystem.h"
//
//
//
//ParticleSystem::ParticleSystem()
//{
//}
//
//ParticleSystem::ParticleSystem(const std::string & fxName, 
//							   const std::string & techName,
//							   const std::string & texName, 
//							   const D3DXVECTOR3 & accel, 
//							   const AABB & box, 
//							   int maxNumParticles, 
//							   float timePerParticle)
//	: mMaxNumParticles(maxNumParticles), mTimePerParticle(timePerParticle)
//	, mBox(box), mAccel(accel)
//{
//	mParticles.resize(mMaxNumParticles);
//	mAliveParticles.resize(mMaxNumParticles);
//	mDeadParticles.resize(mMaxNumParticles);
//
//	for (int i = 0; i < mMaxNumParticles; i++)
//	{
//		mParticles[i].lifeTime = -1.0f;
//		mParticles[i].initialTime = 0.0f;
//	}
//}
//
//
//ParticleSystem::~ParticleSystem()
//{
//}
//
//void ParticleSystem::SetWorldMtx(const D3DMATRIX & world)
//{
//	mWorld = world;
//	D3DXMatrixInverse(&mInvWorld, 0, &mWorld);
//}
//
//void ParticleSystem::AddParticle()
//{
//	if (mDeadParticles.size() > 0)
//	{
//		Particle* p = mDeadParticles.back();
//		InitParticle(*p);
//
//		mDeadParticles.pop_back();
//		mAliveParticles.push_back(p);
//	}
//}
//
//void ParticleSystem::OnLostDevice()
//{
//	HR(mfx->OnLostDevice());
//	ReleaseCOM(mvb);
//}
//
//void ParticleSystem::OnResetDevice()
//{
//	HR(mfx->OnResetDevice());
//	if (mvb == 0)
//	{
//		HR(gD3DDevice->CreateVertexBuffer(mMaxNumParticles * sizeof(Particle),
//			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,
//			0, D3DPOOL_DEFAULT, &mvb, 0));
//	}
//}
//
//void ParticleSystem::Update(float dt)
//{
//	mTime += dt;
//
//	mDeadParticles.resize(0);
//	mAliveParticles.resize(0);
//
//	for (int i = 0; i < mMaxNumParticles; i++)
//	{
//		if ((mTime - mParticles[i].initialTime) > mParticles[i].lifeTime)
//		{
//			mDeadParticles.push_back(&mParticles[i]);
//		}
//		else
//		{
//			mAliveParticles.push_back(&mParticles[i]);
//		}
//	}
//
//	if (mTimePerParticle > 0.0f)
//	{
//		static float timeAccum = 0.0f;
//		timeAccum += dt;
//		while (timeAccum >= mTimePerParticle)
//		{
//			AddParticle();
//			timeAccum - +mTimePerParticle;
//		}
//	}
//}
//
//void ParticleSystem::Draw()
//{
//	
//	D3DXVECTOR3 eyePosW = gCamera->pos();
//	D3DXVECTOR3 eyePosL;
//	D3DXVec3TransformCoord(&eyePosL, &eyePosW, &mInvWorld);
//	// Set FX parameters.
//	HR(mfx->SetValue(mhEyePosL, &eyePosL, sizeof(D3DXVECTOR3)));
//	HR(mfx->SetFloat(mhTime, mTime));
//	HR(mfx->SetMatrix(mhWVP, &(mWorld*gCamera->viewProj())));
//	// Point sprite sizes are given in pixels. So if the
//	// viewport size is changed, then more or less pixels
//	// become available, which alters the perceived size of
//	// the particles. For example, if the viewport is 32x32,
//	// then a 32x32 sprite covers the entire viewport! But if
//	// the viewport is 1024x1024, then a 32x32 sprite only
//	// covers a small portion of the viewport. Thus, we scale
//	// the particle's size by the viewport height to keep them
//	// in proportion to the viewport dimensions.
//	HWND hwnd = gEngine->GetMainWindow();
//	RECT clientRect;
//	GetClientRect(hwnd, &clientRect);
//	HR(mfx->SetInt(mhViewportHeight, clientRect.bottom));
//	UINT numPasses = 0;
//	HR(mfx->Begin(&numPasses, 0));
//	HR(mfx->BeginPass(0));
//	HR(gD3DDevice->SetStreamSource(0, mvb, 0, sizeof(Particle)));
//	HR(gD3DDevice->SetVertexDeclaration(Particle::Decl));
//	AABB boxWorld;
//	//mBox.xform(mWorld, boxWorld);
//	if (gCamera->isVisible(boxWorld))
//	{
//		
//		// Initial lock of VB for writing.
//		Particle* p = 0;
//		HR(mvb->Lock(0, 0, (void**)&p, D3DLOCK_DISCARD));
//		int vbIndex = 0;
//		// For each living particle.
//		for (UINT i = 0; i < mAliveParticles.size(); ++i)
//		{
//			// Copy particle to VB
//			p[vbIndex] = *mAliveParticles[i];
//			++vbIndex;
//		}
//		HR(mvb->Unlock());
//		// Render however many particles we copied over.
//		if (vbIndex > 0)
//		{
//			HR(gD3DDevice->DrawPrimitive(
//				D3DPT_POINTLIST, 0, vbIndex));
//		}
//	}
//	HR(mfx->EndPass());
//	HR(mfx->End());
//}
//
//
//
//
