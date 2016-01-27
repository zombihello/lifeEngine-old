#ifndef LEVEL_MANAGER_H
#define LEVEL_MANAGER_H

/////////////////
//	LIFEENGINE
/////////////////
#include "System.h"

namespace le
{
    struct Object
    {
        int GetPropertyInt( string name );
        float GetPropertyFloat( string name );
        string GetPropertyString( string name );

        string name;
        string type;
        Rect<float> rect;
        map<string , string> properties;

        Sprite sprite;
    };

    struct Layer
    {
        int opacity;
        vector<Sprite> tiles;
    };

    class LevelManager
    {
    public:
        LevelManager();
        ~LevelManager();

        bool LoadFromFile( string filename );
        void Clear();
        void Draw( RenderWindow &window );

        Object GetObject( string name ) const;
        vector<Object> GetObjects( string name ) const;
        vector<Object> GetAllObjects() const;
        Vector2i GetTileSize() const;
        Vector2i GetMapSize() const;

        int width , height , tileWidth , tileHeight;
    private:
        int firstTileID;
        Rect<float> drawingBounds;
        Texture tilesetImage;
        vector<Object> objects;
        vector<Layer> layers;
    };
}

#endif // LEVEL_MANAGER_H