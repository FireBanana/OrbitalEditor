#ifndef EDITOR_CONTEXT_H
#define EDITOR_CONTEXT_H

#include "selection.h"
#include <ecs/ecs.h>

// State shared by every panel. Panels never reference each other; they
// communicate only by reading and writing this context.
struct EditorContext
{
    explicit EditorContext(World &world)
        : world(world)
    {}

    World &world;
    Selection selection;
};

#endif // EDITOR_CONTEXT_H
