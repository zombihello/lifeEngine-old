#include "GameStages.h"
#include "../BuildNumber.h"
/////////////////////
// Консоль
/////////////////////
//#include <TGUI/TGUI.hpp>

//---------------------------------------------------------

GameStages::GameStages( le::System& System )
{
	this->System = &System;
	
	TextManager = new le::TextManager( System );
	GUI = new le::GUI( System );
	Scene = new le::Scene3D( System );
	Physic = new le::Physic3D( Vector3f( 0, -100, 0 ) );
	LightManager3D = new le::LightManager3D();
	Physic->EnableDebug();
	

	Player = new PlayerController( System, *Physic, Vector2f( 10, 20 ) );
	Scene->SetPlayerCamera( *Player->GetPlayerCamera() );

	Level = new le::Level3D( System, *Scene, *Physic );
	Level->LoadLevel( "../Maps/testLevel.lmap" );
	vector<Entity> vEntitys = Level->GetAllEntitys();

	for ( int i = 0; i < vEntitys.size(); i++ )
	{
		Entity Entity = vEntitys[i];

		if ( Entity.GetNameEntity() == "StartPosition" )
			Player->SetPosition( Entity.GetPosition() );
		else if ( Entity.GetNameEntity() == "StaticProp" )
		{
			string modelName = Entity.GetValueString( "ModelName" );
			string animationName = Entity.GetValueString( "AnimationName" );
			vector<float> vRotate = Entity.GetVelueVectorFloat( "Rotation" );

			Model* model = new le::Model( System );
			model->LoadModel( modelName, "../Models/" + modelName + ".xlmd" );
			model->GetAnimationManager().Play( animationName, true );
			model->SetPosition( Entity.GetPosition() );
			model->SetScale( Vector3f( 0.5, 0.5, 0.5 ) );
			model->AddToScene( *Scene );

			if ( vRotate.size() == 3 )
				model->SetRotate( Vector3f( vRotate[0], vRotate[1], vRotate[2] ) );

			vStaticModels.push_back( model );
		}
		else if ( Entity.GetNameEntity() == "DinamicProp" )
		{
			string modelName = Entity.GetValueString( "ModelName" );
			string animationName = Entity.GetValueString( "AnimationName" );
			float massa = Entity.GetValueFloat( "Massa" );
			vector<float> vRotate = Entity.GetVelueVectorFloat( "Rotation" );

			Model* model = new le::Model( System );
			model->LoadModel( modelName, "../Models/" + modelName + ".xlmd" );
			model->GetAnimationManager( ).Play( animationName, true );
			model->SetPosition( Entity.GetPosition( ) );
			model->AddToScene( *Scene );

			Body3D_ConstructionInfo ModelConstructionInfo( Body3D_ConstructionInfo::Dinamic, massa, Entity.GetPosition( ), Vector3f( vRotate[0], vRotate[1], vRotate[2] ) );
			Body3D* Body = new le::Body3D( *Physic, &ModelConstructionInfo, ShapeType_Mesh( &model->GetCollisionVertexs( ), &model->GetCollisionIdVertexs( ) ) );

			model->SetScale( Vector3f( 0.5, 0.5, 0.5 ), Body->GetIndexVertexArrays( ) );

			vBodys.push_back( Body );
			vDinamicModels.push_back( model );
		}
		else if ( Entity.GetNameEntity() == "light" )
		{
			Light3D light;
			vector<float> vColor = Entity.GetVelueVectorFloat( "Color" );

			light.SetPosition( glm::vec3( Entity.GetPosition().x, Entity.GetPosition().y, Entity.GetPosition().z ) );
			light.SetColor( glm::vec4( vColor[ 0 ], vColor[ 1 ], vColor[ 2 ], 1 ) );
			light.SetRadius( Entity.GetValueFloat( "Radius" ) );

			LightManager3D->CreateLight( light );
		}
	}

	LightManager3D->AddLightsToScene( *Scene );

	TextManager->LoadFont( "../Resources/Fonts/UIFont.ttf" );
	TextManager->WriteText( L"FPS: ", 15, Vector2f( 5, 5 ), Color::White ); // id 1
	TextManager->WriteText( "Build: " BUILD_NUMBER, 15, Vector2f( 5, 700 ), Color::White ); // id 2

	GUI->AddTextManager( TextManager );

	/////////////////////
	// Консоль
	/////////////////////
	//console_main = new le::Console(System);
	//console_main->loadWidgets(*console_main->gui);
}

//---------------------------------------------------------

GameStages::~GameStages()
{
	for ( int i = 0; i < vBodys.size(); i++ )
		delete vBodys[i];

	for ( int i = 0; i < vStaticModels.size(); i++ )
		delete vStaticModels[i];

	for ( int i = 0; i < vDinamicModels.size( ); i++ )
		delete vDinamicModels[i];

	delete TextManager;
	delete GUI;
	delete Player;
	delete Level;
	delete Physic;
	delete LightManager3D;
	delete Scene;
}

//---------------------------------------------------------

void GameStages::CheckStages()
{
	if ( sf::Keyboard::isKeyPressed( sf::Keyboard::Escape ) )
		System->GetWindow().close();

	Player->Update();

	for ( int i = 0; i < vDinamicModels.size( ); i++ )
	{
		Body3D* Body = vBodys[i];

		vDinamicModels[i]->SetPosition( Body->GetPosition() );
		vDinamicModels[i]->SetRotate( Body->GetRotation() );
	}

	Physic->UpdatePhysic( 0.01667f );

	LightManager3D->UpdateLights();
	Level->UpdateLevel();

	for ( int i = 0; i < vDinamicModels.size( ); i++ )
	{
		vDinamicModels[i]->GetAnimationManager( ).UpdateAnimation( );
		vDinamicModels[i]->UpdateModel( );
	}

	for ( int i = 0; i < vStaticModels.size(); i++ )
	{
		vStaticModels[i]->GetAnimationManager().UpdateAnimation();
		vStaticModels[i]->UpdateModel();
	}

	Scene->RenderScene();
	
	if ( Keyboard::isKeyPressed( Keyboard::Q ) )
		Physic->ShowDebug( Player->GetPlayerCamera( ) );

	FPS.UpdateFPS();
	TextManager->GetText( 1 ).SetText( L"FPS: ", FPS.GetFPS() );

	GUI->UpdateGUI();

	/////////////////////
	// Консоль
	/////////////////////
	//console_main->Update(*console_main->gui);
}

//---------------------------------------------------------