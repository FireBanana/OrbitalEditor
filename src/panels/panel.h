#ifndef EDITOR_PANEL_H
#define EDITOR_PANEL_H

#include "../editor_context.h"
#include <imgui.h>
#include <string>

// Base class for every dockable editor window.
//
// draw() owns the ImGui::Begin/End pair so derived panels only describe
// their contents. ImGui requires End() even when Begin() returns false
// (window collapsed or clipped), which is easy to get wrong per-panel.
class Panel
{
public:
    explicit Panel(std::string name)
        : m_name(std::move(name))
    {}
    virtual ~Panel() = default;

    Panel(const Panel &) = delete;
    Panel &operator=(const Panel &) = delete;

    // Must be called between ImGui::NewFrame() and ImGui::Render().
    void draw(EditorContext &context)
    {
        if (!m_open)
            return;

        if (ImGui::Begin(m_name.c_str(), &m_open))
            drawContents(context);
        ImGui::End();
    }

    const std::string &name() const { return m_name; }
    bool isOpen() const { return m_open; }
    void setOpen(bool open) { m_open = open; }

protected:
    virtual void drawContents(EditorContext &context) = 0;

private:
    std::string m_name;
    bool m_open = true;
};

#endif // EDITOR_PANEL_H
