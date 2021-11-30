#pragma once
#include "Scene.h"

class MenuScene : public Scene
{
public:
	MenuScene(sf::RenderWindow* in_window, SceneManager* in_managerRef);
	void Update(float frame_time) override;
	void Render() override;
	void Cleanup() override;

	// events
	void ResizeEvent() override;
};

