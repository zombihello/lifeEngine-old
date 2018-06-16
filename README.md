# lifeEngine
Это игровой 3D-движок, написанный на С++ и распространяющийся под лицензией [GNU LGPL](https://ru.wikipedia.org/wiki/GNU_Lesser_General_Public_License). Он обладает высокоуровневым API которое упрощающает разработку игры.

# Пример работы движка
![lifeEngine DEMO](https://github.com/zombihello/lifeEngine/blob/raw/doc/example.jpg) 
Исходный код примера можно посмотреть по [этой сылке](https://github.com/zombihello/lifeEngine/blob/dev/testBed/lifeEngine_TestBed/Main.cpp)

# Пример использования

```
#include <System\System.h>
#include <Graphics\Mesh.h>
#include <Graphics\Model.h>
#include <Graphics\Scene.h>
#include <Graphics\Camera.h>

class Game : public le::BasicApplication
{
public:
	Game( le::System& System ) : le::BasicApplication( System )
	{
		Model.LoadModel( "Leanna", "../models/leanna.lmd" );
		Model.GetAnimationManager()->Play( "leanna_anim", true );

		Scene = new le::Scene();
		GBuffer = &Scene->GetGBuffer();
		Scene->AddModelToScene( &Model );
		
		Camera = new le::Camera( System );
		Scene->SetCamera( *Camera );
	}

	~Game()
	{
		delete Scene;
		delete Camera;
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
	
		Model.GetAnimationManager()->Update();
		Camera->UpdateCamera();
		Scene->RenderScene();
	}
	
private:

	le::Model Model;
	le::Scene* Scene;
	le::Camera* Camera;
};

int main( int argc, char** argv )
{
	le::System System( argc, argv, "../config.cfg", "../" ENGINE ".log" );
	System.WindowCreate( "lifeEngine Test", Style::Default );

	Game Game( System );
	System.MainLoop( Game );

	return 0;
}
```
