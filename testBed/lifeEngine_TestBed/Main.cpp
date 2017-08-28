#include <System\System.h>
#include <System\ResourcesManager.h>
#include <Graphics\Mesh.h>

class Game : public le::BasicApplication
{
public:
	Game( le::System& System ) : le::BasicApplication( System )
	{
		le::ResourcesManager::LoadGlTexture( "Door1", "../textures/1.jpg" );
		le::ResourcesManager::LoadGlTexture( "Door1", "../textures/1.jpg" );
		le::ResourcesManager::LoadGlTexture( "Door2", "../textures/2.jpg" );
		le::ResourcesManager::LoadGlTexture( "Door3", "../textures/13.jpg" );

		le::Mesh mesh;
		mesh.LoadMesh( "../models/box.lmd" );

		le::ResourcesManager::LoadMesh( "Box", "../models/box.lmd" );
		le::ResourcesManager::LoadMesh( "Box1", "../models/box2.lmd" );
		le::ResourcesManager::LoadMesh( "Box", "../models/box.lmd" );
	}

	void Update()
	{
	}
};

int main( int argc, char** argv )
{
	le::System System( argc, argv, "../config.cfg", "../" ENGINE ".log" );
	System.WindowCreate( ENGINE " | " ENGINE_VERSION, Style::Default );

	Game Game( System );
	System.MainLoop( Game );

	return 0;
}