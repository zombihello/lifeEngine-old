#include "../Sound.h"
#include "../../Entity/BasicEntity.h"

//-------------------------------------------------------------------------//

le::Sound::Sound( string Route , string NameSound )
{
	SoundBuffer.loadFromFile( Route );
	sound.setBuffer( SoundBuffer );

	sNameSound = NameSound;
	iCoefficientVolume = 0;
	Entity = NULL;

	sound.setPosition( 25 , 25 , 0 );
	sound.setMinDistance( 5.f );
	sound.setAttenuation( 10.f );
}

//-------------------------------------------------------------------------//

le::Sound::Sound( string Route , string NameSound , Vector2f Position , float MinDistance , float Attenuation )
{
	SoundBuffer.loadFromFile( Route );
	sound.setBuffer( SoundBuffer );

	sNameSound = NameSound;
	iCoefficientVolume = 0;
	Entity = NULL;

	sound.setPosition( Position.x , Position.y , 0 );
	sound.setMinDistance( MinDistance );
	sound.setAttenuation( Attenuation );
}

//-------------------------------------------------------------------------//

le::Sound::Sound( string Route , string NameSound , BasicEntity& Entity , float MinDistance , float Attenuation )
{
	SoundBuffer.loadFromFile( Route );
	sound.setBuffer( SoundBuffer );

	sNameSound = NameSound;
	iCoefficientVolume = 0;
	this->Entity = &Entity;

	sound.setPosition( Entity.GetRect().left , Entity.GetRect().top , 0 );
	sound.setMinDistance( MinDistance );
	sound.setAttenuation( Attenuation );
}

//-------------------------------------------------------------------------//

le::Sound::Sound( string Route , string NameSound , Vector2f Position , float MinDistance , float Attenuation , int CoefficientVolume )
{
	SoundBuffer.loadFromFile( Route );
	sound.setBuffer( SoundBuffer );
	sound.setVolume( sound.getVolume() + CoefficientVolume );

	sNameSound = NameSound;
	iCoefficientVolume = CoefficientVolume;
	Entity = NULL;

	sound.setPosition( Position.x , Position.y , 0 );
	sound.setMinDistance( MinDistance );
	sound.setAttenuation( Attenuation );
}

//-------------------------------------------------------------------------//

le::Sound::Sound( string Route , string NameSound , BasicEntity& Entity , float MinDistance , float Attenuation , int CoefficientVolume )
{
	SoundBuffer.loadFromFile( Route );
	sound.setBuffer( SoundBuffer );
	sound.setVolume( sound.getVolume() + CoefficientVolume );

	sNameSound = NameSound;
	iCoefficientVolume = CoefficientVolume;
	this->Entity = &Entity;

	sound.setPosition( Entity.GetRect().left , Entity.GetRect().top , 0 );
	sound.setMinDistance( MinDistance );
	sound.setAttenuation( Attenuation );
}

//-------------------------------------------------------------------------//

le::Sound::Sound( string Route , string NameSound , int CoefficientVolume )
{
	SoundBuffer.loadFromFile( Route );
	sound.setBuffer( SoundBuffer );
	sound.setVolume( sound.getVolume() + CoefficientVolume );

	sNameSound = NameSound;
	iCoefficientVolume = CoefficientVolume;
}

//-------------------------------------------------------------------------//

void le::Sound::UpdatePositionSound()
{
	if ( Entity != NULL )
		sound.setPosition( Entity->GetRect().left , Entity->GetRect().top , 0 );
}

//-------------------------------------------------------------------------//

void le::Sound::UpdatePositionSound( BasicEntity& Entity )
{
	sound.setPosition( Entity.GetRect().left , Entity.GetRect().top , 0 );
}

//-------------------------------------------------------------------------//

void le::Sound::UpdatePositionSound( Vector2f Position )
{
	sound.setPosition( Position.x , Position.y , 0 );
}

//-------------------------------------------------------------------------//

void le::Sound::Play()
{
	sound.play();
}

//-------------------------------------------------------------------------//

void le::Sound::Pause()
{
	sound.pause();
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

bool le::Sound::IsPlaying()
{
	if ( sound.getStatus() == sf::Sound::Playing )
		return true;
	return false;
}

//-------------------------------------------------------------------------//