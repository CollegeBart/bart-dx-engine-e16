#include "SpriteApp.h"

SpriteApp::SpriteApp()
	: currentRotation(0)
{
	D3DXVECTOR3 camPos(0.f, 0.f, -1086.f);
	gEngine->SetCameraPostion(camPos);

	bkg = new GameObject("background.png");
	bkg->SetCenter((float)bkg->GetWidth() / 2.f, (float)bkg->GetHeight() / 2.f, 0.f);

	canon = new GameObject("Bullet_Bill_Cannon.png");
	canon->SetCenter((float)canon->GetWidth() / 2.f, 36.f, 0.f);
	canon->SetRotationZ(-45.f);
	canon->SetPosition(0.f, (-gD3DApp->GetResolutionH()/2) + 100, 0.f);

	//btVector3 pos{ 0,0,0 };
	//test2->CreateBody(pos, 1.0f, SPHERE);
}

SpriteApp::~SpriteApp()
{

}

void SpriteApp::OnLostDevice()
{

}

void SpriteApp::OnResetDevice()
{

}

void SpriteApp::Update()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(gD3DApp->GetMainWindow(), &mousePos);

	D3DXVECTOR3 vMousePos(mousePos.x - gD3DApp->GetResolutionW()/2, mousePos.y - gD3DApp->GetResolutionH()/2, 0.f);
	D3DXVECTOR3 dir = vMousePos - canon->GetPosition();

	D3DXVec3Normalize(&dir, &dir);

	canon->SetRotationZ( atan2(dir.y, dir.x) - 90 * DEG_TO_RAD);
}

void SpriteApp::Draw(ID3DXSprite * spriteBatch)
{

}
