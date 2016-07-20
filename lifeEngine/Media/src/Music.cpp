#include "../Music.h"

//-------------------------------------------------------------------------//

le::Music::Music( string Route, string NameMusic, bool loop )
{
	music.openFromFile( Route );
	music.setLoop( loop );

	sNameMusic = NameMusic;
	iCoefficientVolume = 0;
}

//-------------------------------------------------------------------------//

le::Music::Music( string Route, string NameMusic, int CoefficientVolume, bool loop )
{
	music.openFromFile( Route );
	music.setLoop( loop );
	music.setVolume( music.getVolume() + CoefficientVolume );
	
	sNameMusic = NameMusic;
	iCoefficientVolume = CoefficientVolume;
}

//-------------------------------------------------------------------------//

void le::Music::Play()
{
	music.play();
}

//-------------------------------------------------------------------------//

void le::Music::Pause()
{
	music.pause();
}

//-------------------------------------------------------------------------//

void le::Music::Stop()
{
	music.stop();
}

//-------------------------------------------------------------------------//

bool le::Music::IsPlaying()
{
	if ( music.getStatus() == sf::Music::Playing )
		return true;
	return false;
}

//-------------------------------------------------------------------------//

string le::Music::GetNameMusic()
{
	return sNameMusic;
}

//-------------------------------------------------------------------------//

int le::Music::GetCoefficientVolume()
{
	return iCoefficientVolume;
}

//-------------------------------------------------------------------------//

sf::Music& le::Music::GetMusic()
{
	return music;
}

//-------------------------------------------------------------------------//
