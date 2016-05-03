#include "../Sound.h"

//-------------------------------------------------------------------------//

le::Sound::Sound( string Route, string NameSound )
{
	SoundBuffer.loadFromFile( Route );
	sound.setBuffer( SoundBuffer );

	sNameSound = NameSound;
	iCoefficientVolume = 0;
}

//-------------------------------------------------------------------------//

le::Sound::Sound( string Route, string NameSound, int CoefficientVolume )
{
	SoundBuffer.loadFromFile( Route );
	sound.setBuffer( SoundBuffer );
	sound.setVolume( sound.getVolume() + CoefficientVolume );

	sNameSound = NameSound;
	iCoefficientVolume = CoefficientVolume;
}

//-------------------------------------------------------------------------//

void le::Sound::Play()
{
	sound.play();
}

//-------------------------------------------------------------------------//

void le::Sound::Stop()
{
	sound.stop();
}

//-------------------------------------------------------------------------//

string le::Sound::GetNameSound()
{
	return sNameSound;
}

//-------------------------------------------------------------------------//

int le::Sound::GetCoefficientVolume()
{
	return iCoefficientVolume;
}

//-------------------------------------------------------------------------//

sf::SoundBuffer& le::Sound::GetBufferSound()
{
	return SoundBuffer;
}

//-------------------------------------------------------------------------//

sf::Sound& le::Sound::GetSound()
{
	return sound;
}

//-------------------------------------------------------------------------//
