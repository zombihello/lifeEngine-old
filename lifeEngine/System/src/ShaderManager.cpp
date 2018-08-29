#include "..\ShaderManager.h"

//-------------------------------------------------------------------------//

le::GraphicsSettings le::ShaderManager::GraphicsSettings = le::GraphicsSettings();

//-------------------------------------------------------------------------//

le::GraphicsSettings::GraphicsSettings() :
	QualityShadows( le::QualityShadows::High )
{}

//-------------------------------------------------------------------------//

void le::ShaderManager::LoadShaders()
{
	//TODO: [zombiHello] Сделать убершейдер для этих целей

	string PointLight_FragmentShader;
	string SpotLight_FragmentShader;
	string DirectionalLight_FragmentShader;

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

	ResourcesManager::LoadShader( "AnimationModels", "../shaders/geometry/AnimationModelsRender.vs", "../shaders/geometry/AnimationModelsRender.fs" );
	ResourcesManager::LoadShader( "StaticModels", "../shaders/geometry/StaticModelsRender.vs", "../shaders/geometry/StaticModelsRender.fs" );
	ResourcesManager::LoadShader( "StaticLevel", "../shaders/geometry/Static_LevelRender.vs", "../shaders/geometry/Static_LevelRender.fs" );
	ResourcesManager::LoadShader( "DynamicLevel", "../shaders/geometry/Dynamic_LevelRender.vs", "../shaders/geometry/Dynamic_LevelRender.fs" );

	ResourcesManager::LoadShader( "AnimationModels_GBuffer", "../shaders/geometry/AnimationModelsRender_GBuffer.vs", "../shaders/geometry/AnimationModelsRender_GBuffer.fs" );
	ResourcesManager::LoadShader( "StaticModels_GBuffer", "../shaders/geometry/StaticModelsRender_GBuffer.vs", "../shaders/geometry/StaticModelsRender_GBuffer.fs" );
	ResourcesManager::LoadShader( "StaticLevel_GBuffer", "../shaders/geometry/Static_LevelRender_GBuffer.vs", "../shaders/geometry/Static_LevelRender_GBuffer.fs" );
	ResourcesManager::LoadShader( "DynamicLevel_GBuffer", "../shaders/geometry/Dynamic_LevelRender_GBuffer.vs", "../shaders/geometry/Dynamic_LevelRender_GBuffer.fs" );

	ResourcesManager::LoadShader( "ShadowMap", "../shaders/light/ShadowMapRender.vs", "../shaders/light/ShadowMapRender.fs" );
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
	ResourcesManager::DeleteShader( "AnimationModels_GBuffer" );
	ResourcesManager::DeleteShader( "StaticModels_GBuffer" );
	ResourcesManager::DeleteShader( "Brushes_GBuffer" );
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