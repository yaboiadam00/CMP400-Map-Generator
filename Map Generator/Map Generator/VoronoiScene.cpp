#include "VoronoiScene.h"
#include "Application.h"
#include "Timer.h"
#include "Maths.h"

VoronoiScene::VoronoiScene(sf::RenderWindow* in_window, SceneManager* in_managerRef) : Scene(in_window, in_managerRef)
{
	InitWalls();

	InitGrid();

	InitVoronoiPoints();

	CalculateRegions(m_useManhattan);
}

void VoronoiScene::InitVoronoiPoints()
{
	// Resize Vector to fit
	if (m_voronoiPoints.size() < m_numberOfPoints)
	{
		int circlesToAdd = m_numberOfPoints - m_voronoiPoints.size();

		for (int i = 0; i < circlesToAdd; i++)
		{
			m_voronoiPoints.push_back(new AEVoronoiPoint);
			m_voronoiPoints.back()->m_point.setOutlineColor(sf::Color::Black);
			m_voronoiPoints.back()->m_point.setRadius(10.f);
			sf::Vector2f newPosition;
			newPosition.x = Utils::MapToRange(std::rand() % 1000, 0, 999, 0, m_window->getSize().x);
			newPosition.y = Utils::MapToRange(std::rand() % 1000, 0, 999, 0, m_window->getSize().y);
			m_voronoiPoints.back()->m_point.setPosition(newPosition);
		}
	}
	else if (m_voronoiPoints.size() > m_numberOfPoints)
	{
		for (int i = m_voronoiPoints.size(); i > m_numberOfPoints; i--)
		{
			delete  m_voronoiPoints[i];
			m_voronoiPoints[i] = nullptr;
		}
		m_voronoiPoints.resize(m_numberOfPoints);
	}
}

void VoronoiScene::RegenPoints()
{
	// Resize Vector to fit
	if (m_voronoiPoints.size() < m_numberOfPoints)
	{
		int circlesToAdd = m_numberOfPoints - m_voronoiPoints.size();

		for (int i = 0; i < circlesToAdd; i++)
		{
			m_voronoiPoints.push_back(new AEVoronoiPoint);
			m_voronoiPoints.back()->m_point.setOutlineColor(sf::Color::Black);
			m_voronoiPoints.back()->m_point.setRadius(10.f);
			sf::Vector2f newPosition;
			newPosition.x = Utils::MapToRange(std::rand() % 1000, 0, 999, 0, m_window->getSize().x);
			newPosition.y = Utils::MapToRange(std::rand() % 1000, 0, 999, 0, m_window->getSize().y);
			m_voronoiPoints.back()->m_point.setPosition(newPosition);
		}
	}
	else if (m_voronoiPoints.size() > m_numberOfPoints)
	{
		for (int i = m_voronoiPoints.size() - 1; i > m_numberOfPoints; i--)
		{
			delete  m_voronoiPoints[i];
			m_voronoiPoints[i] = nullptr;
		}
		m_voronoiPoints.resize(m_numberOfPoints);
	}

	for (int i = 0; i < m_numberOfPoints; i++)
	{
		sf::Vector2f newPosition;
		newPosition.x = Utils::MapToRange(std::rand() % 1000, 0, 999, 0, m_window->getSize().x);
		newPosition.y = Utils::MapToRange(std::rand() % 1000, 0, 999, 0, m_window->getSize().y);
		m_voronoiPoints[i]->m_point.setPosition(newPosition);
	}
}

void VoronoiScene::CalculateRegions(bool manhattan = true)
{
	int t_gridWidth = m_managerRef->GetGrid()->GetWidth();
	int t_gridHeight = m_managerRef->GetGrid()->GetHeight();

	for (int x = 0; x < t_gridWidth; x++)
	{
		for (int y = 0; y < t_gridHeight; y++)
		{
			int closest = 0;
			for (int points = 0; points < m_numberOfPoints; points++)
			{
				if (manhattan)
				{
					if (
						Utils::ManhattenDistance(m_rectGrid[index(x, y)]->getPosition(), m_voronoiPoints[points]->m_point.getPosition()) <
						Utils::ManhattenDistance(m_rectGrid[index(x, y)]->getPosition(), m_voronoiPoints[closest]->m_point.getPosition()))
					{
						closest = points;
					}
				}
				else
				{
					if (
						Utils::Distance(m_rectGrid[index(x, y)]->getPosition(), m_voronoiPoints[points]->m_point.getPosition()) <
						Utils::Distance(m_rectGrid[index(x, y)]->getPosition(), m_voronoiPoints[closest]->m_point.getPosition()))
					{
						closest = points;
					}
				}
			}
			m_rectGrid[index(x, y)]->setFillColor(m_voronoiPoints[closest]->m_regionColor);
			m_managerRef->GetGrid()->SetVoronoi(m_voronoiPoints[closest]->type, sf::Vector2u{ (unsigned int)x, (unsigned int)y });
		}
	}
}

void VoronoiScene::InitGrid()
{
	int t_gridWidth = m_managerRef->GetGrid()->GetWidth();
	int t_gridHeight = m_managerRef->GetGrid()->GetHeight();
	float t_cellWidth = (float)m_window->getSize().x / (float)t_gridWidth;
	float t_cellHeight = (float)m_window->getSize().y / (float)t_gridHeight;

	// init grid
	for (int i = 0; i < m_managerRef->GetGrid()->GetSize(); i++)
	{
		m_rectGrid.push_back(nullptr);
	}

	for (int x = 0; x < t_gridWidth; x++)
	{
		for (int y = 0; y < t_gridHeight; y++)
		{
			m_rectGrid[index(x, y)] = new sf::RectangleShape(sf::Vector2f(
				t_cellWidth,
				t_cellHeight)
			);
			m_rectGrid[index(x, y)]->setFillColor(sf::Color::Black);
			m_rectGrid[index(x, y)]->setOutlineColor(sf::Color::Black);
			m_rectGrid[index(x, y)]->setOutlineThickness(2.5f);
			m_rectGrid[index(x, y)]->setPosition(sf::Vector2f(
				(x * t_cellWidth),
				(y * t_cellHeight)
			));
		}
	}
}

void VoronoiScene::InitWalls()
{
	for (int i = 0; i < 4; i++)
	{
		m_walls[i] = new AELine(m_window);
	}

	// TOP
	m_walls[0]->m_start = sf::Vector2f{ 0,0 };
	m_walls[0]->m_end = sf::Vector2f((float)m_window->getSize().x, 0);
	m_walls[0]->RecalculateCurve();

	// RIGHT
	m_walls[1]->m_start = sf::Vector2f(m_window->getSize().x, 0);
	m_walls[1]->m_end = sf::Vector2f{ (float)m_window->getSize().x,(float)m_window->getSize().y };
	m_walls[1]->RecalculateCurve();
	// BOTTOM
	m_walls[2]->m_start = sf::Vector2f{ (float)m_window->getSize().x,(float)m_window->getSize().y };
	m_walls[2]->m_end = sf::Vector2f{ 0,(float)m_window->getSize().y };
	m_walls[2]->RecalculateCurve();
	// LEFT
	m_walls[3]->m_start = sf::Vector2f{ 0,(float)m_window->getSize().y };
	m_walls[3]->m_end = sf::Vector2f{ 0,0 };
	m_walls[3]->RecalculateCurve();
}

void VoronoiScene::Update(float frame_time)
{
}

void VoronoiScene::Render()
{
	ImGui::Begin("Voronoi", 0, m_windowFlags); // begin window
	ImGui::SetWindowSize("Voronoi", m_window->getSize());
	ImGui::SetWindowPos("Voronoi", ImVec2(0, 0));

	if (ImGui::Button("Back"))
	{
		m_managerRef->SwitchScene(SceneName::MAIN_MENU);
	}

	if (ImGui::CollapsingHeader("VoronoiParams"))
	{
		if (ImGui::Button("Re-Calculate Poisitions"))
		{
			Timer recalcTimer("voronoi recalc");
			RegenPoints();
			CalculateRegions(m_useManhattan);
		}

		ImGui::SameLine();

		if (ImGui::Checkbox("manhattan", &m_useManhattan))
		{
			CalculateRegions(m_useManhattan);
		}

		if (ImGui::SliderInt("Number Of Points", &m_numberOfPoints, 1, 20))
		{
			RegenPoints();

			CalculateRegions(m_useManhattan);
		}
	}

	ImGui::End(); // end window
	m_window->clear(sf::Color::Red);

	for (int i = 0; i < m_rectGrid.size(); i++)
		m_window->draw(*m_rectGrid[i]);

	for (int i = 0; i < 4; i++)
	{
		m_walls[i]->Draw();
	}

	for (int i = 0; i < m_voronoiPoints.size(); i++)
		m_window->draw(m_voronoiPoints[i]->m_point);

	ImGui::SFML::Render(*m_window);
	m_window->display();
}

void VoronoiScene::Cleanup()
{
	for (int i = 0; i < 4; i++)
	{
		delete m_walls[i];
		m_walls[i] = nullptr;
	}
}

void VoronoiScene::ResizeEvent()
{
	std::cout << "new X: " << m_window->getSize().x << std::endl;
	std::cout << "new Y: " << m_window->getSize().y << std::endl;

	int t_gridWidth = m_managerRef->GetGrid()->GetWidth();
	int t_gridHeight = m_managerRef->GetGrid()->GetHeight();
	float t_cellWidth = (float)m_window->getSize().x / (float)t_gridWidth;
	float t_cellHeight = (float)m_window->getSize().y / (float)t_gridHeight;
	int t_fourSquareWidth = (m_window->getSize().x) / 3.f;
	int t_fourSquareHeight = (m_window->getSize().y) / 3.f;

	for (int x = 0; x < t_gridWidth; x++)
	{
		for (int y = 0; y < t_gridHeight; y++)
		{
			m_rectGrid[index(x, y)]->setSize(sf::Vector2f(
				t_cellWidth,
				t_cellHeight)
			);

			m_rectGrid[index(x, y)]->setPosition(sf::Vector2f(
				((float)x * t_cellWidth),
				((float)y * t_cellHeight)
			));
		}
	}

	// Resize Walls
	// TOP
	m_walls[0]->line[0].position = sf::Vector2f{ 0,0 };
	m_walls[0]->line[1].position = sf::Vector2f((float)m_window->getSize().x, 0);

	// RIGHT
	m_walls[1]->line[0].position = sf::Vector2f(m_window->getSize().x, 0);
	m_walls[1]->line[1].position = sf::Vector2f{ (float)m_window->getSize().x,(float)m_window->getSize().y };

	// BOTTOM
	m_walls[2]->line[0].position = sf::Vector2f{ (float)m_window->getSize().x,(float)m_window->getSize().y };
	m_walls[2]->line[1].position = sf::Vector2f{ 0,(float)m_window->getSize().y };

	// LEFT
	m_walls[3]->line[0].position = sf::Vector2f{ 0,(float)m_window->getSize().y };
	m_walls[3]->line[1].position = sf::Vector2f{ 0,0 };
}