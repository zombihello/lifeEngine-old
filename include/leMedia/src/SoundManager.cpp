#include "../SoundManager.h"

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

void le::SoundManager::LoadSound( string Route, string NameSound )
{
	vSound[ NameSound ] = new le::Sound( Route, NameSound );
}

//-------------------------------------------------------------------------//

void le::SoundManager::PlaySound( string NameSound )
{
	if ( vSound.count( NameSound ) != 0 && Configuration->bSound )
	{
		le::Sound* Sound = vSound[ NameSound ];

		Sound->GetSound().setVolume( Configuration->iVolumeSound );

		if ( !Sound->GetSound().getStatus() != sf::Sound::Playing )
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
