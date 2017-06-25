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

	struct ModelVertex;
	struct VBO_ModelVertex;

	//-------------------------------------------------------------------------//

	struct DLL_API Bone
	{
		Bone();

		Matrixf				StartMatrix;
		Matrixf				InvertMatrix;
		Matrixf				Realese;

		vector<Bone*>		vChild;

		Bone*				Perent;
		string				name;
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

		/////////////////////
		/// ОЧИСТИТЬ СКЕЛЕТ
		/////////////////////
		void ClearSkeleton();

		//////////////////////
		/// ЗАГРУЗИТЬ СКЕЛЕТ
		/////////////////////
		void LoadSkeleton( TiXmlElement* skeleton, map<int, ModelVertex>& mVertexs );

		//////////////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ ПОЗИЦИИ ВЕРШИН
		/////////////////////////////////
		void InitMesh( map<int, ModelVertex>& ModelVertexs, vector<VBO_ModelVertex>& VBO_Vertexs );

		////////////////////////////////////
		/// ИНИЦИАЛИЗИРОВАТЬ СКЕЛЕТ МОДЕЛИ
		////////////////////////////////////
		void InitSkeleton( GLuint& VertexBuffer, vector<VBO_ModelVertex>& VBO_Vertexs, map<int, ModelVertex>& ModelVertexs );

		//////////////////////////////
		/// ДОБАВИТЬ КОСТЬ СКЕЛЕТУ
		//////////////////////////////
		void AddBone( Bone* bone );

		///////////////////////////////////////////////////
		/// ЗАДАТЬ ТРАНСФОРМАЦИЮ МОДЕЛИ ПЕРЕД БИНДИНГОМ 
		///////////////////////////////////////////////////
		void SetBindShape( Matrixf bindShape );

		///////////////////////////////////
		/// ЗАДАТЬ МАТРИЦУ ДЛЯ КОСТИ
		//////////////////////////////////
		void SetMatrixBone( string nameBone, Matrixf matrix );

		///////////////////////////////////////////////////
		/// ПОЛУЧИТЬ ТРАНСФОРМАЦИЮ МОДЕЛИ ПЕРЕД БИНДИНГОМ 
		///////////////////////////////////////////////////
		float* GetBindShape();

		/////////////////////////////////
		/// ПОЛУЧИТЬ ВСЕ КОСТИ СКЕЛЕТА
		////////////////////////////////
		vector<Bone*> GetAllBones();

		////////////////////////////////////////
		/// ПОЛУЧИТЬ КОСТЬ СКЕЛЕТА ПО ИМЕНИ
		////////////////////////////////////////
		Bone* GetBone( string NameBone );

		//////////////////////////////////
		/// ПОЛУЧИТЬ КОСТЬ СКЕЛЕТА ПО ID
		/////////////////////////////////
		int	GetIdBone( string NameBone );

		///////////////////////
		/// НАРИСОВАТЬ СКЕЛЕТ
		//////////////////////
		void DrawSkeleton( vector<Bone*> bones );

	private:
		////////////////////////////////////////////
		/// СЧИТЫВАНИЕ КОСТЕЙ [РЕКУРСИВНЫЙ МЕТОД]
		///////////////////////////////////////////
		void ReadingBone( TiXmlElement* node , Bone* bone );

		//////////////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ КОСТЕЙ СКЕЛЕТА
		//////////////////////////////////
		void InitBone();

		//////////////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ МАТРИЦ КОСТЕЙ
		//////////////////////////////////
		void InitMatrixBone( Bone& bone );

		bool							IsLoad;

		GLuint*							VertexBuffer;
		Matrixf							BindShape;

		vector<Bone*>					vBone;
		vector<VBO_ModelVertex>*		vVBO_Vertexs;
		map<int, ModelVertex>*			mModelVertexs;
	};

	//-------------------------------------------------------------------------//
}

#endif // SKELETON_H
