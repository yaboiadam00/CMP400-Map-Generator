#pragma once
#include "SFML/Graphics.hpp"
#include <unordered_map>

enum class SceneName
{
	NONE = 0,
	MAIN_MENU,
	FOUR_SQUARE,
	VORONOI,
	TEST_SCENE,
};

class Scene;
class MenuScene;
class TestScene;
class FourSquareScene;
class VoronoiScene;
class GridData;

class SceneManager
{
public:
	SceneManager(sf::RenderWindow*, GridData*);
	void Update(float frame_time);
	void Render();
	void Cleanup();

	// events
	void ResizeEvent();

	void SwitchScene(SceneName in_scene);
	SceneName GetCurrentScene() { return m_sceneEnum; }
	GridData* GetGrid();
private:
	std::unordered_map<SceneName, Scene*> m_activeScenes;
	sf::RenderWindow* m_window;
	Scene* m_currentScene = nullptr;
	MenuScene* m_menuScene = nullptr;
	TestScene* m_testScene = nullptr;
	FourSquareScene* m_fourSquareScene = nullptr;
	VoronoiScene* m_voronoiScene = nullptr;
	SceneName m_sceneEnum = SceneName::NONE;
	GridData* m_gridData;
};
