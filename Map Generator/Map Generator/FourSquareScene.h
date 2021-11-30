#pragma once
#include "Scene.h"
#include <string>

struct FourSquarePoint
{
public:
	sf::CircleShape m_point;
	float normalisedX;
	float normalisedY;

	bool one = false;
	bool two = false;
	bool three = false;
	bool four = false;

	AELine neighborOne;

	AELine neighborTwo;

	AELine neighborThree;

	AELine neighborFour;
};

class FourSquareScene : public Scene
{
public:
	FourSquareScene(sf::RenderWindow* in_window, SceneManager* in_managerRef);
	void Update(float frame_time) override;
	void Render() override;
	void Cleanup() override;

	// events
	void ResizeEvent() override;
private:

	void ResetGridData();
	void SetPointPositions();
	void SetLinePositions();
	void CalculateBezierHallway();
	void UpdateColors();

	int m_hallResolution = 10;
	float m_bezierHallDistance = 10;
	float m_straightHallDistance = 10;

	std::vector<sf::RectangleShape*> m_rectGrid;

	std::string buffer;
	std::string_view view{ buffer };

	FourSquarePoint m_points[3][3];

	bool m_useStraightHallways = true;

	/*	AELine testLine;*/
};
