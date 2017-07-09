#ifndef SKELETON_H
#define SKELETON_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "../System.h"

namespace le
{
	//-------------------------------------------------------------------------//

	struct ModelVertex;
	struct VBO_ModelVertex;

	//-------------------------------------------------------------------------//

	struct DLL_API CollisionVertex
	{
		glm::mat4x4				Matrix;
		glm::mat4x4				MatrixVertex;

		float*					Position[3];

		Vector3f				DefaultPosition;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API Bone
	{
		/////////////////
		/// КОНСТРУКТОР
		/////////////////
		Bone();

		glm::mat4x4			StartMatrix;
		glm::mat4x4			InvertMatrix;
		glm::mat4x4			Realese;

		int			        iIdPerent;

		string				sNameParentBone;
		string				sNameBone;

		vector<int>		    vIdChild;
		vector<int>			vIdVertex;
		vector<int>			vCollision_IdVertex;
		vector<float>		vWeights;
		vector<float>		vCollision_Weights;
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
		void LoadSkeleton( TiXmlElement* skeleton );

		//////////////////////////////////////////
		/// ЗАГРУЗИТЬ ИНФОРМАЦИЮ ПРО МЕШ КОЛИЗИИ
		//////////////////////////////////////////
		void LoadCollisionInfo( TiXmlElement* skeleton );

		//////////////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ ПОЗИЦИИ ВЕРШИН
		/////////////////////////////////
		void InitMesh( map<int, ModelVertex>& ModelVertexs, vector<VBO_ModelVertex>& VBO_Vertexs );

		//////////////////////////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ ПОЗИЦИИ ВЕРШИН У КОЛЛИЗИИ
		//////////////////////////////////////////////
		void InitCollisionMesh( vector<float>& Vertexs );

		////////////////////////////////////
		/// ИНИЦИАЛИЗИРОВАТЬ СКЕЛЕТ МОДЕЛИ
		////////////////////////////////////
		void InitSkeleton( GLuint& VertexBuffer, vector<VBO_ModelVertex>& VBO_Vertexs, map<int, ModelVertex>& ModelVertexs );

		//////////////////////////////////////////
		/// ИНИЦИАЛИЗИРОВАТЬ КОЛЛИЗИЮ ДЛЯ СКЕЛЕТА
		//////////////////////////////////////////
		void InitCollision( vector<float>& Vertexs );

		/////////////////////////////////////////
		/// ОБНОВЛЕНИЕ ИЗМЕНЕНЫХ ПОЗИЦИИ ВЕРШИН
		/////////////////////////////////////////
		void UpdateMesh();

		//////////////////////////////
		/// ДОБАВИТЬ КОСТЬ СКЕЛЕТУ
		//////////////////////////////
		void AddBone( Bone bone );

		///////////////////////////////////
		/// ЗАДАТЬ МАТРИЦУ ДЛЯ КОСТИ
		//////////////////////////////////
		void SetMatrixBone( string nameBone, glm::mat4x4 matrix );

		///////////////////////////////////////////////////
		/// ПОЛУЧИТЬ ТРАНСФОРМАЦИЮ МОДЕЛИ ПЕРЕД БИНДИНГОМ 
		///////////////////////////////////////////////////
		glm::mat4x4 GetBindShape();

		/////////////////////////////////
		/// ПОЛУЧИТЬ ВСЕ КОСТИ СКЕЛЕТА
		////////////////////////////////
		vector<Bone> GetAllBones();

		////////////////////////////////////////
		/// ПОЛУЧИТЬ КОСТЬ СКЕЛЕТА ПО ИМЕНИ
		////////////////////////////////////////
		Bone* GetBone( string NameBone );

		//////////////////////////////////
		/// ПОЛУЧИТЬ КОСТЬ СКЕЛЕТА ПО ID
		/////////////////////////////////
		Bone* GetBone( int idBone );

		////////////////////////////////////////
		/// ПОЛУЧИТЬ ID КОСТИ СКЕЛЕТА ПО ИМЕНИ
		///////////////////////////////////////
		int	GetIdBone( string NameBone );

		///////////////////////
		/// НАРИСОВАТЬ СКЕЛЕТ
		//////////////////////
		//void DrawSkeleton( vector<Bone>& bones );

		/////////////////////////////////////////////
		/// ГОТОВ ЛИ СКЕЛЕТ К ПРОИГРЫВАНИЮ АНИМАЦИИ
		/////////////////////////////////////////////
		bool IsInit();

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
		bool							IsDebug;
		bool							IsCollision;

		GLuint*							VertexBuffer;
		glm::mat4x4						BindShape;

		vector<VBO_ModelVertex>*		vVBO_Vertexs;	
		vector<CollisionVertex>		    vCollision_Vertexs;
		vector<Bone>					vBones;
		vector<Bone*>					vUpdateBones;
		map<int, ModelVertex>*			mModelVertexs;
	};

	//-------------------------------------------------------------------------//
}

#endif // SKELETON_H
