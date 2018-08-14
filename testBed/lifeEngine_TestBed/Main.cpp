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

		Scene = new le::Scene();
		GBuffer = &Scene->GetGBuffer();

		Camera = new le::Camera( System );
		Scene->SetCamera( *Camera );

		Level = new le::Level( System );
		Level->LoadLevel( "../maps/" + NameMap + ".bsp" );
		Level->AddToScene( *Scene );

		glm::vec3 Position, LightRotation;
		string Name;
		vector<int> LightColor;
		vector<float> Rotation;
	/*	vector<le::Entity>* LevelEntitys = &Level->GetAllEntitys();

		for ( auto it = LevelEntitys->begin(); it != LevelEntitys->end(); it++ )
		{
			if ( it->GetNameEntity() == "Dinamic_Light" )
			{
				LightColor = it->GetVelueVectorInt( "Color" );
				float Radius = it->GetValueFloat( "Radius" );
				float Intensivity = it->GetValueFloat( "Intensivity" );
				Name = it->GetValueString( "Name" );
				Position = it->GetPosition();

				LightManager.AddPointLight( Name, Radius, Position, glm::vec4( LightColor[ 0 ], LightColor[ 1 ], LightColor[ 2 ], 255 ), Intensivity );
			}

			if ( it->GetNameEntity() == "Dinamic_DirectionalLight" )
			{
				LightColor = it->GetVelueVectorInt( "Color" );
				Name = it->GetValueString( "Name" );
				float Intensivity = it->GetValueFloat( "Intensivity" );
				Position = it->GetPosition();

				LightManager.AddDirectionalLight( Name, Position, glm::vec4( LightColor[ 0 ], LightColor[ 1 ], LightColor[ 2 ], 255 ), Intensivity );
			}

			if ( it->GetNameEntity() == "Dinamic_SpotLight" )
			{
				LightColor = it->GetVelueVectorInt( "Color" );
				Rotation = it->GetVelueVectorFloat( "Rotation" );
				Name = it->GetValueString( "Name" );
				float Radius = it->GetValueFloat( "Radius" );
				float Height = it->GetValueFloat( "Height" );
				float Intensivity = it->GetValueFloat( "Intensivity" );
				Position = it->GetPosition();
				LightRotation = glm::vec3( Rotation[ 0 ], Rotation[ 1 ], Rotation[ 2 ] );

				LightManager.AddSpotLight( Name, Radius, Height, LightRotation, Position, glm::vec4( LightColor[ 0 ], LightColor[ 1 ], LightColor[ 2 ], 255 ), Intensivity );
			}

			if ( it->GetNameEntity() == "StaticProp" )
			{
				string ModelName = it->GetValueString( "ModelName" );
				Name = it->GetValueString( "Name" );
				string AnimationName = it->GetValueString( "AnimationName" );
				Rotation = it->GetVelueVectorFloat( "Rotation" );
				Position = it->GetPosition();

				le::Model* Model = new le::Model();
				Model->LoadModel( Name, "../models/" + ModelName + ".lmd" );
				Model->GetAnimationManager()->Play( AnimationName, true );
				Model->SetPosition( Position );
				Model->SetRotation( glm::vec3( Rotation[ 0 ], Rotation[ 1 ], Rotation[ 2 ] ) );
				Model->SetScale( glm::vec3( 0.5f, 0.5f, 0.5f ) );

				Scene->AddModelToScene( Model );
				Models.push_back( Model );
			}
		}
		*/
		LightManager.AddSpotLight( "spot", 300, 300, glm::vec3( 0, -90, 0 ), glm::vec3(), glm::vec4( 164.f, 126.f, 0, 255.f ), 2.f );
		Spot = LightManager.GetSpotLight( "spot" );

		LightManager.AddPointLight( "point", 300, glm::vec3(), glm::vec4( 164.f, 126.f, 0, 255.f ), 2.f );
		Point = LightManager.GetPointLight( "point" );

		LightManager.AddLightsToScene( *Scene );
		LightManager.BuildShadowMaps( *Level, Scene->GetRenderBuffer_StaticModel(), Scene->GetRenderBuffer_AnimationModel() );

		Count = 0.f;
		MoveRight = true;
	}

	~Game()
	{
		delete Scene;
		delete Camera;
		delete Level;

		for ( size_t i = 0; i < Models.size(); i++ )
			delete Models[ i ];
	}

	void Update()
	{
		if ( MoveRight )
		{
			Point->SetPosition( glm::vec3( Point->Position.x + 1.f * Configuration->Time, Point->Position.y, Point->Position.z ) );
			Count += 1.f;
		}
		else
		{
			Point->SetPosition( glm::vec3( Point->Position.x - 1.f * Configuration->Time, Point->Position.y, Point->Position.z ) );
			Count -= 1.f;
		}

		if ( Count > 100 )
			MoveRight = false;
		else if ( Count < -100 )
			MoveRight = true;

		if ( Keyboard::isKeyPressed( Keyboard::W ) )
			Camera->Move( le::Camera::Forward, 5.25f * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::S ) )
			Camera->Move( le::Camera::Back, 5.25f * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::A ) )
		{
			Camera->Move( le::Camera::Left, 3.25f * Configuration->Time );

			if ( Camera->GetInclinationCamera() > -5 )
				Camera->TiltCamera( -0.2f );
		}

		if ( Keyboard::isKeyPressed( Keyboard::D ) )
		{
			Camera->Move( le::Camera::Right, 3.25f * Configuration->Time );

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

		for ( size_t Id = 0; Id < Models.size(); Id++ )
			Models[ Id ]->GetAnimationManager()->Update();

		Camera->UpdateCamera();
		Scene->RenderScene();

		if ( Keyboard::isKeyPressed( Keyboard::E ) )
			GBuffer->ShowDebug();
	}

	bool MoveRight;
	float Count;

	le::Scene*			Scene;
	le::Camera*			Camera;
	le::Level*			Level;
	le::GBuffer*		GBuffer;
	le::LightManager	LightManager;
	le::SpotLight*		Spot;
	le::PointLight*		Point;

	vector<le::Model*>	Models;
};

int main( int argc, char** argv )
{
	int IndexMap;
	string NameMap;

	cout << "------------\n";
	cout << "     Select Map					 \n";
	cout << "> 1. Wolfenstein: Stalingrad		 \n";
	cout << "> 2. Return to Castle Wolfenstein	 \n";
	cout << "> 3. Quake							 \n";
	cout << "> 4. Other..	   					 \n";
	cout << "------------\n";

	cout << "> Select: ";
	cin >> IndexMap;

	switch ( IndexMap )
	{
	case 2:
		NameMap = "escape2";
		break;

	case 3:
		NameMap = "QuakeMap";
		break;

	case 4:
		cout << "> Enter Name Map: ";
		cin >> NameMap;
		cout << endl;
		break;

	default:
		NameMap = "stalingrad";
		break;
	}

	le::System System( argc, argv, "../config.cfg", "../" ENGINE ".log" );
	System.WindowCreate( ENGINE " | " ENGINE_VERSION, Style::Default );

	Game Game( System, NameMap );
	System.MainLoop( Game );

	return 0;
}