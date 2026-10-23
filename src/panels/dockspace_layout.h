#ifndef EDITOR_DOCKSPACE_LAYOUT_H
#define EDITOR_DOCKSPACE_LAYOUT_H

#include "panel.h"
#include <memory>
#include <vector>

// The editor's window frame: the main menu bar and a dockspace covering the
// rest of the window. The dockspace's central node is see-through, so the
// scene rendered by the engine passes shows behind it (see MEMORY.md,
// "Viewport decision").
//
// It is not a Panel: it has no window of its own and must be drawn before
// any panel so the panels can dock into it.
class DockspaceLayout
{
public:
    // Must be called between ImGui::NewFrame() and ImGui::Render(), before
    // the panels are drawn. The View menu toggles each panel's open state.
    void draw(const std::vector<std::unique_ptr<Panel>> &panels);

private:
    void drawMenuBar(const std::vector<std::unique_ptr<Panel>> &panels);
};

#endif // EDITOR_DOCKSPACE_LAYOUT_H
