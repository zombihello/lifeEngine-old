/////////////////////////////////////
// This is example work this engine
//////////////////////////////////////
#include "headerFiles\System.h"

class StageGame : public le::BasicStageGame
{
public:
    StageGame( le::System& System )
    {
        this->System = &System;
        System.GetMouseCursor().LoadTexture( "cur.png" );
    }
    void CheckStages()
    {
        System->GetMouseCursor().DrawCursor( System->GetWindow() );
    }
private:
    le::System*     System;
};

int main()
{
    le::System System;
    StageGame StageGame( System );

    System.MainLoop( StageGame );

    return 0;
}