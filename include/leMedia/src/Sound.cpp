#include "../Sound.h"

//-------------------------------------------------------------------------//

le::Sound::Sound( string Route, string NameSound )
{
	SoundBuffer.loadFromFile( Route );
	sound.setBuffer( SoundBuffer );

	sNameSound = NameSound;
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
