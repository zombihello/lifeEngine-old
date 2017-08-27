#include <System\System.h>
#include <System\ResourcesManager.h>

class Game : public le::BasicApplication
{
public:
	Game( le::System& System ) : le::BasicApplication( System )
	{
		le::ResourcesManager::LoadGlTexture( "Door1", "../textures/1.jpg" );
		le::ResourcesManager::LoadGlTexture( "Door1", "../textures/1.jpg" );
		le::ResourcesManager::LoadGlTexture( "Door2", "../textures/2.jpg" );
		le::ResourcesManager::LoadGlTexture( "Door3", "../textures/13.jpg" );
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