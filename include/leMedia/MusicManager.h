#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

/////////////////
/// LIFEENGINE
////////////////
#include "Music.h"

namespace le
{
	//-------------------------------------------------------------------------//
	
	class DLL_API MusicManager
	{
	public:
		/////////////////
		/// КОНСТРУКТОР
		////////////////
		MusicManager( System& System );

		/////////////////
		/// ДЕСТРУКТОР
		////////////////
		~MusicManager();

		//////////////////////
		/// ЗАГРУЗИТЬ МУЗЫКУ
		/////////////////////
		void LoadMusic( string Route, string NameMusic, bool loop = false );

		///////////////////
		/// ИГРАТЬ МУЗЫКУ
		//////////////////
		void PlayMusic( string NameMusic );

		////////////////////////
		/// УДАЛИТЬ ВСЮ МУЗЫКУ
		///////////////////////
		void DeleteAllMusic();

		////////////////////
		/// УДАЛИТЬ МУЗЫКУ
		///////////////////
		void DeleteMusic( string NameMusic );

		/////////////////////////
		/// ЗАГРУЖЕНА ЛИ МУЗЫКА
		/////////////////////////
		bool GetLoadedMusic( string NameMusic );
	private:
		Configuration*							Configuration;
		map<string,le::Music*>					vMusic;
		map<string,le::Music*>::iterator		Iterator;
	};
	
	//-------------------------------------------------------------------------//
}

#endif // MUSICMANAGER_H
