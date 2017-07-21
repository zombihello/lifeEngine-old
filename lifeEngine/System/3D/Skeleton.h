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
		glm::mat4x4				BoneTransform;

		float*					Position[3];

		glm::vec3				DefaultPosition;
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
		glm::mat4x4			InterpolatedMatrix;
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


		/////////////////////////////////////////////
		/// ОБНОВИТЬ ИЕРАРХИЮ КОСТЕЙ СКЕЛЕТА
		/////////////////////////////////////////////
		void UpdateHierarchy( );

		/////////////////////////////////
		/// ОБНОВИТЬ МАТРИЦУ КОСТИ
		////////////////////////////////
		void UpdateMatrixBone( string nameBone, glm::mat4x4 Matrix );

		//////////////////////////////
		/// ДОБАВИТЬ КОСТЬ СКЕЛЕТУ
		//////////////////////////////
		void AddBone( Bone bone );

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

		//////////////////////////////////////////////////
		/// ОБНОВИТЬ МАТРИЦУ КОСТЕЙ [РЕКУРСИВНЫЙ МЕТОД]
		//////////////////////////////////////////////////
		void UpdateMatrixBone( Bone& bone );

		bool							IsLoad;
		bool							IsDebug;
		bool							IsCollision;

		GLuint*							VertexBuffer;
		glm::mat4x4						BindShape;

		vector<VBO_ModelVertex>*		vVBO_Vertexs;	
		vector<CollisionVertex>		    vCollision_Vertexs;
		vector<Bone>					vBones;
		vector<int>						vRootBones;
		map<int, ModelVertex>*			mModelVertexs;
	};

	//-------------------------------------------------------------------------//
}

#endif // SKELETON_H
