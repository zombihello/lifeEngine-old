#ifndef POLYGONSHAPE_H
#define POLYGONSHAPE_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

////////////
/// BOX2D
///////////
#include <Box2D.h>

//////////////////
// LIFEENGINE
/////////////////
#include "../leSystem/leSystem.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
    class DLL_API PolygonShape
    {
    public:
        ////////////////
        /// КОНСТРУКТОР
        ////////////////
        PolygonShape( Vector2f SizeBody );
        PolygonShape( Vector2f SizeBody, Vector2f Center, float Angle = 0 );

        /////////////////////
        /// ЗАДАТЬ СВОЙСТВА
        /////////////////////
		void SetPropirtes( float Density, float Restitution, float Friction );

        //////////////////////////////////////
        /// ЗАДАТЬ РАЗМЕРЫ ПОЛИГОНОЙ ФОРМЫ
        ////////////////////////////////////
        void SetSize( Vector2f SizeBody );

        ////////////////////
        /// ЗАДАТЬ ФИКСТУРУ
        ////////////////////
        void SetFixture( b2Fixture* Fixture );

        //////////////////////////////
        /// ПОЛУЧИТЬ БАЗОВЫЕ СВОЙСТВА
        //////////////////////////////
        b2FixtureDef& GetFixtureDef();

        ///////////////////////
        /// ПОЛУЧИТЬ СВОЙСТВА
        ///////////////////////
        b2Fixture& GetFixture();
    private:
        float                           fAngle;

        Vector2f                        SizeBody;
        Vector2f                        Center;
        b2PolygonShape                  Shape;
        b2FixtureDef                    fDef;
        b2Fixture*                      Fixture;
    };
	
	//-------------------------------------------------------------------------//
}

#endif // POLYGONSHAPE_H
