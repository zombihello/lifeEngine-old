#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

/////////////////////////
// LIFEENGINE
////////////////////////
#include "../System/System.h"
#include "Animation.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API AnimationManager
	{
	public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		AnimationManager();

		////////////////
		/// ДЕСТРУКТОР
		///////////////
		~AnimationManager();

		/////////////////////////////
		/// ЗАГРУЗИТЬ АНИМАЦИЮ С XML
		////////////////////////////
		void LoadAnimation( string sRoute );

		/////////////////////
		/// ЗАДАТЬ  АНИМАЦИЮ
		/////////////////////
		void SetAnimation( string sName, bool loop = false );

		//////////////////////////////////////////////
		/// ОТРАЗИТЬ ТЕКУЩУЮ АНИМАЦИЮ ( ЗЕРКАЛЬНО )
		//////////////////////////////////////////////
		void Flip( bool bFlip = true );

		////////////////////////////////////////////////
		/// ОТРАЗИТЬ АНИМАЦИЮ ПО НАЗВАНИЮ ( ЗЕРКАЛЬНО )
		////////////////////////////////////////////////
		void Flip( const string NameAnimation , bool bFlip = true );

		//////////////////////////
		/// ОБНОВИТЬ АНИМАЦИЮ
		//////////////////////////
		void UpdateAnimation( float Time, Sprite& Sprite );

		/////////////////////////////////
		/// ПОСТАВИТЬ АНИМАЦИЮ НА ПАУЗУ
		/////////////////////////////////
		void Pause();

		////////////////////////
		/// ПРОИГРАТЬ АНИМАЦИЮ
		////////////////////////
		void Play();

		////////////////////////
		/// ИГРАЕТ ЛИ АНИМАЦИЯ
		///////////////////////
		bool IsPlaying();

		//////////////////////////
		/// ОТРАЖЕНА ЛИ АНИМАЦИЯ
		/////////////////////////
		bool IsFlip();
		bool IsFlip( string NameAnimation );

		////////////////////////////////////////
		/// ПОЛУЧИТЬ НАЗВАНИЕ ТЕКУЩЕЙ АНИМАЦИИ
		///////////////////////////////////////
		string GetNameCurrentAnim();
	private:
		string                          sCurrentAnim;
		map<string , Animation>         mAnimList;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // ANIMATIONMANAGER_H
