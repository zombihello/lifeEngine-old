#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

//////////////
// SFML
//////////////
#include <SFML\Audio.hpp>
using namespace sf;

/////////////////
// LIFEENGINE
////////////////
#include "System.h"

namespace le
{
    class Sound
    {
    public:
        Sound( Console& Console , const string sRoute , const string sNameSound );

        string              sNameSound;
        SoundBuffer         SoundBuffer;
        sf::Sound           sound;
    };

    class SoundManager
    {
    public:
        /////////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР  
        ///////////////////////////////
        SoundManager( System& System );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~SoundManager();

        ///////////////////////////////////
        // LOAD SOUND  | ЗАГРУЗИТЬ ЗВУК   
        //////////////////////////////////
        void LoadSound( const string sRoute , const string sNameSound );

        ///////////////////////////////////
        // PLAY SOUND | ПРОИГРАТЬ ЗВУК   
        //////////////////////////////////
        void PlaySound( const string sNameSound );

        /////////////////////////////////////////
        // DELETE ALL SOUND | УДАЛИТЬ ВСЕ ЗВУКИ
        ////////////////////////////////////////
        void DeleteAllSound();

        ///////////////////////////////////
        // DELETE SOUND | УДАЛИТЬ ЗВУК
        //////////////////////////////////
        void DeleteSound( const string sNameSound );
    private:
        Console*                    Console;
        Configuration*              Configuration;
        vector<le::Sound*>          vSound;
    };
}

#endif // MUSIC_MANAGER_H