#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

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

    class Music
    {
    public:
        Music( const string sRoute , const string sNameMusic , bool loop = false );

        bool                bIsPlaying;

        string              sNameMusic;
        sf::Music           music;
    };

    class MusicManager
    {
    public:
        /////////////////////////////////
        // CONSTRUCTOR | КОНСТРУКТОР  
        ///////////////////////////////
        MusicManager( System& System );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~MusicManager();

        ///////////////////////////////////
        // LOAD MUSIC  | ЗАГРУЗИТЬ МУЗЫКУ   
        //////////////////////////////////
        void LoadMusic( const string sRoute , const string sNameMusic , bool loop = false );

        ///////////////////////////////////
        // PLAY MUSIC | ИГРАТЬ МУЗЫКУ   
        //////////////////////////////////
        void PlayMusic( const string sNameMusic );

        /////////////////////////////////////////
        // DELETE ALL MUSIC | УДАЛИТЬ ВСЮ МУЗЫКУ
        ////////////////////////////////////////
        void DeleteAllMusic();

        ///////////////////////////////////
        // DELETE MUSIC | УДАЛИТЬ МУЗЫКУ
        //////////////////////////////////
        void DeleteMusic( const string sNameMusic );

        ////////////////////////////////////////////////////////////
        // TO GET THE LOADED MUSIC  | ПОЛУЧИТЬ ЗАГРУЖЕНА ЛИ МУЗЫКА
        // @return true or false
        ///////////////////////////////////////////////////////////
        bool GetLoadedMusic( const string sNameMusic );
    private:
        Configuration*              Configuration;
        vector<le::Music*>          vMusic;
    };
}

#endif // MUSIC_MANAGER_H