# lifeEngine
Это игровой 3D-движок, написанный на С++ и распространяющийся под лицензией [GNU LGPL](https://ru.wikipedia.org/wiki/GNU_Lesser_General_Public_License). Он обладает высокоуровневым API которое упрощающает разработку игры.

## Подключение к проету
**Для того чтобы подключить движок к проету необходимо сделать следующие:**
1. Создать консольный или оконный проект и добавить в него *.cpp файл
1. Зайти в свойства проекта
1. В поле конфигураций выбрать `"Все конфигурации"`
1. Перейти во вкладку `[C/C++ -> Общее]` и в поле `"Доп. каталоги включаемых файлов"` прописать такие пути: `<каталог lifeEngine>\lifeEngine`, `<каталог lifeEngine>\lifeEngine\Utils\Ogre3D\include`, `<каталог lifeEngine>\lifeEngine\Utils\Ogre3D\include\OgreMain\include`
1. Перейти во вкладку `[Компановщик -> Общее]` и в поле `"Доп. каталоги библиотек"` прописать такие пути для `Debug` конфигурации: `<каталог lifeEngine>\lifeEngine\Utils\Ogre3D\lib\Debug`, `<каталог lifeEngine>\bin`, а для `Release` такие: `<каталог lifeEngine>\lifeEngine\Utils\Ogre3D\lib\Release`, `<каталог lifeEngine>\bin`
1. Перейти во вкладку `[Компановщик -> Ввод]` и в поле "Доп. зависимости" прописать следующие для Debug: `lifeEngine-d.lib`, `OgreMain_d.lib`, а для Release `lifeEngine.lib`, `OgreMain.lib`
1. Нажать на `"Применить"`. Теперь проект готов к работе с движком

**Код для теста:**
```с++
#include <System\System.h>

class Game : public le::BasicApplication
{
public:
	Game( le::System& System ) : le::BasicApplication( System )
	{
	}

	void Update()
	{
	}
};

int main()
{
	le::System System( "../Config.cfg", "../Game.log", "../Plugins" );
	System.WindowCreate( ENGINE " | " ENGINE_VERSION );

	Game Game( System );
	System.MainLoop( Game );

	return 0;
}
```

## Компиляция движка
**Для того чтобы скомпилировать движок необходимо сделать следующие:**
1. Скачать `CMake` ([Скачать CMake](https://cmake.org/))
1. Скачать исходники `lifeEngine`
1. Скачать исходники `Ogre3D` и собрать его для вашей версии Visual Studio ([[Оф. сайт Ogre3D]](http://www.ogre3d.org/), [[Оф. статья по сборке]](http://www.ogre3d.org/tikiwiki/CMake+Quick+Start+Guide))
    1. Скачать `исходники` самого `Ogre3D` и его `зависимостей` ([[Скачать исходники Ogre3D]](https://bitbucket.org/sinbad/ogre), [[Скачать зависимости Ogre3D]](https://bitbucket.org/cabalistic/ogredeps))
    1. Извлечь все из архива (Для удобства советую извлекать примерно таким образом: папка `Dependencies - для зависимостей Ogre3D`, а папка `OgreSrc - для исходников Ogre3D`)
    1. Запустить `CMake` (<каталог cmake>\bin\cmake-gui.exe)
    1. В поле `"Where is the source code:"` указать путь к зависимостям (например, C:/Ogre3D/Dependencies)
    1. В поле `"Where to build the binaries:"` указать путь куда `CMake` положит сгенерирываные файлы (например, C:/Ogre3D/Dependencies/Build)
    1. Нажать на кнопку `Configure` и там выбрать вашу версию Студии (пункт `Specify the generator for this project`)
    1. После чего нажать на `Finish`. Пойдет процесс инициализации, если на этом этапе выскочит ошибка `"Error in configuration process..."`, то вам нужно убрать галачку с поля `OGREDEPS_BUILD_SDL2`.
    1. Нажать на кнопку `Generate`. После чего пойдет генерация бинарников (повторно на `Configure` нажимать не надо)
    1. В поле `"Where is the source code:"` указать путь к `исходникам Ogre3D` (например, C:/Ogre3D/OgreSrc)
    1. В поле `"Where to build the binaries:"` указать путь куда `CMake` положит сгенерирываные файлы (например, C:/Ogre3D/OgreSrc/Build)
    1. Нажать на кнопку `Configure`, выбрать свою версию студии и нажать на `Finish`.
    1. В пункте `OGRE_DEPENDENCIES_DIR` указать `каталог с зависимостями Ogre` (например, C:/Ogre3D/Dependencies), если вам `примеры Ogre` не нужны, то убрать галачку с `OGRE_INSTALL_SAMPLES и OGRE_BUILD_SAMPLES`
    1. Нажать на `Generate`. После чего пойдет процесс генерации.
    1. `Зайти в Build` (например, C:/Ogre3D/OgreSrc/Build), `запустить OGRE.sln` и `скомпилировать Ogre в Debug и Release конфигурациях` (Выбрать `ALL_BUILD` в качестве автозагружаемого проекта)
    1. Переместить в `<каталог lifeEngine>\lifeEngine\Utils\Ogre3D` bin и lib c `<каталог с исходниками Ogre3D>/Build`
    1. Переместить в `<каталог lifeEngine>\lifeEngine\Utils\Ogre3D\include` *.h файлы с каталогов `<каталог с исходниками Ogre3D>/Build/include`, `<каталог с исходниками Ogre3D>/OgreMain`, `<каталог с исходниками Ogre3D>/Components`, `<каталог с исходниками Ogre3D>/PlugIns`, `<каталог с исходниками Ogre3D>/RenderSystems`
1. `Запустить lifeEngine.sln` (лежит в корневом каталоге движка) и `скомпилировать lifeEngine в Debug и Release конфигурациях`
1. На выходе вы получите dll и lib которые будут лежат в `<каталог lifeEngine>\bin`. Вот и все, вы скомпилировали движок