//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H

#define COMPILING_LIBRARY
#include <DllGlobal.h>

///////////////////////////
// СИСТЕМНЫЕ БИБЛИОТЕКИ
///////////////////////////
#include <string>
#include <map>
using namespace std;

//////////////
// OPENGL
//////////////
#include <glew/glew.h>
#include <glm/glm.hpp>
#include <gl/GLU.h>

//////////////////
// SFML
//////////////////
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
using namespace sf;

//////////////////
// LIFEENGINE
//////////////////
#include <Graphics\Mesh.h>
#include <System\Logger.h>

namespace le
{
	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Класс предназначенный для работы с ресурсами
	///
	/// Этот класс необходим для экономии оперативной и видеопамяти
	/// при загрузки ресурсов игры
	//////////////////////////////////////////////////////////////////////
	class DLL_API ResourcesManager
	{
	public:
		//////////////////////////////////////////////////////////////////////
		/// \brief Загрузить OpenGL текстуру
		///		
		/// \param[in] NameTexture Идентификатор текстуры
		/// \param[in] RouteToFile Путь к файлу
		/// \return true если все прошло успешно, иначе false
		//////////////////////////////////////////////////////////////////////
		static bool LoadGlTexture( const string& NameTexture, const string& RouteToFile );

		//////////////////////////////////////////////////////////////////////
		/// \brief Загрузить OpenGL кубическую текстуру
		///		
		/// \param[in] NameCubeTexture Идентификатор текстуры
		/// \param[in] RouteToSides Путь к сторонам текстуры куба (текстура для левой стороны, праой и т.д)
		/// \param[in] IsFlipVertically Перевернуть по вертекали
		/// \return true если все прошло успешно, иначе false
		//////////////////////////////////////////////////////////////////////
		static bool LoadGlCubeTexture( const string& NameCubeTexture, const vector<string>& RouteToSides, bool IsFlipVertically = false );

		//////////////////////////////////////////////////////////////////////
		/// \brief Загрузить меш
		///		
		/// \param[in] NameMesh Идентификатор меша
		/// \param[in] RouteToFile Путь к файлу
		/// \return true если все прошло успешно, иначе false
		//////////////////////////////////////////////////////////////////////
		static bool LoadMesh( const string& NameMesh, const string& RouteToFile );

		//////////////////////////////////////////////////////////////////////
		/// \brief Загрузить шейдер
		///
		/// \param[in] NameShader Идентификатор шейдера
		/// \param[in] VertexShader Вершиный шейдер
		/// \param[in] GeometryShader Геометрический шейдер
		/// \param[in] FragmentShader Фрагментный шейдер
		/// \return true если все прошло успешно, иначе false
		//////////////////////////////////////////////////////////////////////
		static bool LoadShader( const string& NameShader, const string& VertexShader, const string& GeometryShader, const string& FragmentShader );

		//////////////////////////////////////////////////////////////////////
		/// \brief Загрузить шейдер
		///	
		/// \param[in] NameShader Идентификатор шейдера
		/// \param[in] VertexShader Вершиный шейдер
		/// \param[in] FragmentShader Фрагментный шейдер
		/// \return true если все прошло успешно, иначе false
		//////////////////////////////////////////////////////////////////////
		static bool LoadShader( const string& NameShader, const string& VertexShader, const string& FragmentShader );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить OpenGL текстуру
		///		
		/// \param[in] NameTexture Идентификатор текстуры
		//////////////////////////////////////////////////////////////////////
		static void DeleteGlTexture( const string& NameTexture );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить OpenGL кубическую текстуру
		///		
		/// \param[in] NameCubeTexture Идентификатор текстуры
		//////////////////////////////////////////////////////////////////////
		static void DeleteGlCubeTexture( const string& NameCubeTexture );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить меш
		///		
		/// \param[in] NameMesh Идентификатор меша
		//////////////////////////////////////////////////////////////////////
		static void DeleteMesh( const string& NameMesh );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить шейдер
		///		
		/// \param[in] NameShader Идентификатор шейдера
		//////////////////////////////////////////////////////////////////////
		static void DeleteShader( const string& NameShader );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить все OpenGL текстуры
		//////////////////////////////////////////////////////////////////////
		static void DeleteAllGlTexture();

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить все OpenGL кубические текстуры
		//////////////////////////////////////////////////////////////////////
		static void DeleteAllGlTCubeTexture();

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить все меши
		//////////////////////////////////////////////////////////////////////
		static void DeleteAllMeshes();

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить все шейдеры
		//////////////////////////////////////////////////////////////////////
		static void DeleteAllShaders();

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить все ресурсы
		//////////////////////////////////////////////////////////////////////
		static void DeleteAllResources();

		//////////////////////////////////////////////////////////////////////
		/// \brief Задать текстуру ошибки
		///		
		/// \param[in] ErrorTexture Текстура Ошибки
		//////////////////////////////////////////////////////////////////////
		static void SetErrorTexture( const string& ErrorTexture );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить OpenGL текстуру
		///		
		/// \param[in] NameTexture Идентификатор текстуры
		/// \return const GLuint. Если текстура с данным названием 
		/// отсутствует, то вернет текстуру ошибки или 0
		//////////////////////////////////////////////////////////////////////
		static const GLuint GetGlTexture( const string& NameTexture );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить OpenGL кубическую текстуру
		///		
		/// \param[in] NameCubeTexture Идентификатор текстуры
		/// \return const GLuint. Если текстура с данным названием 
		/// отсутствует, то вернет текстуру ошибки или 0
		//////////////////////////////////////////////////////////////////////
		static const GLuint GetGlCubeTexture( const string& NameCubeTexture );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить меш
		///		
		/// \param[in] NameMesh Идентификатор меша
		/// \return Mesh*. Если меша с данным названием
		/// нету, то вернет NULL (0)
		//////////////////////////////////////////////////////////////////////
		static Mesh* GetMesh( const string& NameMesh );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить шейдер
		///		
		/// \param[in] NameShader Идентификатор шейдера
		/// \return Shader*. Если шейдера с данным названием
		/// нету, то вернет NULL (0)
		//////////////////////////////////////////////////////////////////////
		static Shader* GetShader( const string& NameShader );

	private:

		static GLuint						ErrorTexture; ///< Текстура ошибки

		static map<string, GLuint>			GlTextures; ///< Массив OpenGL текстур
		static map<string, GLuint>			GlCubeTextures; ///< Массив OpenGL кубических текстур
		static map<string, Shader>			Shaders; ///< Массив шейдеров
		static map<string, Mesh>			Meshes; ///< Массив мешей
	};

	//-------------------------------------------------------------------------//
}

#endif // RESOURCES_MANAGER_H