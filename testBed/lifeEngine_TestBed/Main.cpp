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
	Game( le::System& System ) : le::BasicApplication( System )
	{
		le::ResourcesManager::SetErrorTexture( "../textures/Error.png" );

		model.LoadModel( "Leanna", "../models/leanna.lmd" );
		model.GetAnimationManager()->Play( "leanna_anim", true );

		Scene = new le::Scene( System );
		GBuffer = &Scene->GetGBuffer();
		Scene->AddModelToScene( &model );

		Camera = new le::Camera( System );
		Scene->SetCamera( *Camera );

		Level.LoadLevel( "../maps/test.lmap" );
		Level.AddToScene( *Scene );
		
		glm::vec3 LightPosition;
		string NameLight;
		vector<int> LightColor;
		vector<le::Level::Entity>* LevelEntitys = &Level.GetAllEntitys();

		for ( auto it = LevelEntitys->begin(); it != LevelEntitys->end(); it++ )
		{
			if ( it->GetNameEntity() == "light" )
			{
				LightColor = it->GetVelueVectorInt( "Color" );
				float Radius = it->GetValueFloat( "Radius" );
				NameLight = it->GetValueString( "Name" );
				LightPosition = it->GetPosition();

				LightManager.AddPointLight( NameLight, Radius, LightPosition, glm::vec4( LightColor[0], LightColor[1], LightColor[2], 255 ) );
			}

			if ( it->GetNameEntity() == "lightDirectional" )
			{
				LightColor = it->GetVelueVectorInt( "Color" );
				NameLight = it->GetValueString( "Name" );
				LightPosition = it->GetPosition();

				LightManager.AddDirectionalLight( NameLight, LightPosition, glm::vec4( LightColor[ 0 ], LightColor[ 1 ], LightColor[ 2 ], 255 ) );
			}
		}

		IsLeftButtonPressed = false;
		Timer = 0.f;
		LightManager.AddLightsToScene( *Scene );
	}

	void Update()
	{
		srand( time( NULL ) );

		if ( IsLeftButtonPressed && !Mouse::isButtonPressed( Mouse::Left ) )
		{
			Timer += Configuration->Time;

			if ( Timer > 250 ) IsLeftButtonPressed = false;
		}

		if ( Keyboard::isKeyPressed( Keyboard::W ) )
			Camera->Move( le::Camera::Forward, 1 * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::S ) )
			Camera->Move( le::Camera::Back, 1 * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::A ) )
			Camera->Move( le::Camera::Left, 1 * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::D ) )
			Camera->Move( le::Camera::Right, 1 * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::Q ) )
			le::System::SetWireframeRender( true );
		else
			le::System::SetWireframeRender( false );

		if ( Mouse::isButtonPressed( Mouse::Left ) && !IsLeftButtonPressed )
		{
			IsLeftButtonPressed = true;

			glm::vec4 LightColor( NUMBER_TO_FLOAT( rand() % 255 ), NUMBER_TO_FLOAT( rand() % 255 ), NUMBER_TO_FLOAT( rand() % 255 ), 255.f );
			float Radius = 45 + rand() % 200;

			LightManager.AddPointLight( "", Radius, Camera->GetPosition(), glm::vec4( LightColor[ 0 ], LightColor[ 1 ], LightColor[ 2 ], 255 ) );
		}

		model.GetAnimationManager()->Update();
		Camera->UpdateCamera();
		Scene->RenderScene();

		if ( Keyboard::isKeyPressed( Keyboard::E ) )
			GBuffer->ShowDebug();
	}

	float Timer;
	bool IsLeftButtonPressed;

	le::Model model;
	le::Scene* Scene;
	le::Camera* Camera;
	le::Level Level;
	le::GBuffer* GBuffer;
	le::LightManager LightManager;
};

int main( int argc, char** argv )
{
	le::System System( argc, argv, "../config.cfg", "../" ENGINE ".log" );
	System.WindowCreate( ENGINE " | " ENGINE_VERSION, Style::Default );

	Game Game( System );
	System.MainLoop( Game );

	return 0;
}