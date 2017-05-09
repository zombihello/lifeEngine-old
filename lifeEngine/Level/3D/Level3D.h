#ifndef LEVEL3D_H
#define LEVEL3D_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "../../System/System.h"
#include "Brush.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Level3D
	{
	public:
		/////////////////
		/// йнмярпсйрнп
		////////////////
		Level3D( System& System );

		/////////////////
		/// деярпсйрнп
		////////////////
		~Level3D();

		///////////////////////
		/// гюцпсгхрэ спнбемэ
		//////////////////////
		bool LoadLevel( string sRoute );

		///////////////////////
		/// нвхярхрэ спнбемэ
		//////////////////////
		void ClearLevel();

		///////////////////////
		/// нрпемдепхрэ спнбемэ
		//////////////////////
		void RenderLevel();

	private:

		string				sNameMap;
		string				sDescriptionMap;
		string				sSkyBoxName;

		System*				System;

		vector<Brush*>		vBrushes;
	};

	//-------------------------------------------------------------------------//
}

#endif // LEVEL3D_H
