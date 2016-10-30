#ifndef LEPHYSIC_H
#define LEPHYSIC_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

////////////
/// BOX2D
///////////
#include <Box2D.h>

//////////////////
// LIFEENGINE
/////////////////
#include "../System/leSystem.h"
#include "Body.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
    struct DLL_API Gravity
    {
        ////////////////
        /// КОНСТРУКТОР
        ////////////////
        Gravity( float fX, float fY );

        float       fX;
        float       fY;
    };

	//-------------------------------------------------------------------------//
	
    class DLL_API Physic
    {
    public:
        ////////////////
        /// КОНСТРУКТОР
        ////////////////
        Physic( Gravity Gravity );

        ////////////////
        /// ДЕСТРУКТОР
        ////////////////
        ~Physic();

        //////////////////
        /// СОЗДАТЬ ТЕЛО
        //////////////////
        void CreateBody( Body* Body );

        /////////////////////
        /// УНИЧТОЖИТЬ ТЕЛО
        ////////////////////
        void DestroyBody( Body* Body );

        ////////////////////////
        /// УНИЧТОЖИТЬ ВСЕ ТЕЛА
        ////////////////////////
        void DestroyAllBody();

        ///////////////////////
        /// ОБНОВЛЕНИЕ ФИЗИКИ
        //////////////////////
        void UpdatePhysic( float timeStep, int velocityIterations, int positionIterations );

        ///////////////////////
        /// ЗАДАТЬ ГРАВИТАЦИЮ
        //////////////////////
        void SetGravity( Gravity Gravity );

        //////////////////
        /// ПОЛУЧИТЬ ТЕЛО
        //////////////////
        Body* GetBody( string sNameBody );

        //////////////////////
        /// ПОЛУЧИТЬ ВСЕ ТЕЛА
        //////////////////////
        vector<Body*> GetAllBody();

    private:

	   b2World* World;
       vector<Body*> vBody;
    };
	
	//-------------------------------------------------------------------------//
}

#endif // LEPHYSIC_H
