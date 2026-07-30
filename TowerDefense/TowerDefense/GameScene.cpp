#include "pch.h"
#include "GameScene.h"

void GameScene::Init(Graphic& graphic)
{
	_inGameBg.Load(graphic, L"Res\\InGame.jpg");
}

void GameScene::Render(Graphic& graphic)
{

	_inGameBg.Draw(graphic, 0.0f, 0.0f);

}
void GameScene::Release()
{
	_inGameBg.Release();
}
void GameScene::Update()
{

}
