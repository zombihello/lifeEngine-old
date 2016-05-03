#include "../Music.h"

//-------------------------------------------------------------------------//

le::Music::Music( string Route, string NameMusic, bool loop )
{
	music.openFromFile( Route );
	music.setLoop( loop );

	sNameMusic = NameMusic;
	bIsPlaying = false;
	iCoefficientVolume = 0;
}

//-------------------------------------------------------------------------//

le::Music::Music( string Route, string NameMusic, int CoefficientVolume, bool loop )
{
	music.openFromFile( Route );
	music.setLoop( loop );
	music.setVolume( music.getVolume() + CoefficientVolume );

	sNameMusic = NameMusic;
	bIsPlaying = false;
	iCoefficientVolume = CoefficientVolume;
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
