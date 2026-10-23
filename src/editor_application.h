#ifndef EDITOR_APPLICATION_H
#define EDITOR_APPLICATION_H

#include "editor_context.h"
#include "panels/panel.h"
#include <ecs/ecs.h>
#include <graphics.h>
#include <memory>
#include <passes/forward_pass.h>
#include <passes/gui_pass.h>
#include <passes/shadow_pass.h>
#include <vector>
#include <window.h>

// Composition root of the editor: owns the engine objects and the editor
// state, wires them together and drives the frame loop. It is the only
// editor class that knows about Graphics and render passes.
class EditorApplication
{
public:
    EditorApplication();

    // Callbacks registered with the engine capture `this`, so the
    // application must never be copied or moved.
    EditorApplication(const EditorApplication &) = delete;
    EditorApplication &operator=(const EditorApplication &) = delete;

    void run();

private:
    void update(double time, double deltaTime);

    // Builds the whole editor UI for one frame. Invoked by GuiPass between
    // ImGui::NewFrame and ImGui::Render (registered via insertUi).
    void drawUi();

    // Declaration order is construction order: Graphics needs the Window,
    // passes need Graphics, and the GuiPass must come after every Window
    // callback is registered (see MEMORY.md, trap T1).
    Window m_window;
    Graphics m_graphics;

    World m_world;
    std::vector<NativeModel> m_models;

    std::unique_ptr<ShadowPass> m_shadowPass;
    std::unique_ptr<ForwardPass> m_forwardPass;
    std::unique_ptr<GuiPass> m_guiPass;

    // Non-owning views in execution order, as required by the render loop.
    std::vector<Pass *> m_graphicsPasses;
    std::vector<Pass *> m_computePasses;

    EditorContext m_context;
    std::vector<std::unique_ptr<Panel>> m_panels;
};

#endif // EDITOR_APPLICATION_H
