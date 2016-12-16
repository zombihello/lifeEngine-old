#ifndef SKELETON_H
#define SKELETON_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "../System.h"
#include <Matrix.h>

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API Bone
	{
		string name;
		Bone();
		Bone*				Perent;
		vector<Bone*>		vChild;

		Matrixf				StartMatrix;
		Matrixf				InvertMatrix;
		Matrixf				Realese;
	};

	//-------------------------------------------------------------------------//

	class DLL_API Skeleton
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		/////////////////
		Skeleton();

		/////////////////
		/// ДЕСТРУКТОР
		/////////////////
		~Skeleton();

		//////////////////////////////
		/// ДОБАВИТЬ КОСТЬ СКЕЛЕТУ
		//////////////////////////////
		void AddBone( Bone* bone );

		///////////////////////////////////////////////////
		/// ЗАДАТЬ ТРАНСФОРМАЦИЮ МОДЕЛИ ПЕРЕД БИНДИНГОМ 
		///////////////////////////////////////////////////
		void SetBindShape( Matrixf bindShape );

		///////////////////////////////////////////////////
		/// ПОЛУЧИТЬ ТРАНСФОРМАЦИЮ МОДЕЛИ ПЕРЕД БИНДИНГОМ 
		///////////////////////////////////////////////////
		float* GetBindShape();

		/////////////////////////////////
		/// ПОЛУЧИТЬ ВСЕ КОСТИ СКЕЛЕТА
		////////////////////////////////
		vector<Bone*> GetAllBones();

		// TODO: создана для теста скелета, позже будет для debug'а
		void DrawSkeleton( vector<Bone*> bones );
		void init(  );
	private:
		Matrixf				BindShape;
		vector<Bone*>		vBone;
	};

	//-------------------------------------------------------------------------//
}

#endif // SKELETON_H
