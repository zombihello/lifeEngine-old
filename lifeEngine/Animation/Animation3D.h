#ifndef ANIMATION3D_H
#define ANIMATION3D_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

/////////////////////////
// LIFEENGINE
////////////////////////
#include <HaffmanCode.h>
#include "../System/System.h"
#include "../Utils/Math/Matrix.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class Skeleton;

	//-------------------------------------------------------------------------//

	struct DLL_API AnimationBone
	{
		vector<float>			vTimes;
		vector<Matrix>			vFrames;
		vector<string>			vTypeInterpolation;
	};

	//-------------------------------------------------------------------------//

	class DLL_API Animation3D
	{
	public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		Animation3D();
		Animation3D( Skeleton& Skeleton, float MaxFrames );
		
		////////////////
		/// ДЕСТРУКТОР
		////////////////
		~Animation3D();

		//////////////////////////////////
		/// НАЧАТЬ ПРОИГРЫВАТЬ АНИМАЦИЮ
		/////////////////////////////////
		void Play( bool isLoop = false );

		//////////////////////////////
		/// ПРИОСТАНОВИТЬ АНИМАЦИЮ
		//////////////////////////////
		void Pause();

		//////////////////////////////
		/// ОСТАНОВИТЬ АНИМАЦИЮ
		//////////////////////////////
		void Stop();

		///////////////////////
		/// ОБНОВИТЬ АНИМАЦИЮ
		//////////////////////
		void UpdateAnimation();

		///////////////////////////
		/// ЗАДАТЬ ЧАСТОТУ КАДРОВ
		///////////////////////////
		void SetFPS( float fps );

		////////////////////////
		/// ИГРАЕТ ЛИ АНИМАЦИЯ
		///////////////////////
		bool IsPlaying();

		//////////////////////////
		/// ЗАГРУЗИТЬ АНИМАЦИЮ
		/////////////////////////
		bool LoadAnimation( TiXmlElement* animation );

		//////////////////////////
		/// ЗАДАТЬ СКЕЛЕТ МОДЕЛИ
		/////////////////////////
		void SetSkeleton( Skeleton& Skeleton );

	private:

		int							iNextFrame;

		float						fNowFrame;
		float						fFps;
		float						fDeltaFrame;
		float					    fMaxFrames;

		bool						bIsLoop;
		bool						bIsPlaying;

		Skeleton*				    Skeleton;
		map<string, AnimationBone>  mAnimationBone;
	};

	//-------------------------------------------------------------------------//
}

#endif // ANIMATION3D_H