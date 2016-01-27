#ifndef BASIC_STAGE_GAME_H
#define BASIC_STAGE_GAME_H

namespace le
{
    class BasicStageGame
    {
    public:
        //////////////////////////////////////////////
        // CHECK STAGES GAME | ПРОВЕРКА ЭТАПОВ ИГРЫ
        //////////////////////////////////////////////
        virtual void CheckStages() = 0;
    private:
        int     iTmpStage;

        enum TYPES_STAGES
        {
            MENU_NOT_LOADED ,
            MENU_LOADED ,
            GAME_NOT_LOADED ,
            GAME_LOADED
        } TypesStages;
    };
}

#endif // BASIC_STAGE_GAME_H

