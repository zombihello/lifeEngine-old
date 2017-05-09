#include "../../UI/GUI.h"

//-------------------------------------------------------------------------//

le::GUI::GUI( le::System& System )
{
	this->System = &System;
	ViewGUI = System.GetMenuCamera();
}

//-------------------------------------------------------------------------//

void le::GUI::SetView( View View )
{
	ViewGUI = View;
}

//-------------------------------------------------------------------------//

void le::GUI::AddText( le::Text* Text )
{
	vText.push_back( Text );
}

//-------------------------------------------------------------------------//

void le::GUI::AddTextManager( TextManager* TextManager )
{
	vTextManager.push_back( TextManager );
}

//-------------------------------------------------------------------------//

void le::GUI::AddButton( Button* Button )
{
	vButton.push_back( Button );
}

//-------------------------------------------------------------------------//

void le::GUI::AddButtonManager( ButtonManager* ButtonManager )
{
	vButtonManager.push_back( ButtonManager );
}

//-------------------------------------------------------------------------//

void le::GUI::UpdateGUI()
{
	System->GetWindow().pushGLStates(); // TODO: НАДО ОПТИМИЗИРОВАТЬ

	System->GetWindow().setView( ViewGUI );

	for ( int i = 0; i < vText.size(); i++ )
		vText[i]->UpdateText();

	for ( int i = 0; i < vTextManager.size(); i++ )
		vTextManager[i]->UpdateText();

	for ( int i = 0; i < vButton.size(); i++ )
		vButton[i]->ButtonUpdate();

	for ( int i = 0; i < vButtonManager.size(); i++ )
		vButtonManager[i]->ButtonsUpdate();

	System->GetWindow().popGLStates();
}

//-------------------------------------------------------------------------//

void le::GUI::ClearGUI()
{
	vText.clear();
	vTextManager.clear();
	vButton.clear();
	vButtonManager.clear();
}

//-------------------------------------------------------------------------//

View le::GUI::GetView()
{
	return ViewGUI;
}

//-------------------------------------------------------------------------//