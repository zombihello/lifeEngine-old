#ifndef CIRCLESHAPE_H
#define CIRCLESHAPE_H

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
	
	class DLL_API CircleShape
	{
	public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		CircleShape( float Radius );
		CircleShape( float Radius, Vector2f Center );

		///////////////////
		/// ЗАДАТЬ РАДИУС
		//////////////////
		void SetRadius( float Radius );

		/////////////////////
		/// ИНИЦ. СВОЙСТВА
		/////////////////////
		void SetPropirtes( float Density, float Restitution, float Friction );

		//////////////////////
		/// ЗАДАТЬ СВОЙСТВА
		/////////////////////
		void SetFixture( b2Fixture* Fixture );

		////////////////////////////////
		/// ПОЛУЧИТЬ БАЗОВЫЕ СВОЙСТВА
		////////////////////////////////
		b2FixtureDef& GetFixtureDef();

		///////////////////////
		/// ПОЛУЧИТЬ СВОЙСТВА
		///////////////////////
		b2Fixture& GetFixture();
	private:
		float                           fRadius;

		Vector2f                        Center;
		b2CircleShape                   Circle;
		b2FixtureDef                    fDef;
		b2Fixture*                      Fixture;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // CIRCLESHAPE_H
