#include "../SoundManager.h"
#include "../../Entity/BasicEntity.h"

//-------------------------------------------------------------------------//

le::SoundManager::SoundManager( le::System& System )
{
	Configuration = &System.GetConfiguration();
}

//-------------------------------------------------------------------------//

le::SoundManager::~SoundManager()
{
	DeleteAllSound();
}

//-------------------------------------------------------------------------//

void le::SoundManager::LoadSound( string Route , string NameSound )
{
	vSound[ NameSound ] = new le::Sound( Route , NameSound );
}

//-------------------------------------------------------------------------//

void le::SoundManager::LoadSound( string Route , string NameSound , Vector2f Position , float MinDistance , float Attenuation )
{
	vSound[ NameSound ] = new le::Sound( Route , NameSound , Position , MinDistance , Attenuation );
}

//-------------------------------------------------------------------------//

void le::SoundManager::LoadSound( string Route , string NameSound , BasicEntity& Entity , float MinDistance , float Attenuation )
{
	vSound[ NameSound ] = new le::Sound( Route , NameSound , Entity , MinDistance , Attenuation );
}

//-------------------------------------------------------------------------//

void le::SoundManager::LoadSound( string Route , string NameSound , Vector2f Position , float MinDistance , float Attenuation , int CoefficientVolume )
{
	vSound[ NameSound ] = new le::Sound( Route , NameSound , Position , MinDistance , Attenuation , CoefficientVolume );
}

//-------------------------------------------------------------------------//

void le::SoundManager::LoadSound( string Route , string NameSound , BasicEntity& Entity , float MinDistance , float Attenuation , int CoefficientVolume )
{
	vSound[ NameSound ] = new le::Sound( Route , NameSound , Entity , MinDistance , Attenuation , CoefficientVolume );
}

//-------------------------------------------------------------------------//

void le::SoundManager::LoadSound( string Route , string NameSound , int CoefficientVolume )
{
	vSound[ NameSound ] = new le::Sound( Route , NameSound , CoefficientVolume );
}

//-------------------------------------------------------------------------//

void le::SoundManager::PlaySound( string NameSound )
{
	if ( vSound.count( NameSound ) != 0 && Configuration->bSound )
	{
		le::Sound* Sound = vSound[ NameSound ];

		float percentage = Configuration->iVolumeSound * abs( Sound->GetCoefficientVolume() ) / 100;

		if ( Sound->GetCoefficientVolume() < 0 )
			Sound->GetSound().setVolume( Configuration->iVolumeSound - percentage );
		else
			Sound->GetSound().setVolume( Configuration->iVolumeSound + percentage );

		Sound->UpdatePositionSound();

		if ( !Sound->IsPlaying() )
			Sound->Play();
	}
	else if ( vSound.count( NameSound ) != 0 && !Configuration->bSound )
		vSound[ NameSound ]->Stop();
}

//-------------------------------------------------------------------------//

void le::SoundManager::DeleteAllSound()
{
	for ( Iterator = vSound.begin(); Iterator != vSound.end(); Iterator++ )
		delete Iterator->second;

	vSound.clear();
}

//-------------------------------------------------------------------------//

void le::SoundManager::DeleteSound( string NameSound )
{
	if ( vSound.count( NameSound ) != 0 )
	{
		le::Sound* Sound = vSound[ NameSound ];
		vSound.erase( vSound.find( NameSound ) );
		delete Sound;
	}
}

//-------------------------------------------------------------------------//

bool le::SoundManager::GetLoadedSound( string NameSound )
{
	if ( vSound.count( NameSound ) != 0 )
		return true;

	return false;
}

//-------------------------------------------------------------------------//

bool le::SoundManager::IsPlaying( string NameSound )
{
	if ( vSound.count( NameSound ) != 0 )
	{
		le::Sound* Sound = vSound[ NameSound ];
		return Sound->IsPlaying();
	}

	return false;
}

//-------------------------------------------------------------------------//
