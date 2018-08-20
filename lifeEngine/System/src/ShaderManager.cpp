#include "..\ShaderManager.h"

//-------------------------------------------------------------------------//

le::GraphicsSettings le::ShaderManager::GraphicsSettings = le::GraphicsSettings();

//-------------------------------------------------------------------------//

le::GraphicsSettings::GraphicsSettings() :
	DynamicLights( true ),
	QualityShadows( le::QualityShadows::High )
{}

//-------------------------------------------------------------------------//

void le::ShaderManager::LoadShaders()
{
	//TODO: [zombiHello] Сделать убершейдер для этих целей

	string LevelRender_VertexShader;
	string LevelRender_FragmentShader;
	string AnimationModels_VertexShader;
	string AnimationModels_FragmentShader;
	string StaticModels_VertexShader;
	string StaticModels_FragmentShader;
	string PointLight_FragmentShader;
	string SpotLight_FragmentShader;
	string DirectionalLight_FragmentShader;

	// ******************************************
	// Определяем путь к шейдеру рендера сцены
	// в засимости включен/выключен дин. свет

	if ( GraphicsSettings.DynamicLights )
	{
		LevelRender_VertexShader = "../shaders/geometry/LevelRender_GBuffer.vs";
		LevelRender_FragmentShader = "../shaders/geometry/LevelRender_GBuffer.fs";
		AnimationModels_VertexShader = "../shaders/geometry/AnimationModelsRender_GBuffer.vs";
		AnimationModels_FragmentShader = "../shaders/geometry/AnimationModelsRender_GBuffer.fs";
		StaticModels_VertexShader = "../shaders/geometry/StaticModelsRender_GBuffer.vs";
		StaticModels_FragmentShader = "../shaders/geometry/StaticModelsRender_GBuffer.fs";
	}
	else
	{
		LevelRender_VertexShader = "../shaders/geometry/LevelRender.vs";
		LevelRender_FragmentShader = "../shaders/geometry/LevelRender.fs";
		AnimationModels_VertexShader = "../shaders/geometry/AnimationModelsRender.vs";
		AnimationModels_FragmentShader = "../shaders/geometry/AnimationModelsRender.fs";
		StaticModels_VertexShader = "../shaders/geometry/StaticModelsRender.vs";
		StaticModels_FragmentShader = "../shaders/geometry/StaticModelsRender.fs";
	}

	// ******************************************
	// Определяем путь к шейдерам освещения в 
	// зависимости от качества теней

	switch ( GraphicsSettings.QualityShadows )
	{
	case QualityShadows::None:
		PointLight_FragmentShader = "../shaders/light/PointLightRender_ShadowNone.fs";
		SpotLight_FragmentShader = "../shaders/light/SpotLightRender_ShadowNone.fs";
		DirectionalLight_FragmentShader = "../shaders/light/DirectionalLightRender_ShadowNone.fs";
		break;

	case QualityShadows::Low:
		PointLight_FragmentShader = "../shaders/light/PointLightRender_ShadowLow.fs";
		SpotLight_FragmentShader = "../shaders/light/SpotLightRender_ShadowLow.fs";
		DirectionalLight_FragmentShader = "../shaders/light/DirectionalLightRender_ShadowLow.fs";
		break;

	case QualityShadows::Medium:
		PointLight_FragmentShader = "../shaders/light/PointLightRender_ShadowMedium.fs";
		SpotLight_FragmentShader = "../shaders/light/SpotLightRender_ShadowMedium.fs";
		DirectionalLight_FragmentShader = "../shaders/light/DirectionalLightRender_ShadowMedium.fs";
		break;

	default:
	case QualityShadows::High:
		PointLight_FragmentShader = "../shaders/light/PointLightRender_ShadowHigh.fs";
		SpotLight_FragmentShader = "../shaders/light/SpotLightRender_ShadowHigh.fs";
		DirectionalLight_FragmentShader = "../shaders/light/DirectionalLightRender_ShadowHigh.fs";
		break;
	}

	// ********************************
	// Загружаем шейдера

	ResourcesManager::LoadShader( "ShadowMap", "../shaders/light/ShadowMapRender.vs", "../shaders/light/ShadowMapRender.fs" );
	ResourcesManager::LoadShader( "AnimationModels", AnimationModels_VertexShader, AnimationModels_FragmentShader );
	ResourcesManager::LoadShader( "StaticModels", StaticModels_VertexShader, StaticModels_FragmentShader );
	ResourcesManager::LoadShader( "Brushes", LevelRender_VertexShader, LevelRender_FragmentShader );
	ResourcesManager::LoadShader( "TestRender", "../shaders/TestRender.vs", "../shaders/TestRender.fs" );
	ResourcesManager::LoadShader( "PointLight", "../shaders/light/PointLightRender.vs", PointLight_FragmentShader );
	ResourcesManager::LoadShader( "DirectionalLight", "../shaders/light/DirectionalLightRender.vs", DirectionalLight_FragmentShader );
	ResourcesManager::LoadShader( "SpotLight", "../shaders/light/SpotLightRender.vs", SpotLight_FragmentShader );
}

//-------------------------------------------------------------------------//

void le::ShaderManager::DeleteShaders()
{
	ResourcesManager::DeleteShader( "ShadowMap" );
	ResourcesManager::DeleteShader( "AnimationModels" );
	ResourcesManager::DeleteShader( "StaticModels" );
	ResourcesManager::DeleteShader( "Brushes" );
	ResourcesManager::DeleteShader( "TestRender" );
	ResourcesManager::DeleteShader( "PointLight" );
	ResourcesManager::DeleteShader( "DirectionalLight" );
	ResourcesManager::DeleteShader( "SpotLight" );
}

//-------------------------------------------------------------------------//

void le::ShaderManager::SetGraphicsSettings( le::GraphicsSettings& GraphicsSettings )
{
	le::ShaderManager::GraphicsSettings = GraphicsSettings;
}

//-------------------------------------------------------------------------//

le::GraphicsSettings& le::ShaderManager::GetGraphicsSettings()
{
	return GraphicsSettings;
}

//-------------------------------------------------------------------------//