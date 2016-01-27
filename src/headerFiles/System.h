#ifndef SYSTEM_H
#define SYSTEM_H

/////////////
// SYSTEM 
////////////
#include <iostream>
#include <fstream>
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
        // CONSTRUCTOR DEFAULT | йнмярпсйрнп он слнквюмхч
        ///////////////////////////////////////////////////
        System();

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | деярпсйрнп он слнквюмхч
        ///////////////////////////////////////////////////
        ~System();

        //////////////////////////////////////////////////////
        // GET VALUE FROM FILE | онксвхрэ оепелемсч я тюикю
        // @return value ( int, string, ... )
        /////////////////////////////////////////////////////
        template<typename T> T ReadFile( const string sRoute , const string sTagName );

        //////////////////////////////////////////////////////
        // GET VALUE FROM XML FILE | онксвхрэ оепелемсч я XML тюикю
        // @return string
        /////////////////////////////////////////////////////
        string ReadXMLFile( const string sRoute , const string sTagMain , const string sTag );

        //////////////////////////////////////////////////////
        // SAVE VALUE IN FILE | янупюмхрэ оепелемсч б тюикю
        /////////////////////////////////////////////////////
        template<typename T> void SaveFile( const string sRoute , const string sTagName , T Value , bool ClearFile = false );

        //////////////////////////////////////////////////////
        // GET FILE EXISTS | опнбепйю мю ясыеярбнбюмхе тюикю
        // @return true or false
        /////////////////////////////////////////////////////
        bool FileExists( const string sRouteToFile );

        /////////////////////////////////////////////////////////////////
        // GET DIRECTORY EXISTS | опнбепйю мю ясыеярбнбюмхе дхпейрнпхх
        // @return true or false
        ////////////////////////////////////////////////////////////////
        bool DirectoryExists( const string sRouteToDirectory );

        ////////////////////////////////////////////////////////
        // UPDATE FILE CONFIG | намнбкемхе тюикю йнмтхцспюжхи
        ///////////////////////////////////////////////////////
        void UpdateConfig();

        ///////////////////////////////////////////
        // CREATE WINDOW | янгдюмхе нймю
        /////////////////////////////////////////
        void WindowCreate( const int iStyle = Style::Default );

        ///////////////////////////////////////////
        // MAIN LOOP GAME | цкюбмши жхйк хцпш
        /////////////////////////////////////////
        void MainLoop( BasicStageGame& BasicStageGame );

        //////////////////////////////////////////////
        // SET TITLE WINDOW | гюдюрэ мюгбюмхе нймс
        //////////////////////////////////////////////
        void SetWindowTitle( const String WindowName , const String GameVersion );

        //////////////////////////////////////////////
        // SET GAME SPEED  | гюдюрэ  яйнпнярэ хцпш
        //////////////////////////////////////////////
        void SetGameSpeed( const float fGameSpeed );

        //////////////////////////////////////////////
        // SET GAME TICK  | гюдюрэ рхй хцпш
        //////////////////////////////////////////////
        void SetGameTick( const float fGameTick );

        //////////////////////////////////////////////
        // GET CONFIGURATION | онксвхрэ йнмтхцспюжхх
        // @return Configuration
        //////////////////////////////////////////////
        Configuration GetConfiguration() const;

        //////////////////////////////////////////////
        // GET TYPE EVENT | онксвхрэ рхо янашрхъ
        // @return TypeEvent
        //////////////////////////////////////////////
        Event::EventType GetTypeEvent() const;

        //////////////////////////////////////////////
        // GET WINDOW | онксвхрэ нймн
        // @return RenderWindow
        //////////////////////////////////////////////
        RenderWindow& GetWindow();

        //////////////////////////////////////////////
        // GET EVENT | онксвхрэ янашрхе
        // @return Event
        //////////////////////////////////////////////
        Event GetEvent() const;

        /////////////////////////////////////////////
        // GET MOUSE CURSOR | онксвхрэ йспянп лшьх
        // @return MouseCursor
        /////////////////////////////////////////////
        MouseCursor& GetMouseCursor();
    private:
        template<typename T> T InitVar();
        void clock();

        Configuration       Configuration;
        Event::EventType    TypeEvent;
        RenderWindow        RenderWindow;
        Event               Event;
        MouseCursor         MouseCursor;
        Clock               Clock;
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
