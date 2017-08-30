//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef SYSTEM_H
#define SYSTEM_H

#define COMPILING_LIBRARY
#define NUMBER_TO_DOUBLE( X )		static_cast< double > ( X )
#define NUMBER_TO_FLOAT( X )		static_cast< float > ( X )
#define NUMBER_TO_INT( X )			static_cast< int > ( X )
#define NUMBER_TO_UINT( X )			static_cast< unsigned int > ( X )
#define NUMBER_TO_STRING( X )		to_string( X );
#define ENGINE						"lifeEngine"
#define ENGINE_VERSION				"v3.0.0"
#include <DllGlobal.h>

///////////////////////////
// СИСТЕМНЫЕ БИБЛИОТЕКИ
///////////////////////////
#include <iostream>
#include <fstream>
using namespace std;

//////////////
// OPENGL
//////////////
#include <glew\glew.h>
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>

//////////////////
// SFML
//////////////////
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
using namespace sf;

//////////////////
// LIFEENGINE
//////////////////
#include <System\Configuration.h>
#include <System\ConfigFile.h>
#include <System\Logger.h>
#include <System\BasicApplication.h>

namespace le
{
	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Класс необходимый для работы движка
	///
	/// Это самый главный класс в движке. Он обеспечивает базовые
	/// методы (создание и обновление окна), так же он необходим
	/// для работы других классов
	//////////////////////////////////////////////////////////////////////
	class DLL_API System
	{
	public:
		//////////////////////////////////////////////////////////////////////
		/// \brief Конструктор
		/// \details В этом конструкторе идет инициализация движка 
		///		
		/// \param[in] argc, argv Аргументы запуска программы
		/// \param[in] ConfigFile Путь к файлу конфигураций
		/// \param[in] LogFile Путь к файлу логов
		//////////////////////////////////////////////////////////////////////
		System( int argc, char** argv, const string& ConfigFile = "config.cfg", const string& LogFile = ENGINE ".log" );

		//////////////////////////////////////////////////////////////////////
		/// \brief Деструктор
		//////////////////////////////////////////////////////////////////////
		~System();

		//////////////////////////////////////////////////////////////////////
		/// \brief Создание окна
		/// \details Этот метод создает окно
		/// 
		/// \param[in] NameWindow Имя окна
		/// \param[in] Style Стиль окна (полноэекранный, оконный и т.д)
		//////////////////////////////////////////////////////////////////////
		void WindowCreate( const string& NameWindow, int Style = sf::Style::Default );

		//////////////////////////////////////////////////////////////////////
		/// \brief Главный цикл игры
		/// \details В этом методе происходить обновление игрового окна
		///		
		/// \param[in] Application Объект приложения
		//////////////////////////////////////////////////////////////////////
		void MainLoop( BasicApplication& Application );

		//////////////////////////////////////////////////////////////////////
		/// \brief Проверка на существование файла
		///		
		/// \param[in] RouteToFile Путь к файлу
		/// \return true - если файл есть, иначе false
		//////////////////////////////////////////////////////////////////////
		static bool FileExists( const string& RouteToFile );

		//////////////////////////////////////////////////////////////////////
		/// \brief Проверка на существование директории
		///		
		/// \param[in] RouteToDirectory Путь к директории
		/// \return true - если директория есть, иначе false
		//////////////////////////////////////////////////////////////////////
		static bool DirectoryExists( const string& RouteToDirectory );

		//////////////////////////////////////////////////////////////////////
		/// \brief Получить конфигурации окна
		///
		/// \return const Configuration&
		//////////////////////////////////////////////////////////////////////
		Configuration& GetConfiguration();

	private:

		Configuration			Configuration; ///< Конфигурации окна
		RenderWindow			RenderWindow; ///< Окно игры
		Event                   Event; ///< Cобытия окна
	};

	//-------------------------------------------------------------------------//
}

#endif // SYSTEM_H