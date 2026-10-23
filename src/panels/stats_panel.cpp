#include "stats_panel.h"

void StatsPanel::drawContents(EditorContext & /*context*/)
{
    // ImGui averages the frame rate over its last 60 frames, which keeps the
    // numbers readable instead of flickering every frame.
    const float framerate = ImGui::GetIO().Framerate;

    ImGui::Text("FPS: %.1f", framerate);
    ImGui::Text("Frame time: %.2f ms", framerate > 0.0f ? 1000.0f / framerate : 0.0f);
}
