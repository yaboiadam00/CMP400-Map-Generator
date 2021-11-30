#include "TestScene.h"

TestScene::TestScene(sf::RenderWindow* in_window, SceneManager* in_managerRef) : 
    Scene(in_window, in_managerRef)
{
}

void TestScene::Update(float frame_time)
{
}

void TestScene::Render()
{
	
    ImGui::Begin("Sample window",0, m_windowFlags); // begin window
    ImGui::SetWindowSize("Sample window", m_window->getSize());
    ImGui::SetWindowPos("Sample window", ImVec2(0, 0));

    if (ImGui::Button("Back"))
    {
        m_managerRef->SwitchScene(SceneName::MAIN_MENU);
    }

    // Background color edit
    if (ImGui::ColorEdit3("Background color", color)) {
        // this code gets called if color value changes, so
        // the background color is upgraded automatically!
        bgColor.r = static_cast<sf::Uint8>(color[0] * 255.f);
        bgColor.g = static_cast<sf::Uint8>(color[1] * 255.f);
        bgColor.b = static_cast<sf::Uint8>(color[2] * 255.f);
    }

    // Window title text edit
    if(ImGui::InputText("Window title", windowTitle, 255))
    {
        m_window->setTitle(windowTitle);
    }

    ImGui::End(); // end window
    m_window->clear(bgColor);
    //window.draw(shape);
	ImGui::SFML::Render(*m_window);
	m_window->display();
}

void TestScene::Cleanup()
{

}

void TestScene::ResizeEvent()
{

}
