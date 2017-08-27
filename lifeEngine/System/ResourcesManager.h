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
		/// \brief Удалить OpenGL текстуру
		///		
		/// \param[in] NameTexture Идентификатор текстуры
		//////////////////////////////////////////////////////////////////////
		static void DeleteGlTexture( const string& NameTexture );

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить все OpenGL текстуры
		//////////////////////////////////////////////////////////////////////
		static void DeleteAllGlTexture();

		//////////////////////////////////////////////////////////////////////
		/// \brief Удалить все ресурсы
		//////////////////////////////////////////////////////////////////////
		static void DeleteAllResources();

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить OpenGL текстуру
		///		
		/// \param[in] NameTexture Идентификатор текстуры
		/// \return const GLuint. Если текстура с данным названием 
		/// отсутствует, то вернет 0
		//////////////////////////////////////////////////////////////////////
		static const GLuint GetGlTexture( const string& NameTexture );

	private:

		static map<string, GLuint>			GlTextures; ///< Массив OpenGL текстур
	};

	//-------------------------------------------------------------------------//
}

#endif // RESOURCES_MANAGER_H