#pragma once
#include "Scene.h"
#include "Maths.h"
#include "Application.h"

class AEVoronoiPoint
{
public:
	AEVoronoiPoint()
	{
		m_regionColor.r = static_cast<sf::Uint8>(Utils::MapToRange(std::rand() % 1000, 0, 999, 64, 255));
		m_regionColor.g = static_cast<sf::Uint8>(Utils::MapToRange(std::rand() % 1000, 0, 999, 64, 255));
		m_regionColor.b = static_cast<sf::Uint8>(Utils::MapToRange(std::rand() % 1000, 0, 999, 64, 255));
		m_regionColor.a = static_cast<sf::Uint8>(255);

		m_point.setFillColor(m_regionColor);

		if (rand() % 2)
		{
			type = CellType::BUILDING;
			m_regionColor = sf::Color::Black;
		}
		else
		{
			type = CellType::WALKABLE;
		}
	}
	sf::CircleShape m_point;
	sf::Color m_regionColor;
	CellType type;
};

class VoronoiScene : public Scene
{
public:
	VoronoiScene(sf::RenderWindow* in_window, SceneManager* in_managerRef);
	void Update(float frame_time) override;
	void Render() override;
	void Cleanup() override;

	// events
	void ResizeEvent() override;
private:
	void InitGrid();
	void InitWalls();
	void InitVoronoiPoints();
	void RegenPoints();
	void CalculateRegions(bool manhattan);

	int m_numberOfPoints = 10;
	bool m_useManhattan = true;
	std::vector<AEVoronoiPoint*> m_voronoiPoints;

	AELine* m_walls[4];

	std::vector<sf::RectangleShape*> m_rectGrid;
};
