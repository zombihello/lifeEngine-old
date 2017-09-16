#include <System\System.h>
#include <System\ResourcesManager.h>
#include <Graphics\Mesh.h>
#include <Graphics\Model.h>
#include <Graphics\Scene.h>
#include <Graphics\Camera.h>
#include <Graphics\Level.h>
#include <System\Logger.h>

class Game : public le::BasicApplication
{
public:
	Game( le::System& System ) : le::BasicApplication( System )
	{	
		model.LoadModel( "Leanna", "../models/leanna.lmd" );
		model.GetAnimationManager()->Play( "leanna_anim", true );

		Scene = new le::Scene( System );
		Scene->AddModelToScene( &model );
		
		Camera = new le::Camera( System );
		Scene->SetCamera( *Camera );

		Level.LoadLevel( "../maps/test.lmap" );
		Level.AddToScene( *Scene );
	}

	void Update()
	{
		if ( Keyboard::isKeyPressed( Keyboard::W ) )
			Camera->Move( le::Camera::Forward, 1 * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::S ) )
			Camera->Move( le::Camera::Back, 1 * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::A ) )
			Camera->Move( le::Camera::Left, 1 * Configuration->Time );

		if ( Keyboard::isKeyPressed( Keyboard::D ) )
			Camera->Move( le::Camera::Right, 1 * Configuration->Time );
		
		model.GetAnimationManager()->Update();
		Camera->UpdateCamera();
		Scene->RenderScene();
	}

	le::Model model;
	le::Scene* Scene;
	le::Camera* Camera;
	le::Level Level;
};

int main( int argc, char** argv )
{
	le::System System( argc, argv, "../config.cfg", "../" ENGINE ".log" );
	System.WindowCreate( ENGINE " | " ENGINE_VERSION, Style::Default );

	Game Game( System );
	System.MainLoop( Game );

	return 0;
}