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
        ///////////////////////////////////////
        // UPDATE ENTITY  | ОБНОВЛЕНИЕ ЭНТИТИ 
        //////////////////////////////////////
        virtual void UpdateEntity( vector<le::Object> obj , vector<BasicEntity*> vEntity  ) = 0;

        ///////////////////////////////////////
        // GET HEALTCH | ПОЛУЧИТЬ ЗДОРОВЬЕ  
        // @return iHealtch [ int ]
        //////////////////////////////////////
        int GetHealtch() const;

        ///////////////////////////////////////
        // GET ARMOR | ПОЛУЧИТЬ БРОНЮ  
        // @return iArmor [ int ]
        //////////////////////////////////////
        int GetArmor() const;

        ///////////////////////////////////////
        // GET LIFE | ПОЛУЧИТЬ ЖИЗНЬ  
        // @return bLife [ bool ]
        //////////////////////////////////////
        bool GetLife() const;

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
    protected:
        /////////////////////////////////
        // INIT VALUES | ИНИЦ. ЗНАЧЕНИЙ  
        /////////////////////////////////
        void Option( const string sName , const int Healtch , const int Armor );

        /////////////////////////////////////////
        // CHARACTER WOUNDED | ПЕРСОНАЖА РАНИЛИ  
        ////////////////////////////////////////
       // void Tick();

        int                 iHealtch;
        int                 iArmor;

        float               fTimer;

        bool                bLife;
        bool                bTick;

        string              sNameEntity;
        FloatRect           Rect;
        Sprite              Sprite;
        Texture             Texture;
    };
}

#endif //BASIC_ENTITY