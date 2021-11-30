#include "SceneManager.h"

// Scenes
#include "Scene.h"
#include "MenuScene.h"
#include "TestScene.h"
#include "FourSquareScene.h"
#include "VoronoiScene.h"
#include "Application.h"

SceneManager::SceneManager(sf::RenderWindow* in_window, GridData* in_gridData)
{
	m_window = in_window;
	m_gridData = in_gridData;
	m_menuScene = new MenuScene(m_window, this);
	m_activeScenes.insert(std::make_pair(SceneName::MAIN_MENU, m_menuScene));

	SwitchScene(SceneName::MAIN_MENU);
}

void SceneManager::Update(float frame_time)
{
	m_currentScene->Update(frame_time);
}

void SceneManager::Render()
{
	m_currentScene->Render();
}

void SceneManager::Cleanup()
{
	if (m_menuScene)
		m_menuScene->Cleanup();

	if (m_testScene)
		m_testScene->Cleanup();

	if (m_fourSquareScene)
		m_fourSquareScene->Cleanup();

	delete m_menuScene;
	delete m_testScene;
	delete m_fourSquareScene;
	m_menuScene = nullptr;
	m_testScene = nullptr;
	m_fourSquareScene = nullptr;
}

void SceneManager::ResizeEvent()
{
	m_currentScene->ResizeEvent();
}

void SceneManager::SwitchScene(SceneName in_scene)
{
	switch (in_scene)
	{
	case SceneName::NONE:
		break;

	case SceneName::MAIN_MENU:
		m_currentScene = m_menuScene;
		m_sceneEnum = SceneName::MAIN_MENU;
		break;

	case SceneName::FOUR_SQUARE:
		if (m_activeScenes.find(SceneName::FOUR_SQUARE) == m_activeScenes.end())
		{
			m_fourSquareScene = new FourSquareScene(m_window, this);
			m_activeScenes.insert(std::make_pair(SceneName::FOUR_SQUARE, m_fourSquareScene));
		}
		m_currentScene = m_fourSquareScene;
		m_sceneEnum = SceneName::FOUR_SQUARE;
		break;

	case SceneName::VORONOI:
		if (m_activeScenes.find(SceneName::VORONOI) == m_activeScenes.end())
		{
			m_voronoiScene = new VoronoiScene(m_window, this);
			m_activeScenes.insert(std::make_pair(SceneName::VORONOI, m_voronoiScene));
		}
		m_currentScene = m_voronoiScene;
		m_sceneEnum = SceneName::VORONOI;
		break;

	case SceneName::TEST_SCENE:
		if (m_activeScenes.find(SceneName::TEST_SCENE) == m_activeScenes.end())
		{
			m_testScene = new TestScene(m_window, this);
			m_activeScenes.insert(std::make_pair(SceneName::TEST_SCENE, m_testScene));
		}
		m_currentScene = m_testScene;
		m_sceneEnum = SceneName::TEST_SCENE;
		break;
	default:
		break;
	}
}

GridData* SceneManager::GetGrid()
{
	return m_gridData;
}