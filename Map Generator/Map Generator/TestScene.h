#pragma once
#include "Scene.h"

class TestScene : public Scene
{
public:
	TestScene(sf::RenderWindow* in_window, SceneManager* in_managerRef);
	void Update(float frame_time) override;
	void Render() override;
	void Cleanup() override;


	// events
	void ResizeEvent() override;
private:
	float color[3] = { 0.f, 0.f, 0.f };

	char windowTitle[255] = "ImGui + SFML = <3";

	sf::Color bgColor;
};

