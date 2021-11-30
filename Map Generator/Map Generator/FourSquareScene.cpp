#include "FourSquareScene.h"
#include "Application.h"
#include "Timer.h"
#include "Maths.h"
#include "curve.hpp"
#include <iostream>

FourSquareScene::FourSquareScene(sf::RenderWindow* in_window, SceneManager* in_managerRef) : Scene(in_window, in_managerRef)
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
			m_rectGrid[index(x, y)]->setOutlineColor(sf::Color::Black);
			m_rectGrid[index(x, y)]->setOutlineThickness(2.5f);
			m_rectGrid[index(x, y)]->setPosition(sf::Vector2f(
				(x * t_cellWidth),
				(y * t_cellHeight)
			));
		}
	}

	int t_fourSquareWidth = (m_window->getSize().x) / 3.f;
	int t_fourSquareHeight = (m_window->getSize().y) / 3.f;

	// init four square circles
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			m_points[x][y].m_point.setFillColor(sf::Color::White);
			m_points[x][y].m_point.setRadius(10);
			m_points[x][y].m_point.setOrigin(sf::Vector2f(
				m_points[x][y].m_point.getRadius(),
				m_points[x][y].m_point.getRadius()
			));
		}
	}

	// connect to neighbors
	ResetGridData();
	SetPointPositions();
	SetLinePositions();
	CalculateBezierHallway();
	UpdateColors();
}

void FourSquareScene::Update(float frame_time)
{
	int t_gridWidth = m_managerRef->GetGrid()->GetWidth();
	int t_gridHeight = m_managerRef->GetGrid()->GetHeight();

	for (int x = 0; x < t_gridWidth; x++)
	{
		for (int y = 0; y < t_gridHeight; y++)
		{
			sf::FloatRect bb = m_rectGrid[index(x, y)]->getGlobalBounds();

			if (bb.contains(sf::Mouse::getPosition(*m_window).x, sf::Mouse::getPosition(*m_window).y)) // collision
			{
				switch (m_managerRef->GetGrid()->GetCell(sf::Vector2u(x, y)))
				{
				case CellType::NONE:
					buffer = "";
					break;
				case CellType::WALL:
					buffer = "WALL";
					break;
				case CellType::WALKABLE:
					buffer = "WALKABLE";
					break;
				case CellType::BUILDING:
					buffer = "BUILDING";
					break;
				defualt:
					buffer = "";
					break;
				}
			}
		}
	}
}

void FourSquareScene::Render()
{
	// GUI
	ImGui::Begin("Four Square", 0, m_windowFlags); // begin window
	ImGui::SetWindowSize("Four Square", m_window->getSize());
	ImGui::SetWindowPos("Four Square", ImVec2(0, 0));

	if (ImGui::Button("Back"))
	{
		m_managerRef->SwitchScene(SceneName::MAIN_MENU);
	}

	if (ImGui::CollapsingHeader("Params"))
	{
		if (ImGui::Button("Re-Calculate Poisitions"))
		{
			Timer recalcTimer("four square recalc");
			ResetGridData();
			SetPointPositions();
			SetLinePositions();
			CalculateBezierHallway();
			UpdateColors();
		}

		if (ImGui::Checkbox("Use Straight Hallways", &m_useStraightHallways))
		{
			ResetGridData();
			CalculateBezierHallway();;
			UpdateColors();
		}

		if (ImGui::SliderInt("Curve Resolution", &m_hallResolution, 1, 30))
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					m_points[i][j].neighborOne.resolution = m_hallResolution;
					m_points[i][j].neighborTwo.resolution = m_hallResolution;
					m_points[i][j].neighborThree.resolution = m_hallResolution;
					m_points[i][j].neighborFour.resolution = m_hallResolution;
				}
			}
			ResetGridData();
			CalculateBezierHallway();;
			UpdateColors();
		}

		if (ImGui::SliderFloat("Bezier Hall Thickness", &m_bezierHallDistance, 0, 100))
		{
			ResetGridData();
			CalculateBezierHallway();;
			UpdateColors();
		}

		if (m_useStraightHallways)
		{
			if (ImGui::SliderFloat("Straight Hall Thickness", &m_straightHallDistance, 0, 100))
			{
				ResetGridData();
				CalculateBezierHallway();;
				UpdateColors();
			}
		}
	}

	if (view.data() != "")
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(view.data());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	ImGui::End(); // end window
	// GUI

	m_window->clear(sf::Color::Red);

	for (int i = 0; i < m_rectGrid.size(); i++)
		m_window->draw(*m_rectGrid[i]);

	//m_window->draw(testLine);

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			m_window->draw(m_points[x][y].m_point);
			if (m_points[x][y].one)
			{
				m_points[x][y].neighborOne.RecalculateCurve();
				m_points[x][y].neighborOne.Draw(m_window);
			}

			if (m_points[x][y].two)
			{
				m_points[x][y].neighborTwo.RecalculateCurve();
				m_points[x][y].neighborTwo.Draw(m_window);
			}

			if (m_points[x][y].three)
			{
				m_points[x][y].neighborThree.RecalculateCurve();
				m_points[x][y].neighborThree.Draw(m_window);
			}

			if (m_points[x][y].four)
			{
				m_points[x][y].neighborFour.RecalculateCurve();
				m_points[x][y].neighborFour.Draw(m_window);
			}
		}
	}

	ImGui::SFML::Render(*m_window);
	m_window->display();
}

void FourSquareScene::Cleanup()
{
	for (sf::RectangleShape* rect : m_rectGrid)
	{
		if (rect != nullptr)
		{
			delete rect;
			rect = nullptr;
		}
	}
	m_rectGrid.empty();
}

void FourSquareScene::ResizeEvent()
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

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			m_points[x][y].m_point.setPosition(
				x * t_fourSquareWidth + Utils::MapToRange(m_points[x][y].normalisedX, 0, 1, 0, t_fourSquareWidth),
				y * t_fourSquareHeight + Utils::MapToRange(m_points[x][y].normalisedY, 0, 1, 0, t_fourSquareHeight)
			);
			SetLinePositions();
			m_points[x][y].m_point.setScale(float(m_window->getSize().x / 800), float(m_window->getSize().y / 600));
		}
	}
}

void FourSquareScene::ResetGridData()
{
	int t_gridWidth = m_managerRef->GetGrid()->GetWidth();
	int t_gridHeight = m_managerRef->GetGrid()->GetHeight();

	for (int x = 0; x < t_gridWidth; x++)
	{
		for (int y = 0; y < t_gridHeight; y++)
		{
			m_rectGrid[index(x, y)]->setFillColor(sf::Color::Black);
			m_managerRef->GetGrid()->SetCell(CellType::NONE, sf::Vector2u{ (unsigned int)x, (unsigned int)y });
		}
	}
}

void FourSquareScene::SetPointPositions()
{
	int t_fourSquareWidth = (m_window->getSize().x) / 3.f;
	int t_fourSquareHeight = (m_window->getSize().y) / 3.f;

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			// pick a random starting position
			m_points[x][y].normalisedX = Utils::MapToRange(std::rand() % 1000, 0, 999, 0, 1);
			m_points[x][y].normalisedY = Utils::MapToRange(std::rand() % 1000, 0, 999, 0, 1);

			m_points[x][y].m_point.setPosition(
				x * t_fourSquareWidth + Utils::MapToRange(m_points[x][y].normalisedX, 0, 1, 0, t_fourSquareWidth),
				y * t_fourSquareHeight + Utils::MapToRange(m_points[x][y].normalisedY, 0, 1, 0, t_fourSquareHeight)
			);
		}
	}
}

void FourSquareScene::SetLinePositions()
{
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			// 			//check up
			// 			m_points[x][y].neighborOne.m_start = m_points[x][y].m_point.getPosition();//from
			//
			// 			if (y - 1 >= 0) // check for valid index
			// 			{
			// 				m_points[x][y].one = true;
			// 				m_points[x][y].neighborOne.m_end = m_points[x][y - 1].m_point.getPosition();//to
			// 				m_points[x][y].neighborOne.m_controlStart.x = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
			// 				m_points[x][y].neighborOne.m_controlStart.y = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
			// 				m_points[x][y].neighborOne.m_controlEnd.x = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
			// 				m_points[x][y].neighborOne.m_controlEnd.y = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
			// 			}
			// 			else
			// 			{
			// 				m_points[x][y].one = false;
			// 				m_points[x][y].neighborOne.m_end = m_points[x][y].m_point.getPosition();//to
			// 			}

						//check right

			m_points[x][y].neighborTwo.m_start = m_points[x][y].m_point.getPosition();//from
			if (x + 1 <= 2)
			{
				m_points[x][y].two = true;
				m_points[x][y].neighborTwo.m_end = m_points[x + 1][y].m_point.getPosition();//to
				m_points[x][y].neighborTwo.m_controlStart.x = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
				m_points[x][y].neighborTwo.m_controlStart.y = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
				m_points[x][y].neighborTwo.m_controlEnd.x = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
				m_points[x][y].neighborTwo.m_controlEnd.y = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
			}
			else
			{
				m_points[x][y].two = false;
				m_points[x][y].neighborTwo.m_end = m_points[x][y].m_point.getPosition();//to
			}

			//check down

			m_points[x][y].neighborThree.m_start = m_points[x][y].m_point.getPosition();//from
			if (y + 1 <= 2)
			{
				m_points[x][y].three = true;
				m_points[x][y].neighborThree.m_end = m_points[x][y + 1].m_point.getPosition();//to
				m_points[x][y].neighborThree.m_controlStart.x = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
				m_points[x][y].neighborThree.m_controlStart.y = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
				m_points[x][y].neighborThree.m_controlEnd.x = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
				m_points[x][y].neighborThree.m_controlEnd.y = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
			}
			else
			{
				m_points[x][y].three = false;
				m_points[x][y].neighborThree.m_end = m_points[x][y].m_point.getPosition();//to
			}
			//
			// 			//check left
			//
			// 			m_points[x][y].neighborFour.m_start = m_points[x][y].m_point.getPosition();//from
			// 			if (x - 1 >= 0)
			// 			{
			// 				m_points[x][y].four = true;
			// 				m_points[x][y].neighborFour.m_end = m_points[x - 1][y].m_point.getPosition();//to
			// 				m_points[x][y].neighborFour.m_controlStart.x = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
			// 				m_points[x][y].neighborFour.m_controlStart.y = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
			// 				m_points[x][y].neighborFour.m_controlEnd.x = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
			// 				m_points[x][y].neighborFour.m_controlEnd.y = Utils::MapToRange(std::rand() % 100, 0, 99, 0, 1);
			// 			}
			// 			else
			// 			{
			// 				m_points[x][y].four = false;
			// 				m_points[x][y].neighborFour.m_end = m_points[x][y].m_point.getPosition();//to
			// 			}
			m_points[x][y].neighborOne.RecalculateCurve();
			m_points[x][y].neighborTwo.RecalculateCurve();
			m_points[x][y].neighborThree.RecalculateCurve();
			m_points[x][y].neighborFour.RecalculateCurve();
		}
	}
}

void FourSquareScene::CalculateBezierHallway()
{
	int t_gridWidth = m_managerRef->GetGrid()->GetWidth();
	int t_gridHeight = m_managerRef->GetGrid()->GetHeight();

	for (int x = 0; x < t_gridWidth; x++)
	{
		for (int y = 0; y < t_gridHeight; y++)
		{
			bool pass = false;
			// init four square circles
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (m_points[i][j].one && m_points[i][j].neighborOne.ShortestDistanceToCurve(m_rectGrid[index(x, y)]->getPosition()) <= m_bezierHallDistance)
					{
						pass = true;
						// Set Cell to correct voronoi type
						m_managerRef->GetGrid()->SetCell(m_managerRef->GetGrid()->GetVoronoi(sf::Vector2u{ (unsigned int)x, (unsigned int)y }), sf::Vector2u{ (unsigned int)x, (unsigned int)y });
					}

					if (m_points[i][j].two && m_points[i][j].neighborTwo.ShortestDistanceToCurve(m_rectGrid[index(x, y)]->getPosition()) <= m_bezierHallDistance)
					{
						pass = true;
						// Set Cell to correct voronoi type
						m_managerRef->GetGrid()->SetCell(m_managerRef->GetGrid()->GetVoronoi(sf::Vector2u{ (unsigned int)x, (unsigned int)y }), sf::Vector2u{ (unsigned int)x, (unsigned int)y });
					}

					if (m_points[i][j].three && m_points[i][j].neighborThree.ShortestDistanceToCurve(m_rectGrid[index(x, y)]->getPosition()) <= m_bezierHallDistance)
					{
						pass = true;
						// Set Cell to correct voronoi type
						m_managerRef->GetGrid()->SetCell(m_managerRef->GetGrid()->GetVoronoi(sf::Vector2u{ (unsigned int)x, (unsigned int)y }), sf::Vector2u{ (unsigned int)x, (unsigned int)y });
					}

					if (m_points[i][j].four && m_points[i][j].neighborFour.ShortestDistanceToCurve(m_rectGrid[index(x, y)]->getPosition()) <= m_bezierHallDistance)
					{
						pass = true;
						// Set Cell to correct voronoi type
						m_managerRef->GetGrid()->SetCell(m_managerRef->GetGrid()->GetVoronoi(sf::Vector2u{ (unsigned int)x, (unsigned int)y }), sf::Vector2u{ (unsigned int)x, (unsigned int)y });
					}

					if (m_useStraightHallways)
					{
						if (m_points[i][j].one && Utils::DistanceToLineSegment(m_points[i][j].neighborOne.m_start, m_points[i][j].neighborOne.m_end, m_rectGrid[index(x, y)]->getPosition()) <= m_straightHallDistance)
						{
							pass = true;
							// Set Cell to correct voronoi type
							m_managerRef->GetGrid()->SetCell(m_managerRef->GetGrid()->GetVoronoi(sf::Vector2u{ (unsigned int)x, (unsigned int)y }), sf::Vector2u{ (unsigned int)x, (unsigned int)y });
						}

						if (m_points[i][j].two && Utils::DistanceToLineSegment(m_points[i][j].neighborTwo.m_start, m_points[i][j].neighborTwo.m_end, m_rectGrid[index(x, y)]->getPosition()) <= m_straightHallDistance)
						{
							pass = true;
							// Set Cell to correct voronoi type
							m_managerRef->GetGrid()->SetCell(m_managerRef->GetGrid()->GetVoronoi(sf::Vector2u{ (unsigned int)x, (unsigned int)y }), sf::Vector2u{ (unsigned int)x, (unsigned int)y });
						}

						if (m_points[i][j].three && Utils::DistanceToLineSegment(m_points[i][j].neighborThree.m_start, m_points[i][j].neighborThree.m_end, m_rectGrid[index(x, y)]->getPosition()) <= m_straightHallDistance)
						{
							pass = true;
							// Set Cell to correct voronoi type
							m_managerRef->GetGrid()->SetCell(m_managerRef->GetGrid()->GetVoronoi(sf::Vector2u{ (unsigned int)x, (unsigned int)y }), sf::Vector2u{ (unsigned int)x, (unsigned int)y });
						}

						if (m_points[i][j].four && Utils::DistanceToLineSegment(m_points[i][j].neighborFour.m_start, m_points[i][j].neighborFour.m_end, m_rectGrid[index(x, y)]->getPosition()) <= m_straightHallDistance)
						{
							pass = true;
							// Set Cell to correct voronoi type
							m_managerRef->GetGrid()->SetCell(m_managerRef->GetGrid()->GetVoronoi(sf::Vector2u{ (unsigned int)x, (unsigned int)y }), sf::Vector2u{ (unsigned int)x, (unsigned int)y });
						}
					}
				}
			}
			if (pass == false)
			{
				m_managerRef->GetGrid()->SetCell(CellType::WALL, sf::Vector2u{ (unsigned int)x, (unsigned int)y });
				// if not close enough to line, make a wall
			}
		}
	}
}

void FourSquareScene::UpdateColors()
{
	int t_gridWidth = m_managerRef->GetGrid()->GetWidth();
	int t_gridHeight = m_managerRef->GetGrid()->GetHeight();

	for (int x = 0; x < t_gridWidth; x++)
	{
		for (int y = 0; y < t_gridHeight; y++)
		{
			switch (m_managerRef->GetGrid()->GetCell(sf::Vector2u{ (unsigned int)x,(unsigned int)y }))
			{
			case CellType::NONE:
				m_rectGrid[index(x, y)]->setFillColor(sf::Color::Green);
				break;
			case CellType::WALKABLE:
				m_rectGrid[index(x, y)]->setFillColor(sf::Color::Cyan);
				break;
			case CellType::WALL:
				m_rectGrid[index(x, y)]->setFillColor(sf::Color::Black);
				break;
			case CellType::BUILDING:
				m_rectGrid[index(x, y)]->setFillColor(sf::Color::Magenta);
				break;
			default:
				m_rectGrid[index(x, y)]->setFillColor(sf::Color::White);
				break;
			}
		}
	}
}