#include "Application.h"

int Application::Start()
{
	m_window.create(sf::VideoMode(600, 600), "Map Generator");

	ImGui::SFML::Init(m_window);

	m_sceneManager = new SceneManager(&m_window, &m_gridData);

	m_FPSWindowFlags |= ImGuiWindowFlags_NoNav;
	m_FPSWindowFlags |= ImGuiWindowFlags_NoDecoration;

	while (m_window.isOpen())
	{
		m_elapsedTime = m_clock.restart();
		while (m_window.pollEvent(m_event))
		{
			ImGui::SFML::ProcessEvent(m_event);
			switch (m_event.type)
			{
			case sf::Event::Closed:
				m_window.close();
				break;
			case sf::Event::Resized:
				m_window.setView(sf::View(sf::FloatRect(0, 0, m_event.size.width, m_event.size.height)));
				m_window.setSize(sf::Vector2u(m_event.size.width, m_event.size.height));
				m_sceneManager->ResizeEvent();
				break;
			case sf::Event::KeyPressed:
				if (m_event.key.code == sf::Keyboard::Escape)
				{
					m_window.close();
				}
				break;
			default:
				break;
			}
		}
		ImGui::SFML::Update(m_window, m_elapsedTime);
		m_sceneManager->Update(m_elapsedTime.asMilliseconds());
		RenderFPS();
		m_sceneManager->Render();
	}

	cleanup();

	return 0;
}

void Application::RenderFPS()
{
	ImGui::Begin("FPSWindow", 0, m_FPSWindowFlags);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::SetWindowPos(ImVec2(0, m_window.getSize().y - ImGui::GetWindowSize().y));
	ImGui::End();
}

void Application::cleanup()
{
	m_sceneManager->Cleanup();
	ImGui::SFML::Shutdown();
}