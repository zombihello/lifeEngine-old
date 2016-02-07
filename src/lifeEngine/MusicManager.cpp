#include "MusicManager.h"


le::MusicManager::MusicManager( System & System )
{
    Configuration = &System.GetConfiguration();
}

le::MusicManager::~MusicManager()
{
    DeleteAllMusic();
}

void le::MusicManager::LoadMusic( const string sRoute , const string sNameMusic , bool loop )
{
    vMusic.push_back( new le::Music( sRoute , sNameMusic , loop ) );
}

void le::MusicManager::PlayMusic( const string sNameMusic )
{
    for ( int i = 0; i < vMusic.size(); i++ )
    {
        le::Music* Music = vMusic[ i ];

        if ( Music->sNameMusic == sNameMusic && Configuration->bMusic )
        {
            Music->music.setVolume( Configuration->iVolumeMusic );

            if ( !Music->bIsPlaying )
            {
                Music->music.play();
                Music->bIsPlaying = true;
            }
            else if ( Music->music.getStatus() == sf::Music::Stopped )
            {
                delete Music;
                vMusic.erase( vMusic.begin() + i );
            }

        }
        else if ( Music->sNameMusic == sNameMusic && !Configuration->bMusic )
            Music->music.pause();
    }
}

void le::MusicManager::DeleteAllMusic()
{
    for ( int i = 0; i < vMusic.size(); i++ )
        delete vMusic[ i ];

    vMusic.clear();
}

void le::MusicManager::DeleteMusic( const string sNameMusic )
{
    for ( int i = 0; i < vMusic.size(); i++ )
    {
        le::Music* Music = vMusic[ i ];

        if ( Music->sNameMusic == sNameMusic )
        {
            delete vMusic[ i ];
            vMusic.erase( vMusic.begin() + i );
        }
    }
}

bool le::MusicManager::GetLoadedMusic( const string sNameMusic )
{
    for ( int i = 0; i < vMusic.size(); i++ )
        if ( vMusic[ i ]->sNameMusic == sNameMusic )
            return true;

    return false;
}

le::Music::Music( const string sRoute , const string sNameMusic , bool loop )
{
    music.openFromFile( sRoute );

    music.setLoop( loop );
    bIsPlaying = false;
    this->sNameMusic = sNameMusic;
}
