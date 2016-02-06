#ifndef BASIC_ENTITY
#define BASIC_ENTITY

//////////////////////
// LIFEENGINE
/////////////////////
#include "System.h"
#include "LevelManager.h"

namespace le
{
    class BasicEntity
    {
    public:
        ///////////////////////////////////////////////////
        // CONSTRUCTOR DEFAULT | КОНСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        BasicEntity( System& System );

        enum TYPES_ENTITY
        {
            ENTITY ,
            BULLET ,
            ITEM
        };

        ///////////////////////////////////////
        // UPDATE ENTITY  | ОБНОВЛЕНИЕ ЭНТИТИ 
        //////////////////////////////////////
        virtual void UpdateEntity( vector<le::Object> obj , vector<BasicEntity*>& vEntity ) = 0;

        ///////////////////////////////////////
        // GET HEALTCH | ПОЛУЧИТЬ ЗДОРОВЬЕ  
        // @return iHealtch [ int ]
        //////////////////////////////////////
        int& GetHealtch();

        ///////////////////////////////////////
        // GET ARMOR | ПОЛУЧИТЬ БРОНЮ  
        // @return iArmor [ int ]
        //////////////////////////////////////
        int& GetArmor();

        ///////////////////////////////////////
        // GET LIFE | ПОЛУЧИТЬ ЖИЗНЬ  
        // @return bLife [ bool ]
        //////////////////////////////////////
        bool& GetLife();

        ///////////////////////////////////////
        // GET TICK | ПОЛУЧИТЬ ТИК  
        // @return bTick [ bool ]
        //////////////////////////////////////
        bool GetTick() const;

        //////////////////////////////////////////
        // GET NAME ENTITY | ПОЛУЧИТЬ ИМЯ ЭНТИТИ  
        // @return sNameEntity [ string ]
        /////////////////////////////////////////
        string GetNameEntity() const;

        /////////////////////////////////////////////////////
        // GET RECT ENTITY | ПОЛУЧИТЬ ПРЯМОУГОЛЬНИК ЭНТИТИ  
        // @return Rect [ FloatRect ]
        ////////////////////////////////////////////////////
        FloatRect GetRect() const;

        /////////////////////////////////////////////////////
        // GET TYPE ENTITY | ПОЛУЧИТЬ ТИП ЭНТИТИ  
        // @return TypeEntity [ TYPES_ENTITY ]
        ////////////////////////////////////////////////////
        TYPES_ENTITY GetTypeEntity() const;
    protected:
        /////////////////////////////////
        // INIT VALUES | ИНИЦ. ЗНАЧЕНИЙ  
        /////////////////////////////////
        void Option( const string sName , const int Healtch , const int Armor, TYPES_ENTITY Type = TYPES_ENTITY::ENTITY );

        /////////////////////////////////////////
        // CHARACTER WOUNDED | ПЕРСОНАЖА РАНИЛИ  
        ////////////////////////////////////////
       // void Tick();

        int                 iHealtch;
        int                 iArmor;

        float               fTimer;
        float*              fTime;

        bool                bLife;
        bool                bTick;

        string              sNameEntity;
        TYPES_ENTITY        TypeEntity;
        FloatRect           Rect;
        Sprite              Sprite;
        Texture             Texture;
        RenderWindow*       RenderWindow;
    };
}

#endif //BASIC_ENTITY