#include "ButtonManager.h"


le::ButtonManager::ButtonManager( le::System & System )
{
    this->System = &System;
    RenderWindow = &System.GetWindow();
    CursorPosition = &System.GetMouseCursor().GetVectorCursor();

    iIdButton = -1;
}

le::ButtonManager::~ButtonManager()
{
    DeleteAllButtons();
}

void le::ButtonManager::CreateButton( const Text & Text )
{
    vButton.push_back( new Button( *System ) );

    vButton[ vButton.size() - 1 ]->CreateButton( Text );
    vButton[ vButton.size() - 1 ]->SetId( vButton.size() );
}

void le::ButtonManager::UpdateButtons()
{
    iIdButton = -1;

    for ( int i = 0; i < vButton.size(); i++ )
    {
        if ( vButton[ i ]->GetRectButton().intersects( System->GetMouseCursor().GetRectCursor() ) )
            iIdButton = i;

        vButton[ i ]->UpdateButton();
    }
}

void le::ButtonManager::DeleteAllButtons()
{
    for ( int i = 0; i < vButton.size(); i++ )
        delete vButton[ i ];

    iIdButton = -1;
    vButton.clear();
}

void le::ButtonManager::DeleteButton( const int id )
{
    if ( id - 1 > -1 && id - 1 < vButton.size() )
        vButton.erase( vButton.begin() + id - 1 );
}

int le::ButtonManager::GetTmpIdButton() const
{
    return iIdButton;
}
