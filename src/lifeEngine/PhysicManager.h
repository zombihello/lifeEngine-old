#ifndef PHYSIC_MANAGER_H
#define PHYSIC_MANAGER_H

////////////////////////
// LIFEENGINE
///////////////////////
#include "BasicEntity.h"
#include "LevelManager.h"

namespace le
{
    class PhysicManager
    {
    public:
        ///////////////////////////////////////////////////
        // CONSTRUCTOR DEFAULT | КОНСТРУКТОР ПО УМОЛЧАНИЮ
        //////////////////////////////////////////////////
        PhysicManager( float& fTime );

        ///////////////////////////////////////////////////
        // DESTRUCTOR DEFAULT | ДЕСТРУКТОР ПО УМОЛЧАНИЮ
        ///////////////////////////////////////////////////
        ~PhysicManager();

        enum TYPE_BODY
        {
            STATIC ,
            DINAMIC
        };

        ///////////////////////////////////////////////////////////////////////////////////
        // UPDATE PHYSIC | ОБНОВЛЕНИЕ ФИЗИКИ
        // num = 0 this check colision for x | num = 0 ЭТО ПРОВЕРКА СТОЛКНОВЕНИЙ ПО ИКСУ
        // num = 1 this check colision for y | num = 1 ЭТО ПРОВЕРКА СТОЛКНОВЕНИЙ ПО ИГРИКУ
        ////////////////////////////////////////////////////////////////////////////////////
        void UpdatePhysic( vector<Object> obj , vector<BasicEntity*> vEntity , FloatRect& RectEntity , int num );

        //////////////////////////////////////////////////////////////////////
        // GET ENTITY COLLIDED WITH | ПОЛУЧИТЬ ЭНТИТИ С КОТОРЫМ СТОЛКНУЛИСЬ
        // @return BasicEntity*
        //////////////////////////////////////////////////////////////////////
        BasicEntity*  GetEntityCollided() const;

        /////////////////////////////////////////////////////////////////////////
        // GET THE OBJECT COLLIDED WITH | ПОЛУЧИТЬ ОБЪЕКТ С КОТОРЫМ СТОЛКНУЛИСЬ
        // @return sNameEntity | string
        ////////////////////////////////////////////////////////////////////////
        Object GetObjectCollided() const;
    protected:
        /////////////////////////////////
        // INIT VALUES | ИНИЦ. ЗНАЧЕНИЙ  
        /////////////////////////////////
        void Option( TYPE_BODY Type = TYPE_BODY::DINAMIC );

        float               fDx;
        float               fDy;

        bool                bOnGround;
    private:
        float*              fTime;

        TYPE_BODY           TypeBody;
        BasicEntity*        EntityCollided;
        Object              ObjectCollided;
    };
}

#endif // PHYSIC_MANAGER_H