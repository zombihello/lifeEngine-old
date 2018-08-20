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

		ActiveCamera = new le::Camera( System );
		Scene->SetCamera( "Player2", *ActiveCamera );
		Cameras.push_back( ActiveCamera );

		ActiveCamera = new le::Camera( System );
		Scene->SetCamera( "Player1", *ActiveCamera );
		Cameras.push_back( ActiveCamera );

		Level = new le::Level( System );
		Level->LoadLevel( "../maps/" + NameMap + ".bsp" );
		Level->AddToScene( *Scene );

		le::Model* mdl = new le::Model();
		mdl->LoadModel( "tt", "../models/leanna.lmd" );
		mdl->SetScale( glm::vec3( 2.5f, 2.5f, 2.5f ) );
		Scene->AddModel( mdl );
		Models.push_back( mdl );

		LightManager.AddSpotLight( "spot", 300, 300, glm::vec3( 0, -90, 0 ), glm::vec3(), glm::vec4( 164.f, 126.f, 0, 255.f ), 2.f );
		Spot = LightManager.GetSpotLight( "spot" );

		LightManager.AddPointLight( "point", 300, glm::vec3(), glm::vec4( 164.f, 126.f, 0, 255.f ), 2.f );
		Point = LightManager.GetPointLight( "point" );

		LightManager.AddLightsToScene( *Scene );

		Count = 0.f;
		MoveRight = true;
	}

	~Game()
	{
		delete Scene;
		delete Level;

		for ( size_t i = 0; i < Models.size(); i++ )
			delete Models[ i ];

		for ( size_t i = 0; i < Cameras.size(); i++ )
			delete Cameras[ i ];
	}

	void Update()
	{
		if ( Keyboard::isKeyPressed( Keyboard::F1 ) )
			ActiveCamera = Scene->SetActiveCamera( "Player1" );

		if ( Keyboard::isKeyPressed( Keyboard::F2 ) )
			ActiveCamera = Scene->SetActiveCamera( "Player2" );

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
			ActiveCamera->Move( le::Camera::Forward, 5.25f * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::S ) )
			ActiveCamera->Move( le::Camera::Back, 5.25f * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::A ) )
		{
			ActiveCamera->Move( le::Camera::Left, 3.25f * Configuration->Time );

			if ( ActiveCamera->GetInclinationCamera() > -5 )
				ActiveCamera->TiltCamera( -0.2f );
		}

		if ( Keyboard::isKeyPressed( Keyboard::D ) )
		{
			ActiveCamera->Move( le::Camera::Right, 3.25f * Configuration->Time );

			if ( ActiveCamera->GetInclinationCamera() < 5 )
				ActiveCamera->TiltCamera( 0.2f );
		}

		if ( Keyboard::isKeyPressed( Keyboard::Q ) )
			le::System::SetWireframeRender( true );
		else
			le::System::SetWireframeRender( false );

		if ( Keyboard::isKeyPressed( Keyboard::Z ) )
			Spot->SetPosition( ActiveCamera->GetPosition() );

		if ( Keyboard::isKeyPressed( Keyboard::X ) )
			Point->SetPosition( ActiveCamera->GetPosition() );

		if ( !Keyboard::isKeyPressed( Keyboard::A ) && !Keyboard::isKeyPressed( Keyboard::D ) )
		{
			if ( ActiveCamera->GetInclinationCamera() + 0.5f < 0 )
				ActiveCamera->TiltCamera( 0.5f );
			else if ( ActiveCamera->GetInclinationCamera() - 0.5f > 0 )
				ActiveCamera->TiltCamera( -0.5f );
		}

		for ( size_t Id = 0; Id < Models.size(); Id++ )
			Models[ Id ]->GetAnimationManager()->Update();

		ActiveCamera->UpdateTargetPoint();
		Scene->Render();

		if ( Keyboard::isKeyPressed( Keyboard::E ) )
			GBuffer->ShowDebug();
	}

	bool MoveRight;
	float Count;

	le::Scene*					Scene;
	le::Camera*					ActiveCamera;
	le::Level*					Level;
	le::GBuffer*				GBuffer;
	le::LightManager			LightManager;
	le::SpotLight*				Spot;
	le::PointLight*				Point;

	vector<le::Model*>			Models;
	vector<le::Camera*>			Cameras;
};

int main( int argc, char** argv )
{
	int IndexMap;
	string NameMap;

	cout << "------------\n";
	cout << "     Select Map\n";
	cout << "> 1. Office\n";
	cout << "> 2. Destroy Home\n";
	cout << "> 3. Other..\n";
	cout << "------------\n";

	cout << "> Select: ";
	cin >> IndexMap;

	switch ( IndexMap )
	{
	case 1:
		NameMap = "office";
		break;

	case 2:
		NameMap = "destroyHome";
		break;

	case 3:
		cout << "> Enter Name Map: ";
		cin >> NameMap;
		cout << endl;
		break;

	default:
		NameMap = "office";
		break;
	}

	le::System System( argc, argv, "../config.cfg", "../" ENGINE ".log" );
	System.WindowCreate( ENGINE " | " ENGINE_VERSION, Style::Default );

	Game Game( System, NameMap );
	System.MainLoop( Game );

	return 0;
}