#ifndef SYSTEM_H
#define SYSTEM_H

/////////////
// SYSTEM 
////////////
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
using namespace std;

/////////////////
// SFML
////////////////
#include <SFML\Graphics.hpp>
using namespace sf;

///////////////
// XML
//////////////
#include "../XML/tinyxml.h"

//////////////////
// LIFEENGINE
/////////////////
#include "BasicStageGame.h"
#include "MouseCursor.h"
#include "Console.h"

namespace le
{

    struct Configuration
    {
        Configuration();

        int         iWindowWidth;
        int         iWindowHeight;
        int         iVolumeSound;
        int         iVolumeMusic;

        float       fGameSpeed;
        float       fGameTick;
        float       fTime;

        bool        bSound;
        bool        bMusic;
        bool        bFullscreen;
        bool        bV_Sinc;

        String      sWindowName;
        String      sGameVersion;
    };

    class System
    {
    public:
        ///////////////////////////////////////////////////
        // CONSTRUCTOR DEFAULT | КОНСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        System( const string sRouteToFontForConsole );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~System();

        //////////////////////////////////////////////////////
        // GET VALUE FROM FILE | ПОЛУЧИТЬ ПЕРЕМЕННУЮ С ФАЙЛА
        // @return value ( int, string, ... )
        /////////////////////////////////////////////////////
        template<typename T> T ReadFile( const string sRoute , const string sTagName );

        //////////////////////////////////////////////////////
        // GET VALUE FROM XML FILE | ПОЛУЧИТЬ ПЕРЕМЕННУЮ С XML ФАЙЛА
        // @return string
        /////////////////////////////////////////////////////
        string ReadXMLFile( const string sRoute , const string sTagMain , const string sTag );

        //////////////////////////////////////////////////////
        // SAVE VALUE IN FILE | СОХРАНИТЬ ПЕРЕМЕННУЮ В ФАЙЛ
        /////////////////////////////////////////////////////
        template<typename T> void SaveFile( const string sRoute , const string sTagName , T Value , bool ClearFile = false );

        //////////////////////////////////////////////////////
        // GET FILE EXISTS | ПРОВЕРКА НА СУЩЕСТВОВАНИЕ ФАЙЛА
        // @return true or false
        /////////////////////////////////////////////////////
        bool FileExists( const string sRouteToFile );

        /////////////////////////////////////////////////////////////////
        // GET DIRECTORY EXISTS | ПРОВЕРКА НА СУЩЕСТВОВАНИЕ ДИРЕКТОРИИ
        // @return true or false
        ////////////////////////////////////////////////////////////////
        bool DirectoryExists( const string sRouteToDirectory );

        ////////////////////////////////////////////////////////
        // UPDATE FILE CONFIG | ОБНОВЛЕНИЕ ФАЙЛА КОНФИГУРАЦИЙ
        ///////////////////////////////////////////////////////
        void UpdateConfig();

        ///////////////////////////////////////////
        // CREATE WINDOW | СОЗДАНИЕ ОКНА
        /////////////////////////////////////////
        void WindowCreate( const int iStyle = Style::Default );

        ///////////////////////////////////////////
        // MAIN LOOP GAME | ГЛАВНЫЙ ЦИКЛ ИГРЫ
        /////////////////////////////////////////
        void MainLoop( BasicStageGame& BasicStageGame );

        //////////////////////////////////////////////
        // SET TITLE WINDOW | ЗАДАТЬ НАЗВАНИЕ ОКНА
        //////////////////////////////////////////////
        void SetWindowTitle( const String WindowName , const String GameVersion );

        //////////////////////////////////////////////
        // SET GAME SPEED  | ЗАДАТЬ СКОРОСТЬ ИГРЫ
        //////////////////////////////////////////////
        void SetGameSpeed( const float fGameSpeed );

        //////////////////////////////////////////////
        // SET GAME TICK  | ЗАДАТЬ ТИК ИГРЫ
        //////////////////////////////////////////////
        void SetGameTick( const float fGameTick );

        //////////////////////////////////////////////
        // GET CONFIGURATION | ПОЛУЧИТЬ КОНФИГУРАЦИИ
        // @return Configuration
        //////////////////////////////////////////////
        Configuration& GetConfiguration();

        //////////////////////////////////////////////
        // GET TYPE EVENT | ПОЛУЧИТЬ ТИП СОБЫТИЯ
        // @return TypeEvent
        //////////////////////////////////////////////
        Event::EventType GetTypeEvent() const;

        //////////////////////////////////////////////
        // GET WINDOW | ПОЛУЧИТЬ ОКНО
        // @return RenderWindow
        //////////////////////////////////////////////
        RenderWindow& GetWindow();

        //////////////////////////////////////////////
        // GET EVENT | ПОЛУЧИТЬ СОБЫТИЕ
        // @return Event
        //////////////////////////////////////////////
        Event GetEvent() const;

        /////////////////////////////////////////////
        // GET MOUSE CURSOR | ПОЛУЧИТЬ КУРСОР МЫШИ
        // @return MouseCursor
        /////////////////////////////////////////////
        MouseCursor& GetMouseCursor();

        ////////////////////////////////////
        // GET CONSOLE | ПОЛУЧИТЬ КОНСОЛЬ 
        // @return Console
        ///////////////////////////////////
        Console& GetConsole();
    private:
        template<typename T> T InitVar();
        void clock();

        Configuration       Configuration;
        Event::EventType    TypeEvent;
        RenderWindow        RenderWindow;
        Event               Event;
        MouseCursor         MouseCursor;
        Clock               Clock;
        Console*            Console;
    };



    template<typename T> T System::ReadFile( const string sRoute , const string sTagName )
    {
        T Value = InitVar<T>();

        if ( FileExists( sRoute ) ) // if file exists
        {
            ifstream fis( sRoute ); // open file
            string sString;

            for ( int i = 0; !fis.eof(); i++ ) // read file
            {
                fis >> sString;

                if ( sString == sTagName )
                {
                    fis >> Value; // Save word or number in Value
                    return Value;
                }

                if ( fis.eof() ) // if not found tag and already end file
                    cout << "Error: Tag [" << sTagName << "] Not Found In File [" << sRoute << "]\n";
            }
        }
        else
            cout << "Error: File [" << sRoute << "] Not Found\n";

        return Value;
    }

    template<typename T> void System::SaveFile( const string sRoute , const string sTagName , T Value , bool ClearFile )
    {
        ofstream fos( sRoute , ios::app );
        if ( ClearFile ) ofstream fos( sRoute );
        fos << sTagName << " " << Value << endl;
        fos.close();
    }

    template<typename T> inline T System::InitVar()
    {
        return 0;
    }

    template<> inline string System::InitVar<string>()
    {
        return "ERROR";
    }
}

#endif //SYSTEM_H
