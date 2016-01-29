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

        ///////////////////////////////////////////////////////////////////////////////////
        // UPDATE PHYSIC | ОБНОВЛЕНИЕ ФИЗИКИ
        // num = 0 this check colision for x | num = 0 ЭТО ПРОВЕРКА СТОЛКНОВЕНИЙ ПО ИКСУ
        // num = 1 this check colision for y | num = 1 ЭТО ПРОВЕРКА СТОЛКНОВЕНИЙ ПО ИГРИКУ
        ////////////////////////////////////////////////////////////////////////////////////
        void UpdatePhysic( vector<Object> obj , vector<BasicEntity*> vEntity , FloatRect& RectEntity , int num );

        //////////////////////////////////////////////////////////////////////////////////////////
        // GET THE NAME OF THE ENTITY COLLIDED WITH | ПОЛУЧИТЬ ИМЯ ЭНТИТИ С КОТОРЫМ СТОЛКНУЛИСЬ
        // @return sNameEntity | string
        ///////////////////////////////////////////////////////////////////////////////////////////
        string GetNameEntityCollided() const;
    protected:
        /////////////////////////////////
        // INIT VALUES | ИНИЦ. ЗНАЧЕНИЙ  
        /////////////////////////////////
        void Option();

        float               fDx;
        float               fDy;

        bool                bOnGround;
    private:
        float*              fTime;
        string              sNameEntityCollided;
    };
}

#endif // PHYSIC_MANAGER_H