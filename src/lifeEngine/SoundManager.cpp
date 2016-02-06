#include "SoundManager.h"


le::SoundManager::SoundManager( System & System )
{
    Configuration = &System.GetConfiguration();
}

le::SoundManager::~SoundManager()
{
    DeleteAllSound();
}

void le::SoundManager::LoadSound( const string sRoute , const string sNameSound )
{
    vSound.push_back( new le::Sound( sRoute , sNameSound ) );
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

bool le::SoundManager::GetLoadedSound( const string sNameSound )
{
    for ( int i = 0; i < vSound.size(); i++ )
        if ( vSound[ i ]->sNameSound == sNameSound )
            return true;

    return false;
}

le::Sound::Sound( const string sRoute , const string sNameSound )
{
    SoundBuffer.loadFromFile( sRoute );

    sound.setBuffer( SoundBuffer );
    this->sNameSound = sNameSound;
}
