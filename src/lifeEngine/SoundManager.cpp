#include "SoundManager.h"


le::SoundManager::SoundManager( System & System )
{
    Console = &System.GetConsole();
    Configuration = &System.GetConfiguration();
}

le::SoundManager::~SoundManager()
{
    DeleteAllSound();
}

void le::SoundManager::LoadSound( const string sRoute , const string sNameSound )
{
    vSound.push_back( new le::Sound( *Console , sRoute , sNameSound ) );
}

void le::SoundManager::PlaySound( const string sNameSound )
{
    for ( int i = 0; i < vSound.size(); i++ )
    {
        le::Sound* Sound = vSound[ i ];

        if ( Sound->sNameSound == sNameSound && Configuration->bSound )
        {
            Sound->sound.setVolume( Configuration->iVolumeSound );
            if ( Sound->sound.getStatus() != sf::Sound::Playing ) Sound->sound.play();
        }
        else if ( Sound->sNameSound == sNameSound && !Configuration->bSound )
            Sound->sound.pause();
    }
}

void le::SoundManager::DeleteAllSound()
{
    for ( int i = 0; i < vSound.size(); i++ )
        delete vSound[ i ];

    vSound.clear();
}

void le::SoundManager::DeleteSound( const string sNameSound )
{
    for ( int i = 0; i < vSound.size(); i++ )
    {
        le::Sound* Sound = vSound[ i ];

        if ( Sound->sNameSound == sNameSound )
        {
            delete vSound[ i ];
            vSound.erase( vSound.begin() + i );
        }
    }
}

le::Sound::Sound( Console& Console , const string sRoute , const string sNameSound )
{
    if ( !SoundBuffer.loadFromFile( sRoute ) )
        Console.WriteToConsole( "Error: File [" + sRoute + "] Not Found" , Color::Red );

    sound.setBuffer( SoundBuffer );
    this->sNameSound = sNameSound;
}
