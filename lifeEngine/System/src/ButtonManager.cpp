#include "../ButtonManager.h"

//-------------------------------------------------------------------------//

le::ButtonManager::ButtonManager( le::System &System )
{
	this->System = &System;

	iIdButton = 0;
	ColorSelect = Color::White;
}

//-------------------------------------------------------------------------//

le::ButtonManager::~ButtonManager()
{
	DeleteAllButtons();
}

//-------------------------------------------------------------------------//

void le::ButtonManager::LoadFont( const string sRoute )
{
	Font.loadFromFile( sRoute );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::CreateButton( const String sText, const int iSize, Vector2f PositionText, Color ColorText )
{
	le::Button* Button = new le::Button( *System );
	Button->SetFont( Font );
	Button->CreateButton( sText, iSize, PositionText, ColorText );
	Button->SetId( vButton.size()+1 );
	Button->SetColorSelect( ColorSelect );

	vButton.push_back( Button );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::CreateButton( sf::Text Text )
{
	le::Button* Button = new le::Button( *System );
	Button->SetFont( Font );
	Button->CreateButton( Text );

	vButton.push_back( Button );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::ButtonsUpdate()
{
	iIdButton = -1;

	for ( int i = 0; i < vButton.size(); i++ )
	{
		vButton[ i ]->ButtonUpdate();

		if ( vButton[ i ]->IsClick() )
			iIdButton = i;
	}
}

//-------------------------------------------------------------------------//

void le::ButtonManager::DeleteAllButtons()
{
	for ( int i = 0; i < vButton.size(); i++ )
		delete vButton[ i ];

	iIdButton = 0;
	vButton.clear();
}

//-------------------------------------------------------------------------//

void le::ButtonManager::DeleteButton( const int id )
{
	if ( id - 1 > -1 && id - 1 < vButton.size() )
	{
		delete vButton[ id - 1 ];
		vButton.erase( vButton.begin() + id - 1 );
	}
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetFont( const sf::Font Font )
{
	this->Font = Font;
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetColorSelect( const int IdButton, Color Color )
{
	if ( IdButton - 1 > -1 && IdButton - 1 < vButton.size() )
		vButton[ IdButton-1 ]->SetColorSelect( Color );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetColorSelect( Color Color )
{
	for ( int i = 0; i < vButton.size(); i++ )
		vButton[ i ]->SetColorSelect( Color );

	ColorSelect = Color;
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetColorDefoult( const int IdButton, Color Color )
{
	if ( IdButton - 1 > -1 && IdButton - 1 < vButton.size() )
		vButton[ IdButton-1 ]->SetColorDefoult( Color );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetColorDefoult( Color Color )
{
	for ( int i = 0; i < vButton.size(); i++ )
		vButton[ i ]->SetColorDefoult( Color );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetText( const int IdButton, const String sText )
{
	if ( IdButton - 1 > -1 && IdButton - 1 < vButton.size() )
		vButton[ IdButton-1 ]->SetText( sText );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetSize( const int IdButton, const int iSize )
{
	if ( IdButton - 1 > -1 && IdButton - 1 < vButton.size() )
		vButton[ IdButton-1 ]->SetSize( iSize );
}

//-------------------------------------------------------------------------//

void le::ButtonManager::SetSize( const int iSize )
{
	for ( int i = 0; i < vButton.size(); i++ )
		vButton[ i ]->SetSize( iSize );
}

//-------------------------------------------------------------------------//

le::Button* le::ButtonManager::GetButton( int id )
{
	if ( id - 1 > -1 && id - 1 < vButton.size() )
		return vButton[ id ];
	else
		return NULL;
}

//-------------------------------------------------------------------------//

le::Button*le::ButtonManager::GetButton( String textInButton )
{
	for ( int i = 0; i < vButton.size(); i++ )
	{
		if ( vButton[ i ]->GetTextInButton() == textInButton )
			return vButton[ i ];
	}

	return NULL;
}

//-------------------------------------------------------------------------//

const int le::ButtonManager::GetIdButton( String textInButton )
{
	for ( int i = 0; i < vButton.size(); i++ )
		if ( vButton[ i ]->GetTextInButton() == textInButton )
			return i + 1;

	return -1;
}

//-------------------------------------------------------------------------//

vector<le::Button*>& le::ButtonManager::GetAllButtons()
{
	return vButton;
}

//-------------------------------------------------------------------------//

int le::ButtonManager::GetTmpIdButton() const
{
	return iIdButton+1;
}

//-------------------------------------------------------------------------//
