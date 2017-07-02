#ifndef AI_H
#define AI_H

#define COMPILING_LIBRARY
#include "../DllGlobal.h"

//////////////////
// LIFEENGINE
/////////////////
#include "AI_View.h"
#include "../Level/Level2D.h"

namespace le
{
	//-------------------------------------------------------------------------//

	class DLL_API AI //: private ViewAI
	{
	public:
		////////////////
		/// КОНСТРУКТОР
		////////////////
		AI( System& System, Level2D& Level );

		////////////////
		/// ДЕСТРУКТОР
		////////////////
		~AI();

		//////////////////
		/// ОБНОВЛЕНИЕ ИИ
		//////////////////
		void UpdateAI( vector<BasicPersonages*> vPersonage );

		//////////////////
		/// ХОДИТЬ
		//////////////////
		void Walk( Vector2f Factor , Body2D::TYPE_MOVE typeMove );

		////////////////////
		/// ИДТИ К ОБЪЕКТУ
		////////////////////
		void MoveToObject( Vector2f Factor , Body2D::TYPE_MOVE typeMove, BasicPersonages *Personage, bool UseY = false );

		//////////////////////
		/// ИИ УВИДЕЛ ВРАГА
		/////////////////////
		bool IsLook();

		//////////////////////////////
		/// ПОЛУЧИТЬ УВИДЕНОГО ВРАГА
		//////////////////////////////
		BasicPersonages* GetLookPersonage();
	protected:
		//////////////////////////
		/// ИНИЦИАЛИЗАЦИЯ ИИ
		//////////////////////////
		void InitAI( BasicPersonages* Personage , GroupColision GroupColision );
		void InitAI( BasicPersonages* Personage , string StartRoute , GroupColision GroupColision );

		/////////////////
		/// ДЕЙСТВИЯ ИИ
		/////////////////
		virtual void Actions() = 0;
		
	
		string								sTmpRouteAI;

	private:
		le::AI_View*						ViewAI;
		map<string , le::AI_Route>			mRoutesAI;
		BasicPersonages*					Personage;
	};

	//-------------------------------------------------------------------------//
}

#endif // AI_H
