#include "editor_application.h"
#include "panels/stats_panel.h"

namespace {
const glm::vec3 LIGHT_DIRECTION{0.3f, 0.6f, 0.4f};
}

EditorApplication::EditorApplication()
    : m_graphics(&m_window)
    , m_context(m_world)
{
    m_shadowPass = std::make_unique<ShadowPass>(&m_graphics);
    m_forwardPass = std::make_unique<ForwardPass>(&m_graphics);

    m_shadowPass->attachModels(&m_models);
    m_forwardPass->attachModels(&m_models);
    m_forwardPass->setShadowMap(m_shadowPass->getShadowMap());

    m_shadowPass->setLightDirection(LIGHT_DIRECTION);
    m_forwardPass->setLightDirection(LIGHT_DIRECTION);
    m_forwardPass->setLightVPMatrix(m_shadowPass->getLightVPMatrix());

    // Created last: ImGui chains onto GLFW callbacks that exist at this point.
    m_guiPass = std::make_unique<GuiPass>(&m_graphics);
    m_guiPass->insertUi([this] { drawUi(); });

    m_graphicsPasses = {m_shadowPass.get(), m_forwardPass.get(), m_guiPass.get()};

    m_panels.push_back(std::make_unique<StatsPanel>());
}

void EditorApplication::run()
{
    m_graphics.beginRenderLoopSingle(m_graphicsPasses,
                                     m_computePasses,
                                     [this](double time, double deltaTime) {
                                         update(time, deltaTime);
                                     });
}

void EditorApplication::update(double /*time*/, double /*deltaTime*/) {}

void EditorApplication::drawUi()
{
    m_dockspaceLayout.draw(m_panels);

    for (auto &panel : m_panels)
        panel->draw(m_context);
}
