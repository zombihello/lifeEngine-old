#ifndef ENTITY_H
#define ENTITY_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../../System/System.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Entity
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		Entity( TiXmlElement& ElementEntity );

		//////////////////////////
		/// ПОЛУЧИТЬ ИМЯ ЭНТИТИ
		/////////////////////////
		string GetNameEntity();

		///////////////////////////////
		/// ПОЛУЧИТЬ ЗНАЧЕНИЕ СТРОКОЙ
		///////////////////////////////
		string GetValueString( string NameValue );

		/////////////////////////////////////
		/// ПОЛУЧИТЬ ЗНАЧЕНИЕ ЦЕЛЫМ ЧИСЛОМ
		/////////////////////////////////////
		int GetValueInt( string NameValue );

		///////////////////////////////
		/// ПОЛУЧИТЬ ЗНАЧЕНИЕ ДРОБЬЮ
		///////////////////////////////
		float GetValueFloat( string NameValue );

		///////////////////////////////////////
		/// ПОЛУЧИТЬ ЗНАЧЕНИЕ МАССИВОМ ДРОБЕЙ
		///////////////////////////////////////
		vector<float> GetVelueVectorFloat( string NameValue );

		///////////////////////////////
		/// ПОЛУЧИТЬ ПОЗИЦИЮ ЭНТИТИ
		///////////////////////////////
		Vector3f GetPosition();

	private:

		string					sNameEntity;
		Vector3f				Position;
		map<string, string>		mValues;
	};

	//-------------------------------------------------------------------------//
}

#endif // ENTITY_H
