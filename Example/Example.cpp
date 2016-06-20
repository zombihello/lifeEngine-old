#include <leSystem/leSystem.h>
#include <leMedia/leMedia.h>

class Game : public le::BasicStagesGame
{
public:
	Game( le::System& System )
	{
		TextManager = new le::TextManager( *System );
		MusicManager = new le::MusicManager( *System );

		TextManager->LoadFont( "Resources/Dikovina.ttf" );
		TextManager->WriteText( "This is WORK!", 25, Vector2f( 10, 10 ), Color::White );
		TextManager->WriteText( "SPARTA!!", 25, Vector2f( 20, 20 ), Color::Red );

		MusicManager->LoadMusic( "Resources/menu.ogg", "MUSIC", true );
	}

	~Game()
	{
		delete MusicManager;
		delete TextManager;
	}

	void CheckStages()
	{
		MusicManager->PlayMusic( "MUSIC" );
		TextManager->UpdateText();
	}

private:
	le::MusicManager* MusicManager;
	le::TextManager* TextManager;
};

int main()
{
	le::System System( "config.cfg" );
	System.SetWindowTitle( "lifeEngine work!", "" );

	Game game( System );
	System.MainLoop( game );

	return 0;
}
