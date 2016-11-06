# lifeEngine
Это игровой 2D-движок, написанный на С++ и распространяющийся под лицензией [GNU LGPL](https://ru.wikipedia.org/wiki/GNU_Lesser_General_Public_License). Он обладает высокоуровневым API которое упрощающает разработку игры.

На данный момент он в себя включает:
* Статическое освещение (имитация освещения)
* Загрузка игровых карт (XML формата)
* Работа с динамическими объектами (пули, персонажи и т.д.)
* Работа с анимациями
* Небольшой GUI (кнопки и текст)
* Возможность локализации
* Работа со звуком и музыкой (звук подерживает имитацию 3D пространства [подробнее здесь](http://www.sfml-dev.org/tutorials/2.4/audio-spatialization.php))

Для создания карт лучше всего использовать [Tiled Map Editor](http://www.mapeditor.org/), а для анимаций [Sprite Decomposer](https://github.com/eriknelson/SpriteDecomposer). 

## Подключение к проету
**Для начала необходимо добавить пути к .lib и .h файлам (Для Debug'a и Reales'a):**

Для этого заходим в **[Свойства проекта -> C/C++ -> Общее]** и в поле **"Дополнительные каталоги включаемых файлов"** вставляем  пути к таким папкам:

`[lifeengine-install-path]/lifeEngine`
 
`[lifeengine-install-path]/lifeEngine/Utils/Box2D`

`[lifeengine-install-path]/lifeEngine/Utils/XML`

`[lifeengine-install-path]/lifeEngine/Utils/SFML/include`

Далее заходим в  **[Свойства проекта -> Компановщик -> Общее]** и в поле **"Дополнительные каталоги библиотек"** вставляем  пути к таким папкам:

`[lifeengine-install-path]/bin`

`[lifeengine-install-path]/lifeEngine/Utils/Box2D`

`[lifeengine-install-path]/lifeEngine/Utils/SFML/lib`

**После этого нужно подрубить .lib'ы:**

Для этого заходим в  **[Свойства проекта -> Компановщик -> Ввод]** и в поле **"Дополнительные зависимости"** вставляем такие либы:

**Для Debug'a:**

`sfml-system-d.lib` `sfml-window-d.lib` `sfml-graphics-d.lib` `sfml-audio-d.lib` `box2d-d.lib` `lifeEngine-d.lib`

**Для Reales'a:**

`sfml-system.lib` `sfml-window.lib` `sfml-graphics.lib` `sfml-audio.lib` `box2d.lib` `lifeEngine.lib`

**И на последок нужно добавить в проект .cpp и .h файлы XML'a:**

Заходим в **[ [lifeengine-install-path]/lifeEngine/Utils/XML ]** и все файлы перетаскиваем в свой проект. Готово, проект настроен

## Код для теста

```сpp
#include <leSystem/leSystem.h>

class Game : public le::BasicStagesGame
{
public:
	Game( le::System& System )
	{
		TextManager = new le::TextManager( *System );

		TextManager->LoadFont( "Resources/Dikovina.ttf" );
		TextManager->WriteText( "This is WORK!", 25, Vector2f( 10, 10 ), Color::White );
		TextManager->WriteText( "SPARTA!!", 25, Vector2f( 20, 20 ), Color::Red );
	}

	~Game()
	{
		delete TextManager;
	}

	void CheckStages()
	{
		TextManager->UpdateText();
	}

private:
	le::TextManager* TextManager;
};

int main( int argc, char** argv )
{
	le::System System( "config.cfg", argc, argv );
	System.SetWindowTitle( "lifeEngine work!" );

	Game game( System );
	System.MainLoop( game );

	return 0;
}

```
