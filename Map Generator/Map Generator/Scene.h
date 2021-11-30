#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Color.hpp"
#include "imgui/imgui.h"
#include "imgui-sfml/imgui-sfml.h"
#include "SceneManager.h"
#include "Application.h"

class Scene
{
public:
	Scene(sf::RenderWindow*, SceneManager*);
	virtual void Update(float frame_time);
	virtual void Render();
	virtual void Cleanup();

	// events
	virtual void ResizeEvent();

protected:
	int index(int x, int y);

	sf::RenderWindow* m_window = nullptr;
	SceneManager* m_managerRef = nullptr;
	ImGuiWindowFlags m_windowFlags = NULL;
	sf::Color m_backgroundColor;
	sf::Color m_accentColor1;
	sf::Color m_accentColor2;
};
