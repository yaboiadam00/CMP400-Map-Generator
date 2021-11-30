#include "Scene.h"
Scene::Scene(sf::RenderWindow* in_window, SceneManager* in_managerRef)
{
	m_window = in_window;
	m_managerRef = in_managerRef;

	// generic window flags
	m_windowFlags |= ImGuiWindowFlags_NoTitleBar;
	m_windowFlags |= ImGuiWindowFlags_NoBackground;
	m_windowFlags |= ImGuiWindowFlags_NoMove;
	m_windowFlags |= ImGuiWindowFlags_NoCollapse;

	m_backgroundColor.r = static_cast<sf::Uint8>(221);
	m_backgroundColor.g = static_cast<sf::Uint8>(153);
	m_backgroundColor.b = static_cast<sf::Uint8>(204);
	m_backgroundColor.a = static_cast<sf::Uint8>(255);

	m_accentColor1.r = static_cast<sf::Uint8>(242);
	m_accentColor1.g = static_cast<sf::Uint8>(131);
	m_accentColor1.b = static_cast<sf::Uint8>(182);
	m_accentColor1.a = static_cast<sf::Uint8>(255);

	m_accentColor2.r = static_cast<sf::Uint8>(224);
	m_accentColor2.g = static_cast<sf::Uint8>(108);
	m_accentColor2.b = static_cast<sf::Uint8>(159);
	m_accentColor2.a = static_cast<sf::Uint8>(255);
}

void Scene::Update(float frame_time)
{
}

void Scene::Render()
{
}

void Scene::Cleanup()
{
}

void Scene::ResizeEvent()
{
}

int Scene::index(int x, int y)
{
	return (m_managerRef->GetGrid()->GetWidth() * x) + y;
}