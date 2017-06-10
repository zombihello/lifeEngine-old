#ifndef CAMERA_H
#define CAMERA_H

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

////////////////
/// LIFEENGINE
///////////////
#include "../System.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Camera
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		/////////////////
		Camera( System& System );

		/////////////////
		/// ДЕСТРУКТОР
		/////////////////
		~Camera();

		////////////////////////////////////
		/// ЗАДАТЬ ЧУВСТВИТЕЛЬНОСТЬ МЫШИ
		///////////////////////////////////
		void SetSensitivityMouse( float sensitivityMouse );

		///////////////////////////
		/// ЗАДАТЬ ПОЗИЦИЮ КАМЕРЫ
		///////////////////////////
		void SetPosition( Vector3f NewPosition );

		////////////////////////
		/// ОБНОВЛЕНИЕ КАМЕРЫ
		////////////////////////
		void UpdateCamera();

	private:

		void Move();

		float			fSensitivityMouse;
		float			fSpeed;

		System*			System;

		Vector2f		Angle;
		Vector3f		Position;
		Vector2f		CenterWindow;
	};

	//-------------------------------------------------------------------------//
}

#endif // CAMERA_H

