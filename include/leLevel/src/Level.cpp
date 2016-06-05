#include "../Level.h"

//-------------------------------------------------------------------------//

le::Level::Level( le::System &System )
{
	RenderWindow = &System.GetWindow();
}

//-------------------------------------------------------------------------//

le::Level::~Level()
{
	ClearLevel();
}

//-------------------------------------------------------------------------//

bool le::Level::LoadLevel( const string sRoute, le::Physic& Physic )
{
	TiXmlDocument levelFile( sRoute.c_str() );

	// Загружаем XML-карту
	if ( !levelFile.LoadFile() )
	{
		cout << "Error: File [" << sRoute << "] Not Found" << endl;
		return false;
	}

	// Работаем с контейнером map
	TiXmlElement *map;
	map = levelFile.FirstChildElement( "map" );

	// Пример карты: <map version="1.0" orientation="orthogonal"
	// width="10" height="10" tilewidth="34" tileheight="34">
	iWidth = atoi( map->Attribute( "width" ) );
	iHeight = atoi( map->Attribute( "height" ) );
	iTileWidth = atoi( map->Attribute( "tilewidth" ) );
	iTileHeight = atoi( map->Attribute( "tileheight" ) );

	// Берем описание тайлсета и идентификатор первого тайла
	TiXmlElement *tilesetElement;
	tilesetElement = map->FirstChildElement( "tileset" );
	iFirstTileID = atoi( tilesetElement->Attribute( "firstgid" ) );

	// source - путь до картинки в контейнере image
	TiXmlElement *image;
	image = tilesetElement->FirstChildElement( "image" );
	std::string imagepath = image->Attribute( "source" );

	// Пытаемся загрузить тайлсет
	sf::Image img;

	if ( !img.loadFromFile( imagepath ) )
	{
		std::cout << "Error: Failed to load tile sheet." << std::endl;
		return false;
	}


	img.createMaskFromColor( sf::Color( 255 , 255 , 255 ) );
	tilesetImage.loadFromImage( img );
	tilesetImage.setSmooth( false );

	// Получаем количество столбцов и строк тайлсета
	int columns = tilesetImage.getSize().x / iTileWidth;
	int rows = tilesetImage.getSize().y / iTileHeight;

	// Вектор из прямоугольников изображений (TextureRect)
	std::vector<sf::Rect<int>> subRects;

	for ( int y = 0; y < rows; y++ )
		for ( int x = 0; x < columns; x++ )
		{
			sf::Rect<int> rect;

			rect.top = y * iTileHeight;
			rect.height = iTileHeight;
			rect.left = x * iTileWidth;
			rect.width = iTileWidth;

			subRects.push_back( rect );
		}

	// Работа со слоями
	TiXmlElement *layerElement;
	layerElement = map->FirstChildElement( "layer" );
	while ( layerElement )
	{
		Layer layer;

		// Если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
		if ( layerElement->Attribute( "opacity" ) != NULL )
		{
			float opacity = strtod( layerElement->Attribute( "opacity" ) , NULL );
			layer.iOpacity = 255 * opacity;
		}
		else
			layer.iOpacity = 255;

		// Контейнер <data>
		TiXmlElement *layerDataElement;
		layerDataElement = layerElement->FirstChildElement( "data" );

		if ( layerDataElement == NULL )
			std::cout << "Error: Bad map. No layer information found." << std::endl;

		// Контейнер <tile> - описание тайлов каждого слоя
		TiXmlElement *tileElement;
		tileElement = layerDataElement->FirstChildElement( "tile" );

		if ( tileElement == NULL )
		{
			std::cout << "Error: Bad map. No tile information found." << std::endl;
			return false;
		}

		int x = 0;
		int y = 0;

		while ( tileElement )
		{
			int tileGID = atoi( tileElement->Attribute( "gid" ) );
			int subRectToUse = tileGID - iFirstTileID;

			// Устанавливаем TextureRect каждого тайла
			if ( subRectToUse >= 0 )
			{
				sf::Sprite sprite;
				sprite.setTexture( tilesetImage );
				sprite.setTextureRect( subRects[ subRectToUse ] );
				sprite.setPosition( x * iTileWidth , y * iTileHeight );
				sprite.setColor( sf::Color( 255 , 255 , 255 , layer.iOpacity ) );

				layer.tiles.push_back( sprite );
			}

			tileElement = tileElement->NextSiblingElement( "tile" );

			x++;
			if ( x >= iWidth )
			{
				x = 0;
				y++;
				if ( y >= iHeight )
					y = 0;
			}
		}

		vLayers.push_back( layer );

		layerElement = layerElement->NextSiblingElement( "layer" );
	}

	// Работа с объектами
	TiXmlElement *objectGroupElement;

	// Если есть слои объектов
	if ( map->FirstChildElement( "objectgroup" ) != NULL )
	{
		objectGroupElement = map->FirstChildElement( "objectgroup" );
		while ( objectGroupElement )
		{
			// Контейнер <object>
			TiXmlElement *objectElement;
			objectElement = objectGroupElement->FirstChildElement( "object" );

			while ( objectElement )
			{
				// Получаем все данные - тип, имя, позиция, etc
				std::string objectType;
				if ( objectElement->Attribute( "type" ) != NULL )
				{
					objectType = objectElement->Attribute( "type" );
				}
				std::string objectName;
				if ( objectElement->Attribute( "name" ) != NULL )
				{
					objectName = objectElement->Attribute( "name" );
				}
				int x = atoi( objectElement->Attribute( "x" ) );
				int y = atoi( objectElement->Attribute( "y" ) );

				int width , height;

				sf::Sprite sprite;
				sprite.setTexture( tilesetImage );
				sprite.setTextureRect( sf::Rect<int>( 0 , 0 , 0 , 0 ) );
				sprite.setPosition( x , y );

				if ( objectElement->Attribute( "width" ) != NULL )
				{
					width = atoi( objectElement->Attribute( "width" ) );
					height = atoi( objectElement->Attribute( "height" ) );
				}
				else
				{
					width = subRects[ atoi( objectElement->Attribute( "gid" ) ) - iFirstTileID ].width;
					height = subRects[ atoi( objectElement->Attribute( "gid" ) ) - iFirstTileID ].height;
					sprite.setTextureRect( subRects[ atoi( objectElement->Attribute( "gid" ) ) - iFirstTileID ] );
				}

				float roation = 0;
				if ( objectElement->Attribute( "rotation" ) != NULL )
					roation = strtod( objectElement->Attribute( "rotation" ), NULL );

				// Экземпляр объекта
				Object object( Physic );
				object.sName = objectName;
				object.sType = objectType;
				object.sprite = sprite;

				sf::Rect <float> objectRect;
				objectRect.top = y;
				objectRect.left = x;
				objectRect.height = height;
				objectRect.width = width;
				object.rect = objectRect;
				object.Rotation = roation;

				// "Переменные" объекта
				TiXmlElement *properties;
				properties = objectElement->FirstChildElement( "properties" );
				if ( properties != NULL )
				{
					TiXmlElement *prop;
					prop = properties->FirstChildElement( "property" );
					if ( prop != NULL )
					{
						while ( prop )
						{
							string propertyName = prop->Attribute( "name" );
							string propertyValue = prop->Attribute( "value" );

							object.mProperties[ propertyName ] = propertyValue;

							prop = prop->NextSiblingElement( "property" );
						}
					}
				}


				vObjects.push_back( object );

				objectElement = objectElement->NextSiblingElement( "object" );
			}
			objectGroupElement = objectGroupElement->NextSiblingElement( "objectgroup" );
		}

		for ( int i = 0; i < vObjects.size(); i++ )
		{
			Object* obj = &vObjects[ i ];

			if ( obj->sType == "solid" )
			{
				obj->BodyObject = new le::Body( Vector2f( obj->rect.left + obj->rect.width/2, obj->rect.top + obj->rect.height/2 ), obj->Rotation, obj->sName );

				if ( obj->mProperties.count( "friction" ) != 0 )
					obj->BodyObject->SetPropirtes( 1, 0, obj->GetPropertyFloat( "friction" ) );
				else
					obj->BodyObject->SetPropirtes( 1, 0, 0.2f );

				Physic.CreateBody( obj->BodyObject );

				obj->BodyObject->CreatePolygonShape( Vector2f( obj->rect.width, obj->rect.height ) );

			}
		}

	}
	else
		cout << "Error: No object layers found..." << endl;

	return true;
}

//-------------------------------------------------------------------------//

void le::Level::ClearLevel()
{
	vObjects.clear();
	vLayers.clear();
}

//-------------------------------------------------------------------------//

void le::Level::DrawLevel()
{
	// Рисуем все тайлы (объекты НЕ рисуем!)
	for ( int layer = 0; layer < vLayers.size(); layer++ )
		for ( int tile = 0; tile < vLayers[ layer ].tiles.size(); tile++ )
			RenderWindow->draw( vLayers[ layer ].tiles[ tile ] );
}

//-------------------------------------------------------------------------//

le::Object& le::Level::GetObject( const string sName )
{
	// Только первый объект с заданным именем
	for ( int i = 0; i < vObjects.size(); i++ )
		if ( vObjects[ i ].sName == sName )
			return vObjects[ i ];
}

//-------------------------------------------------------------------------//

vector<le::Object> le::Level::GetObjects( const string sName )
{
	// Все объекты с заданным именем
	vector<Object> vec;
	for ( int i = 0; i < vObjects.size(); i++ )
		if ( vObjects[ i ].sName == sName )
			vec.push_back( vObjects[ i ] );

	return vec;
}

//-------------------------------------------------------------------------//

vector<le::Object>& le::Level::GetAllObjects()
{
	return vObjects;
}

//-------------------------------------------------------------------------//

Vector2i le::Level::GetTileSize() const
{
	return Vector2i( iTileWidth , iTileHeight );
}

//-------------------------------------------------------------------------//

Vector2i le::Level::GetMapSize() const
{
	return Vector2i( iWidth * iTileWidth, iHeight * iTileHeight );
}

//-------------------------------------------------------------------------//
