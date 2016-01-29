#include "MusicManager.h"


le::MusicManager::MusicManager( System & System )
{
    Console = &System.GetConsole();
    Configuration = &System.GetConfiguration();
}

le::MusicManager::~MusicManager()
{
    DeleteAllMusic();
}

void le::MusicManager::LoadMusic( const string sRoute , const string sNameMusic , bool loop )
{
    vMusic.push_back( new le::Music( *Console , sRoute , sNameMusic , loop ) );
}

void le::MusicManager::PlayMusic( const string sNameMusic )
{
    for ( int i = 0; i < vMusic.size(); i++ )
    {
        le::Music* Music = vMusic[ i ];

        if ( Music->sNameMusic == sNameMusic && Configuration->bMusic )
        {
            Music->music.setVolume( Configuration->iVolumeMusic );
            if ( Music->music.getStatus() != sf::Music::Playing ) Music->music.play();
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

le::Music::Music( Console& Console , const string sRoute , const string sNameMusic , bool loop )
{
    if ( !music.openFromFile( sRoute ) )
        Console.WriteToConsole( "Error: File [" + sRoute + "] Not Found" , Color::Red );

    music.setLoop( loop );
    this->sNameMusic = sNameMusic;
}
