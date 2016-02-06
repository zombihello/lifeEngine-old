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
        Sound( const string sRoute , const string sNameSound );

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

        //////////////////////////////////////////////////////////
        // TO GET THE LOADED SOUND  | ПОЛУЧИТЬ ЗАГРУЖЕН ЛИ ЗВУК
        // @return true or false
        /////////////////////////////////////////////////////////
        bool GetLoadedSound( const string sNameSound );
    private:
        Configuration*              Configuration;
        vector<le::Sound*>          vSound;
    };
}

#endif // MUSIC_MANAGER_H