#ifndef EDITOR_STATS_PANEL_H
#define EDITOR_STATS_PANEL_H

#include "panel.h"

// Frame timing. Entity count is added once the ECS can be iterated (gap G3).
class StatsPanel : public Panel
{
public:
    StatsPanel()
        : Panel("Stats")
    {}

protected:
    void drawContents(EditorContext &context) override;
};

#endif // EDITOR_STATS_PANEL_H
