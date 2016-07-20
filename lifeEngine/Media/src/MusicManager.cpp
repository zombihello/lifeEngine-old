#include "../MusicManager.h"

//-------------------------------------------------------------------------//

le::MusicManager::MusicManager( le::System& System )
{
	Configuration = &System.GetConfiguration();
}

//-------------------------------------------------------------------------//

le::MusicManager::~MusicManager()
{
	DeleteAllMusic();
}

//-------------------------------------------------------------------------//

void le::MusicManager::LoadMusic( string Route, string NameMusic, bool loop )
{
	vMusic[ NameMusic ] = new le::Music( Route, NameMusic, loop );
}

//-------------------------------------------------------------------------//

void le::MusicManager::LoadMusic( string Route, string NameMusic, int CoefficientVolume, bool loop )
{
	vMusic[ NameMusic ] = new le::Music( Route, NameMusic, CoefficientVolume, loop );
}

//-------------------------------------------------------------------------//

void le::MusicManager::PlayMusic( string NameMusic )
{
	if ( vMusic.count( NameMusic ) != 0 && Configuration->bMusic )
	{
		le::Music* Music = vMusic[ NameMusic ];

		float percentage = Configuration->iVolumeMusic * abs( Music->GetCoefficientVolume() ) / 100;

		if ( Music->GetCoefficientVolume() < 0 )
			Music->GetMusic().setVolume( Configuration->iVolumeMusic - percentage );
		else
			Music->GetMusic().setVolume( Configuration->iVolumeMusic + percentage );

		if ( !Music->IsPlaying() )
			Music->Play();
	}
	else if ( vMusic.count( NameMusic ) != 0 && !Configuration->bMusic )
		vMusic[ NameMusic ]->Stop();
}

//-------------------------------------------------------------------------//

void le::MusicManager::StopMusic( string NameMusic )
{
	if ( vMusic.count( NameMusic ) != 0 )
	{
		le::Music* Music = vMusic[ NameMusic ];
		Music->Stop();
	}
}

//-------------------------------------------------------------------------//

void le::MusicManager::StopMusic()
{
	for ( auto it = vMusic.begin(); it != vMusic.end(); it++ )
		(*it).second->Stop();
}

//-------------------------------------------------------------------------//

void le::MusicManager::PauseMusic( string NameMusic )
{
	if ( vMusic.count( NameMusic ) != 0 )
	{
		le::Music* Music = vMusic[ NameMusic ];
		Music->Pause();
	}
}

//-------------------------------------------------------------------------//

void le::MusicManager::PauseMusic()
{
	for ( auto it = vMusic.begin(); it != vMusic.end(); it++ )
		(*it).second->Pause();
}

//-------------------------------------------------------------------------//

void le::MusicManager::DeleteAllMusic()
{
	for ( Iterator = vMusic.begin(); Iterator != vMusic.end(); Iterator++ )
		delete Iterator->second;

	vMusic.clear();
}

//-------------------------------------------------------------------------//

void le::MusicManager::DeleteMusic( string NameMusic )
{
	if ( vMusic.count( NameMusic ) != 0 )
	{
		le::Music* Music = vMusic[ NameMusic ];
		vMusic.erase( vMusic.find( NameMusic ) );
		delete Music;
	}
}

//-------------------------------------------------------------------------//

bool le::MusicManager::GetLoadedMusic( string NameMusic )
{
	if ( vMusic.count( NameMusic ) != 0 )
		return true;

	return false;
}

//-------------------------------------------------------------------------//

bool le::MusicManager::IsPlaying( string NameMusic )
{
	if ( vMusic.count( NameMusic ) != 0 )
	{
		le::Music* Music = vMusic[ NameMusic ];
		return Music->IsPlaying();
	}
}

//-------------------------------------------------------------------------//
