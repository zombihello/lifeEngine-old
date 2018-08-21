//////////////////////////////////////////////////////////////////////////
// 
//			*** lifeEngine (Двигатель Жизни) ***
//					Copyright (C) 2017
//
// Связь со мной:		https://vk.com/zombihello
// Репозиторий движка:  https://github.com/zombihello/lifeEngine
// 
//////////////////////////////////////////////////////////////////////////

#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#pragma warning( push )

#pragma warning( disable: 4251 )

#define COMPILING_LIBRARY
#include <DllGlobal.h>

//////////////
// OPENGL
//////////////
#include <glm/glm.hpp>

namespace le
{
	//-------------------------------------------------------------------------//

	//////////////////////////////////////////////////////////////////////
	/// \brief Структура хранящая конфигурации окна
	///
	/// В данной стрктуре находятся свойства окна, а так же другая
	/// информация, например, громкость звука
	//////////////////////////////////////////////////////////////////////
	struct DLL_API Configuration
	{	
		//////////////////////////////////////////////////////////////////////
		/// \brief Конструктор
		//////////////////////////////////////////////////////////////////////
		Configuration();

		int					FrameLimit;				///< Макс. FPS
		int					AntialiasingLevel;		///< Уровень сглаживания
		int					FOV;					///< Угол обзора
		int					RenderDistance;			///< Дальность прорисовки
		int					QualityShadows;			///< Качество теней
		static int			ShadowMapSize;			///< Разрешение карт теней
		float				SensitivityMouse;		///< Чувствительность мыши
		float				Time;					///< Прошедшие время

		bool				Fullscreen;				///< На весь ли экран окно
		bool				VSinc;					///< Включина ли виртуальная синхронизация
		static bool			IsWireframeRender;		///< Включен ли каркасный рендер

		glm::vec2			WindowSize;				///< Размеры окна
		glm::mat4			ProjectionMatrix;		///< Матрица проекции
	};
}

//-------------------------------------------------------------------------//

#pragma warning( pop )
#endif // CONFIGURATION_H
