#include "dockspace_layout.h"

void DockspaceLayout::draw(const std::vector<std::unique_ptr<Panel>> &panels)
{
    // The menu bar goes first: it shrinks the viewport's work area, which the
    // dockspace then fills.
    drawMenuBar(panels);
    ImGui::DockSpaceOverViewport(0, nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
}

void DockspaceLayout::drawMenuBar(const std::vector<std::unique_ptr<Panel>> &panels)
{
    if (!ImGui::BeginMainMenuBar())
        return;

    if (ImGui::BeginMenu("View")) {
        for (auto &panel : panels) {
            bool open = panel->isOpen();
            if (ImGui::MenuItem(panel->name().c_str(), nullptr, &open))
                panel->setOpen(open);
        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}
