#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

/////////////////
/// LIFEENGINE
////////////////
#include "Sound.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API SoundManager
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		SoundManager( System& System );

		/////////////////
		/// ДЕСТРУКТОР
		////////////////
		~SoundManager();

		//////////////////////
		/// ЗАГРУЗИТЬ МУЗЫКУ
		/////////////////////
		void LoadSound( string Route, string NameSound );

		///////////////////
		/// ИГРАТЬ МУЗЫКУ
		//////////////////
		void PlaySound( string NameSound );

		////////////////////////
		/// УДАЛИТЬ ВСЮ МУЗЫКУ
		///////////////////////
		void DeleteAllSound();

		////////////////////
		/// УДАЛИТЬ МУЗЫКУ
		///////////////////
		void DeleteSound( string NameSound );

		/////////////////////////
		/// ЗАГРУЖЕНА ЛИ МУЗЫКА
		/////////////////////////
		bool GetLoadedSound( string NameSound );
	private:
		Configuration*							Configuration;
		map<string,le::Sound*>					vSound;
		map<string,le::Sound*>::iterator		Iterator;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // SOUNDMANAGER_H
