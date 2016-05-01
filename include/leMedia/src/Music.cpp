#include "../Music.h"

//-------------------------------------------------------------------------//

le::Music::Music( string Route, string NameMusic, bool loop )
{
	music.openFromFile( Route );
	music.setLoop( loop );

	sNameMusic = NameMusic;
	bIsPlaying = false;
}

//-------------------------------------------------------------------------//

void le::Music::Play()
{
	music.play();
	bIsPlaying = true;
}

//-------------------------------------------------------------------------//

void le::Music::Stop()
{
	music.stop();
	bIsPlaying = false;
}

//-------------------------------------------------------------------------//

bool le::Music::IsPlaying()
{
	return bIsPlaying;
}

//-------------------------------------------------------------------------//

string le::Music::GetNameMusic()
{
	return sNameMusic;
}

//-------------------------------------------------------------------------//

sf::Music& le::Music::GetMusic()
{
	return music;
}

//-------------------------------------------------------------------------//
