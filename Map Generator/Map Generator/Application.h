#pragma once
#include <iostream>
#include <vector>
#include "SceneManager.h"
#include "imgui/imgui.h"
#include "maths.h"
#include "imgui-sfml/imgui-sfml.h"

enum class CellType
{
	NONE = 0,
	WALL = 1,
	WALKABLE = 2,
	BUILDING = 3
};

class AELine
{
public:

	AELine()
	{
		RecalculateCurve();
	}

	AELine(sf::RenderWindow* in_window)
	{
		SetWindow(in_window);

		if (m_window != nullptr)
		{
			m_start = { 0, (float)m_window->getSize().y };
			m_end = { (float)m_window->getSize().x, 0 };
		}

		RecalculateCurve();
	}

	AELine(sf::Vector2f in_start, sf::Vector2f in_end, sf::RenderWindow* in_window = nullptr)
	{
		SetWindow(in_window);
		m_start = in_start;
		m_end = in_end;

		RecalculateCurve();
	}

	AELine(sf::Vector2f in_start, sf::Vector2f in_end, sf::Vector2f in_contorlStart, sf::Vector2f in_controlEnd, sf::RenderWindow* in_window = nullptr)
	{
		SetWindow(in_window);
		m_start = in_start;
		m_end = in_end;
		m_controlEnd = in_contorlStart;
		m_controlStart = in_controlEnd;

		RecalculateCurve();
	}

	void SetWindow(sf::RenderWindow* in_window)
	{
		if (in_window == nullptr)
			return;
		m_window = in_window;
	}

	void RecalculateCurve()
	{
		std::vector<sf::Vector2f> points =
			Utils::CalcCubicBezier(
				m_start,
				m_end,
				sf::Vector2f(Utils::MapToRange(m_controlStart.x, 1.f, 0.f, m_start.x, m_end.x), Utils::MapToRange(m_controlStart.y, 1, 0, m_start.y, m_end.y)),
				sf::Vector2f(Utils::MapToRange(m_controlEnd.x, 1.f, 0.f, m_start.x, m_end.x), Utils::MapToRange(m_controlEnd.y, 1, 0, m_start.y, m_end.y)),
				resolution
			);

		line.clear();

		for (std::vector<sf::Vector2f>::const_iterator a = points.begin(); a != points.end(); ++a)
			line.append(sf::Vertex(*a, sf::Color::White));
	}

	void Draw()
	{
		m_window->draw(line);
	}

	void Draw(sf::RenderWindow* in_window)
	{
		in_window->draw(line);
	}

	float ShortestDistanceToCurve(sf::Vector2f in_point)
	{
		float shortest = INTMAX_MAX;
		for (int i = 0; i < line.getVertexCount(); i++)
		{
			if (shortest > Utils::Distance(in_point, line[i].position))
			{
				shortest = Utils::Distance(in_point, line[i].position);
			}
		}
		return shortest;
	}

	void ChangeHallResolution(int in_value)
	{
		resolution = in_value;
	}

	sf::VertexArray line{ sf::LinesStrip, 0 };

	sf::Vector2f m_start = { 0,0 };
	sf::Vector2f m_end = { 0,0 };
	sf::Vector2f m_controlStart = { 0,0 };
	sf::Vector2f m_controlEnd = { 1,1 };

	int resolution = 30;

private:
	sf::RenderWindow* m_window;
};

class GridData
{
public:
	GridData(uint8_t in_width, uint8_t in_height)
	{
		m_width = in_width;
		m_height = in_height;

		for (int x = 0; x < m_width; x++)
		{
			for (int y = 0; y < m_height; y++)
			{
				Cells.push_back(CellType::NONE);
			}
		}

		for (int x = 0; x < m_width; x++)
		{
			for (int y = 0; y < m_height; y++)
			{
				VoronoiTypes.push_back(CellType::NONE);
			}
		}
	} // Init cell vector

	int GetSize() { return m_width * m_height; }
	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }
	inline void SetCell(CellType in_type, sf::Vector2u in_pos) { Cells[m_width * in_pos.y + in_pos.x] = in_type; }
	inline void SetVoronoi(CellType in_type, sf::Vector2u in_pos) { VoronoiTypes[m_width * in_pos.y + in_pos.x] = in_type; }
	inline CellType GetCell(sf::Vector2u in_pos) { return Cells[m_width * in_pos.y + in_pos.x]; }
	inline CellType GetCell(int in_pos) { return Cells[in_pos]; }
	inline CellType GetVoronoi(sf::Vector2u in_pos) { return VoronoiTypes[m_width * in_pos.y + in_pos.x]; }

private:
	uint8_t m_width = 0;
	uint8_t m_height = 0;

	std::vector<CellType> Cells;
	std::vector<CellType> VoronoiTypes;
};

class Application
{
public:
	Application() = default;
	int Start();

private:
	void RenderFPS();
	void cleanup();

	SceneManager* m_sceneManager;

	GridData m_gridData{ 120, 120 }; // make mul of 3

	ImGuiWindowFlags m_FPSWindowFlags;

	// SFML BOILERPLATE-------------
	sf::RenderWindow m_window;

	// SFML event handler
	sf::Event m_event;

	// For delta time calculations
	sf::Clock m_clock;
	sf::Time m_elapsedTime;
	// SFML BOILERPLATE-------------
};
