#include "MenuScene.h"
#include <fstream>

MenuScene::MenuScene(sf::RenderWindow* in_window, SceneManager* in_managerRef) :
	Scene(in_window, in_managerRef)
{
}

void MenuScene::Update(float frame_time)
{
}

void MenuScene::Render()
{
	ImGui::Begin("MainMenu", 0, m_windowFlags); // begin window
	ImGui::SetWindowSize("MainMenu", m_window->getSize());
	ImGui::SetWindowPos("MainMenu", ImVec2(0, 0));

	if (ImGui::Button("Set Four Square Anchor Points"))
	{
		m_managerRef->SwitchScene(SceneName::FOUR_SQUARE);
		// Switch to 4 square scene
	}

	if (ImGui::Button("Set Voronoi Regions"))
	{
		m_managerRef->SwitchScene(SceneName::VORONOI);
		// Switch to Voronoi scene
	}

	if (ImGui::Button("Set Wave Function Collapse"))
	{
		// Switch to Wave Function Collapse
	}

	if (ImGui::Button("Write Grid Data to BMP"))
	{
		std::vector<int> cells;

		for (int i = 0; i < m_managerRef->GetGrid()->GetSize(); i++)
		{
			switch (m_managerRef->GetGrid()->GetCell(i))
			{
			case CellType::NONE:
				cells.push_back((int)CellType::NONE);
				break;
			case CellType::WALL:
				cells.push_back((int)CellType::WALL);
				break;
			case CellType::WALKABLE:
				cells.push_back((int)CellType::WALKABLE);
				break;
			case CellType::BUILDING:
				cells.push_back((int)CellType::BUILDING);
				break;
			default:
				break;
			}
		}

		Utils::WriteTOBMP("output.bmp", m_managerRef->GetGrid()->GetWidth(), m_managerRef->GetGrid()->GetWidth(), cells);
	}

	ImGui::End(); // end window

	m_window->clear(m_backgroundColor);
	//window.draw(shape);
	ImGui::SFML::Render(*m_window);
	m_window->display();
}

void MenuScene::Cleanup()
{
}

void MenuScene::ResizeEvent()
{
}