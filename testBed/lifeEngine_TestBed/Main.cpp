#include <System\System.h>
#include <System\ResourcesManager.h>
#include <Graphics\Mesh.h>
#include <Graphics\Model.h>
#include <Graphics\Scene.h>
#include <Graphics\Camera.h>
#include <Graphics\Level.h>
#include <Graphics\LightManager.h>
#include <System\Logger.h>

class Game : public le::BasicApplication
{
public:
	Game( le::System& System, const string& NameMap ) : le::BasicApplication( System )
	{
		le::ResourcesManager::SetErrorTexture( "../textures/Error.png" );

		model.LoadModel( "Leanna", "../models/leanna.lmd" );
		model.GetAnimationManager()->Play( "leanna_anim", true );

		Scene = new le::Scene();
		GBuffer = &Scene->GetGBuffer();
		Scene->AddModelToScene( &model );

		Camera = new le::Camera( System );
		Scene->SetCamera( *Camera );

		Level = new le::Level( System );
		Level->LoadLevel( "../maps/" + NameMap + ".lmap" );
		Level->AddToScene( *Scene );

		glm::vec3 LightPosition, LightRotation;
		string NameLight;
		vector<int> LightColor;
		vector<float> Rotation;
		vector<le::Entity>* LevelEntitys = &Level->GetAllEntitys();

		for ( auto it = LevelEntitys->begin(); it != LevelEntitys->end(); it++ )
		{
			if ( it->GetNameEntity() == "light" )
			{
				LightColor = it->GetVelueVectorInt( "Color" );
				float Radius = it->GetValueFloat( "Radius" );
				float Intensivity = it->GetValueFloat( "Intensivity" );
				NameLight = it->GetValueString( "Name" );
				LightPosition = it->GetPosition();

				LightManager.AddPointLight( NameLight, Radius, LightPosition, glm::vec4( LightColor[ 0 ], LightColor[ 1 ], LightColor[ 2 ], 255 ), Intensivity );
			}

			if ( it->GetNameEntity() == "lightDirectional" )
			{
				LightColor = it->GetVelueVectorInt( "Color" );
				NameLight = it->GetValueString( "Name" );
				float Intensivity = it->GetValueFloat( "Intensivity" );
				LightPosition = it->GetPosition();

				LightManager.AddDirectionalLight( NameLight, LightPosition, glm::vec4( LightColor[ 0 ], LightColor[ 1 ], LightColor[ 2 ], 255 ), Intensivity );
			}

			if ( it->GetNameEntity() == "SpotLight" )
			{
				LightColor = it->GetVelueVectorInt( "Color" );
				Rotation = it->GetVelueVectorFloat( "Rotation" );
				NameLight = it->GetValueString( "Name" );
				float Radius = it->GetValueFloat( "Radius" );
				float Height = it->GetValueFloat( "Height" );
				float Intensivity = it->GetValueFloat( "Intensivity" );
				LightPosition = it->GetPosition();
				LightRotation = glm::vec3( Rotation[0], Rotation[1], Rotation[2] );			

				LightManager.AddSpotLight( NameLight, Radius, Height, LightRotation, LightPosition, glm::vec4( LightColor[ 0 ], LightColor[ 1 ], LightColor[ 2 ], 255 ), Intensivity );
			}
		}

		LightManager.AddSpotLight( "spot", 150, 150, glm::vec3( 0, -90, 0 ), glm::vec3(), glm::vec4( 164.f, 126.f, 0, 255.f ), 2.f );
		Spot = LightManager.GetSpotLight( "spot" );

		LightManager.AddPointLight( "point", 100, glm::vec3(), glm::vec4( 164.f, 126.f, 0, 255.f ), 2.f );
		Point = LightManager.GetPointLight( "point" );

		LightManager.AddLightsToScene( *Scene );	
		LightManager.BuildShadowMaps( *Level, Scene->GetRenderBuffer_StaticModel(), Scene->GetRenderBuffer_AnimationModel() );
	}

	~Game()
	{
		delete Scene;
		delete Camera;
		delete Level;
	}

	void Update()
	{
		if ( Keyboard::isKeyPressed( Keyboard::W ) )
			Camera->Move( le::Camera::Forward, 1.25f * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::S ) )
			Camera->Move( le::Camera::Back, 1.25f * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::A ) )
		{
			Camera->Move( le::Camera::Left, 1.25f * Configuration->Time );

			if ( Camera->GetInclinationCamera() > -5 )
				Camera->TiltCamera( -0.2f );
		}

		if ( Keyboard::isKeyPressed( Keyboard::D ) )
		{
			Camera->Move( le::Camera::Right, 1.25f * Configuration->Time );

			if ( Camera->GetInclinationCamera() < 5 )
				Camera->TiltCamera( 0.2f );
		}

		if ( Keyboard::isKeyPressed( Keyboard::Q ) )
			le::System::SetWireframeRender( true );
		else
			le::System::SetWireframeRender( false );
		
		if ( Keyboard::isKeyPressed( Keyboard::Z ) )
			Spot->SetPosition( Camera->GetPosition() );

		if ( Keyboard::isKeyPressed( Keyboard::X ) )
			Point->SetPosition( Camera->GetPosition() );

		if ( !Keyboard::isKeyPressed( Keyboard::A ) && !Keyboard::isKeyPressed( Keyboard::D ) )
		{
			if ( Camera->GetInclinationCamera() + 0.5f < 0 )
				Camera->TiltCamera( 0.5f );
			else if ( Camera->GetInclinationCamera() - 0.5f > 0 )
				Camera->TiltCamera( -0.5f );
		}

		model.GetAnimationManager()->Update();
		Camera->UpdateCamera();
		Scene->RenderScene();
		
		if ( Keyboard::isKeyPressed( Keyboard::E ) )
			GBuffer->ShowDebug();
	}

	le::Model model;
	le::Scene* Scene;
	le::Camera* Camera;
	le::Level* Level;
	le::GBuffer* GBuffer;
	le::LightManager LightManager;
	le::SpotLight* Spot;
	le::PointLight* Point;
};

int main( int argc, char** argv )
{
	int IndexMap;
	string NameMap;

	cout << "*******************\n";
	cout << "*    Select Map   *\n";
	cout << "* 1. base         *\n";
	cout << "* 2. shadows      *\n";
	cout << "*******************\n\n";

	cout << "> Select: ";
	cin >> IndexMap;
	cout << endl;

	switch ( IndexMap )
	{
	case 2:
		NameMap = "shadows";
		break;

	default:
		NameMap = "base";
		break;
	}

	le::System System( argc, argv, "../config.cfg", "../" ENGINE ".log" );
	System.WindowCreate( ENGINE " | " ENGINE_VERSION, Style::Default );

	Game Game( System, NameMap );
	System.MainLoop( Game );

	return 0;
}