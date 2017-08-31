#include <System\System.h>
#include <System\ResourcesManager.h>
#include <Graphics\Mesh.h>
#include <Graphics\Model.h>
#include <Graphics\Scene.h>
#include <Graphics\Camera.h>
#include <System\Logger.h>

class Game : public le::BasicApplication
{
public:
	Game( le::System& System ) : le::BasicApplication( System )
	{
		le::ResourcesManager::LoadGlTexture( "Door1", "../textures/1.jpg" );

		le::Mesh mesh;
		mesh.LoadMesh( "../models/box.lmd" );

		le::ResourcesManager::LoadMesh( "Box", "../models/box.lmd" );

		model.LoadModel( "Leanna", "../models/leanna.lmd" );

		le::Model model1;
		model1.LoadModel( *le::ResourcesManager::GetMesh( "Box" ) );

		Scene = new le::Scene( System );
		Scene->AddModelToScene( &model );

		Camera = new le::Camera( System );
		Scene->SetCamera( *Camera );
	}

	void Update()
	{
		if ( Keyboard::isKeyPressed( Keyboard::W ) )
			Camera->Move( le::Camera::Forward, 1 );

		if ( Keyboard::isKeyPressed( Keyboard::S ) )
			Camera->Move( le::Camera::Back, 1 );

		if ( Keyboard::isKeyPressed( Keyboard::A ) )
			Camera->Move( le::Camera::Left, 1 );

		if ( Keyboard::isKeyPressed( Keyboard::D ) )
			Camera->Move( le::Camera::Right, 1 );

		Camera->UpdateCamera();
		Scene->RenderScene();
	}

	le::Model model;
	le::Scene* Scene;
	le::Camera* Camera;
};

int main( int argc, char** argv )
{
	le::System System( argc, argv, "../config.cfg", "../" ENGINE ".log" );
	System.WindowCreate( ENGINE " | " ENGINE_VERSION, Style::Default );

	Game Game( System );
	System.MainLoop( Game );

	return 0;
}