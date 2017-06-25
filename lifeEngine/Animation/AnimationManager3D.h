#ifndef ANIMATION_MANAGER_3D
#define ANIMATION_MANAGER_3D

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

/////////////////////////
// LIFEENGINE
////////////////////////
#include <HaffmanCode.h>
#include "Animation3D.h"
#include "../System/System.h"

namespace le
{
	class DLL_API AnimationManager3D
	{
	public:

		////////////////
		/// КОНСТРУКТОР
		////////////////
		AnimationManager3D();
		AnimationManager3D( Skeleton& Skeleton );

		////////////////
		/// ДЕСТРУКТОР
		////////////////
		~AnimationManager3D();

		/////////////////////////////////
		/// НАЧАТЬ ПРОИГРЫВАТЬ АНИМАЦИЮ
		/////////////////////////////////
		void Play( string nameAnimation, bool isLoop = false );

		////////////////////////////
		/// ПРИОСТАНОВИТЬ АНИМАЦИЮ
		////////////////////////////
		void Pause();

		/////////////////////////
		/// ОСТАНОВИТЬ АНИМАЦИЮ
		/////////////////////////
		void Stop();

		///////////////////////
		/// ОБНОВИТЬ АНИМАЦИЮ
		///////////////////////
		void UpdateAnimation();

		////////////////////////////////////
		/// ЗАГРУЗИТЬ ВСЕ АНИМАЦИИ МОДЕЛИ
		////////////////////////////////////
		bool LoadAnimations( TiXmlElement* animations );

		////////////////////////
		/// ИГРАЕТ ЛИ АНИМАЦИЯ
		///////////////////////
		bool IsPlaying( );

		//////////////////////////
		/// ЗАДАТЬ СКЕЛЕТ МОДЕЛИ
		/////////////////////////
		void SetSkeleton( Skeleton& Skeleton );

	private:

		string					 sCurrentAnimation;

		Skeleton*				 Skeleton;
		map<string, Animation3D> mAnimations;
	};

	//-------------------------------------------------------------------------//
}

#endif // ANIMATION_MANAGER_3D