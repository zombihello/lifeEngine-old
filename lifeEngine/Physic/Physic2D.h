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
#include "Body2D.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
    struct DLL_API Gravity2D
    {
        ////////////////
        /// КОНСТРУКТОР
        ////////////////
        Gravity2D( float fX, float fY );

        float       fX;
        float       fY;
    };

	//-------------------------------------------------------------------------//
	
    class DLL_API Physic2D
    {
    public:
        ////////////////
        /// КОНСТРУКТОР
        ////////////////
		Physic2D( Gravity2D Gravity );

        ////////////////
        /// ДЕСТРУКТОР
        ////////////////
		~Physic2D( );

        //////////////////
        /// СОЗДАТЬ ТЕЛО
        //////////////////
        void CreateBody( Body2D* Body );

        /////////////////////
        /// УНИЧТОЖИТЬ ТЕЛО
        ////////////////////
        void DestroyBody( Body2D* Body );

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
        void SetGravity( Gravity2D Gravity );

        //////////////////
        /// ПОЛУЧИТЬ ТЕЛО
        //////////////////
        Body2D* GetBody( string sNameBody );

        //////////////////////
        /// ПОЛУЧИТЬ ВСЕ ТЕЛА
        //////////////////////
		vector<Body2D*> GetAllBody( );

    private:

	   b2World* World;
	   vector<Body2D*> vBody;
    };
	
	//-------------------------------------------------------------------------//
}

#endif // LEPHYSIC_H
