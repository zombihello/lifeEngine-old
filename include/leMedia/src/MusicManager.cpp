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

void le::MusicManager::PlayMusic( string NameMusic )
{
	if ( vMusic.count( NameMusic ) != 0 && Configuration->bMusic )
	{
		le::Music* Music = vMusic[ NameMusic ];

		Music->GetMusic().setVolume( Configuration->iVolumeMusic );

		if ( !Music->IsPlaying() )
			Music->Play();
		else if ( Music->GetMusic().getStatus() == sf::Music::Stopped )
		{
			vMusic.erase( vMusic.find( NameMusic ) );
			delete Music;
		}
	}
	else if ( vMusic.count( NameMusic ) != 0 && !Configuration->bMusic )
		vMusic[ NameMusic ]->Stop();
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
