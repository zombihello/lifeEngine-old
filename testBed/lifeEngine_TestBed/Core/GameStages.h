#pragma once

// lifeEngine
#include <System\leSystem.h>
#include <System\3D\Model.h>
#include <System\3D\Camera.h>
#include <System\3D\Scene3D.h>
#include <System\UI\GUI.h>
#include <System\MaterialManager.h>
#include <Level\3D\Level3D.h>
#include <Physic\3D\Physic3D.h>
#include <Animation\Animation3D.h>
#include <Physic\3D\Body3D.h>
#include <Efects\3D\LightManager3D.h>
using namespace le;

#include "PlayerController.h"

/////////////////////
// Консоль
/////////////////////
//#include "System\console.h"

//---------------------------------------------------------

class GameStages : public BasicStagesGame
{
public:

	GameStages( System& System );
	~GameStages();

	void CheckStages();

	/////////////////////
	// Консоль
	/////////////////////
	//Console* console_main;

private:

	PlayerController*	Player;

	System*				System;
	Scene3D*			Scene;
	GUI*				GUI;	
	TextManager*		TextManager;
	Physic3D*			Physic;
	Level3D*			Level;
	LightManager3D*     LightManager3D;
	FPS					FPS;

	vector<Model*>		vStaticModels;
	vector<Model*>		vDinamicModels;
	vector<Body3D*>     vBodys;
};

//---------------------------------------------------------