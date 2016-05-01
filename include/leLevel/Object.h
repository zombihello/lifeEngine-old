#ifndef OBJECT_H
#define OBJECT_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../leSystem/leSystem.h"
#include "../lePhysic/lePhysic.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	struct DLL_API Object
	{
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		Object( Physic& Physic );

		/////////////////
		/// ДЕСТРКУТОР
		////////////////
		~Object();

		//////////////////////////////////
		/// ПОЛУЧИТЬ СВОЙСТВО ТИПА INT
		/////////////////////////////////
		int GetPropertyInt( const string sName );

		//////////////////////////////////
		/// ПОЛУЧИТЬ СВОЙСТВО ТИПА FLOAT
		/////////////////////////////////
		float GetPropertyFloat( const string sName );

		//////////////////////////////////
		/// ПОЛУЧИТЬ СВОЙСТВО ТИПА STRING
		/////////////////////////////////
		string& GetPropertyString( const string sName );

		string                      sName;
		string                      sType;
		Rect<float>                 rect;
		float                       Rotation;
		Body*                       BodyObject;
		Physic*                     Physic;
		map<string , string>        mProperties;

		Sprite                      sprite;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // OBJECT_H
