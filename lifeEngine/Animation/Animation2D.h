#ifndef ANIMATION2D_H
#define ANIMATION2D_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

/////////////////////////
// LIFEENGINE
////////////////////////
#include "../System/System.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API Animation2D
	{
	public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		Animation2D();

		//////////////////
		/// ТИК АНИМАЦИИ
		//////////////////
		void TickAnimation( float time , Sprite& Sprite );

		////////////////////////
		/// ЗАЦИКЛИТЬ АНИМАЦИЮ
		////////////////////////
		void SetLoop( bool loop );

		//////////////////////////////
		/// ЗАДАТЬ СКОРОСТЬ АНИМАЦИИ
		/////////////////////////////
		void SetSpeed( float speed );

		///////////////////////
		/// СКОРОСТЬ АНИМАЦИИ
		///////////////////////
		float& GetSpeed();

		/////////////////////////////
		/// ПОЛУЧИТЬ КАДРЫ АНИМАЦИИ
		/////////////////////////////
		vector<IntRect>& GetFrames();

		//////////////////////////////////////////
		/// ПОЛУЧИТЬ ОТЗЕРКАЛЕНЫЕ КАДРЫ АНИМАЦИИ
		//////////////////////////////////////////
		vector<IntRect>& GetFramesFlip();

		///////////////////
		/// ПОЛУЧИТЬ ФЛИП
		//////////////////
		bool& GetFlip();

		////////////////////////
		/// ИГРАЕТ ЛИ АНИМАЦИЯ
		///////////////////////
		bool& IsPlaying();
	private:
		int							iCountAnimationFrames;

		float                       fCurrentFrame;
		float                       fSpeed;

		bool                        bLoop;
		bool                        bFlip;
		bool                        bIsPlaying;

		vector<IntRect>             vFrames;
		vector<IntRect>             vFrames_flip;
	};

	//-------------------------------------------------------------------------//
}

#endif // ANIMATION2D_H
