#ifndef EDITOR_SELECTION_H
#define EDITOR_SELECTION_H

#include <ecs/ecs.h>
#include <optional>

// The entity the user is currently working on. Panels read it, and only
// user actions (clicking in the hierarchy, picking, etc.) change it.
class Selection
{
public:
    void select(Entity entity) { m_selected = entity; }
    void clear() { m_selected.reset(); }

    std::optional<Entity> selected() const { return m_selected; }
    bool isSelected(Entity entity) const { return m_selected == entity; }

private:
    std::optional<Entity> m_selected;
};

#endif // EDITOR_SELECTION_H
