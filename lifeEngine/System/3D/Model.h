#ifndef MODEL_H
#define MODEL_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "../System.h"
#include "Skeleton.h"
#include "../../Animation/AnimationManager3D.h"

namespace le
{
	//-------------------------------------------------------------------------//

	struct DLL_API Vertex
	{
		Vector3f			Position;
		Vector3f			DefaultPosition;
		vector<int>			vIdBones;
		vector<float>		vIdWeihgt;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API TextureCoord
	{
		Vector2f		Coords;
		int				idTexture;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API IDsVNTC
	{
		IDsVNTC();

		int		idVertex;
		int		idNormal;
		int		idTextureCoord;
		int		idVertexColor;
	};

	//-------------------------------------------------------------------------//

	struct DLL_API Mesh
	{
		/////////////////
		/// ОЧИСТИТЬ МЕШ
		/////////////////
		void Clear();

		vector<IDsVNTC>			vIDs;

		vector<Vertex>			vVertexs;
		vector<Vector3f>		vNormals;
		vector<TextureCoord>	vTextureCoords;
		vector<Color>			vVertexColors;
		vector<GLuint>			vTextures;
	};

	//-------------------------------------------------------------------------//

	class DLL_API Model
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		/////////////////
		Model( System& System );

		/////////////////
		/// ДЕСТРУКТОР
		/////////////////
		~Model();

		/////////////////////
		/// УДАЛИТЬ МОДЕЛЬ
		/////////////////////
		void DeleteModel();

		//////////////////////
		/// ЗАГРУЗКА МОДЕЛИ
		//////////////////////
		bool LoadModel( string route );

		////////////////////////////
		/// ЗАДАТЬ ПОЗИЦИЮ МОДЕЛИ
		///////////////////////////
		void SetPosition( Vector3f Position );

		////////////////////////////
		/// ЗАДАТЬ МАСШТАБ МОДЕЛИ
		///////////////////////////
		void SetScale( Vector3f Scale );

		////////////////////////////
		/// ЗАДАТЬ ПОВОРОТ МОДЕЛИ
		///////////////////////////
		void SetRotate( Vector3f Angle );

		////////////////////
		/// СМЕСТИТЬ МОДЕЛЬ
		////////////////////
		void Move( Vector3f FactorMove );

		///////////////////////////
		/// МАСШТАБИРОВАТЬ МОДЕЛЬ
		//////////////////////////
		void Scale( Vector3f FactorScale );

		//////////////////////
		/// ПОВЕРНУТЬ МОДЕЛЬ
		//////////////////////
		void Rotate( Vector3f Angle );

		//////////////////////
		/// ОТРИСОВКА МОДЕЛИ
		//////////////////////
		void RenderModel();

		////////////////////////////////
		/// ПОЛУЧИТЬ МЕНЕДЖЕР АНИМАЦИЙ
		////////////////////////////////
		AnimationManager3D& GetAnimationManager();

		/////////////////////////////
		/// ПОЛУЧИТЬ ПОЗИЦИЮ МОДЕЛИ
		/////////////////////////////
		Vector3f GetPosition();

		////////////////////////////
		/// ПОЛУЧИТЬ МАСШТАБ МОДЕЛИ
		////////////////////////////
		Vector3f GetScale();

		////////////////////////////
		/// ПОЛУЧИТЬ ПОВОРОТ МОДЕЛИ
		////////////////////////////
		Vector3f GetRotate();

	private:

		Vector3f						Position;
		Vector3f						ScaleModel;
		Vector3f						Angle;

		Mesh							Mesh;
		Skeleton*						Skeleton;
		AnimationManager3D*				AnimationManager3D;
		System*							System;
	};

	//-------------------------------------------------------------------------//
}

#endif // MODEL_H