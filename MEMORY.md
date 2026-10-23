# Orbital Editor — Design Memory

Reference for the editor's architecture, the rules it follows, and the engine
features it is waiting on. Update this file whenever a milestone or gap changes.

## Ground rules
- The editor is its own repo (`~/Projects/OrbitalEditor`, github.com/FireBanana/OrbitalEditor). Nothing in the engine repo is changed by the editor work.
- The engine is built from a local checkout at `../Orbital` (`add_subdirectory` into `build/orbital`; configure fails if it's missing). Since 2026-10-23 there is no remote fetch or pinned commit, so the editor always builds against whatever is in the engine working tree.
- The editor uses the engine's **public API only**. No workarounds and no touching `Global::`.
  If something is missing, it becomes a **gap** (below), work pauses, and the engine owner implements it.
- One responsibility per class. Panels depend only on `EditorContext`.
- Code style matches the engine: include guards, `m_` members, camelCase, 4-space indent.
- Language standard is **C++20** (set in the editor's CMake; fastgltf also requires it, even though the engine sets 17).

## Core idea
1. The editor is an engine *application*, like `examples/fps`: Window + Graphics + passes + World + render loop.
2. A UI layer of **panels** sits on top and edits **shared state** (`EditorContext`).
3. Panels never reference each other; they communicate only through `EditorContext`.
4. Editor-only behaviour (editor camera, gizmos, undo) lives in the editor and drives the engine via its API.

## Viewport decision
POC uses a **passthrough dockspace**: the scene renders to the full window as it does today,
and panels dock around a see-through central node (`DockSpaceOverViewport` +
`ImGuiDockNodeFlags_PassthruCentralNode`). An offscreen viewport texture comes later (gap G8).

## Layout
```
OrbitalEditor/
  CMakeLists.txt                 top-level project: adds ../Orbital, OrbitalEditor executable
  MEMORY.md                      this file
  src/
    main.cpp                     builds EditorApplication, calls run()
    editor_application.h/.cpp    composition root; only class aware of Graphics/passes
    editor_context.h             shared state (World&, Selection; later CommandHistory)
    selection.h                  currently selected Entity
    panels/panel.h               Panel base: draw() owns Begin/End, derived implement drawContents()
    panels/dockspace_layout.*    main menu bar (View toggles panels) + passthrough dockspace; not a Panel
    panels/stats_panel.*         FPS / frame time (entity count after G3)
  -- planned --
    panels/scene_hierarchy_panel.*  list/select/create/delete entities      (M1)
    panels/inspector_panel.*     edit Name/Transform of selection           (M1)
    camera/editor_camera.*       fly camera when central node hovered       (M2)
    panels/asset_browser_panel.* browse assets/*.glb and import             (M3)
    commands/command.h, command_history.*, transform_command.h  undo/redo   (M4)
```

## Milestones
| ID | Scope | Needs | Status |
|----|-------|-------|--------|
| M0 | Scaffolding: CMake target, EditorApplication, EditorContext, Selection, Panel base; `drawUi()` registered via `GuiPass::insertUi` | G0, G1 | **Done**. Builds with no editor warnings; runs (window, render loop, UI hook) |
| M1 | Dockspace, menu bar, Hierarchy, Inspector, Stats | G1, G2, G3 | **Partial** (2026-10-23): dockspace, menu bar, Stats (frame time) done; builds and runs, layout not yet checked visually. Hierarchy, Inspector, entity count wait on G3 |
| M2 | Meshes as entities, Transform edits move meshes, editor fly camera | G4, G5, G6 | Waiting |
| M3 | Asset browser + import | G7 (for delete) | Waiting |
| M4 | Undo/redo, ImGuizmo gizmos (fetched in editor/CMakeLists.txt) | G5 matrices | Waiting |
| M5 | Offscreen viewport, picking, save/load | G8, G9, G10 | Later |

## Engine gaps (owner: engine; status: Open / Done)
| # | Status | Gap | Where | Why the editor needs it |
|---|--------|-----|-------|---------------------------|
| G0 | Superseded | `add_subdirectory(editor)` in root `CMakeLists.txt` | root CMake | Replaced by the separate repo; see G14 |
| G1 | Done | `GuiPass::insertUi(std::function<void()>)` stores callbacks and calls them between `NewFrame` and `Render` | `src/passes/gui_pass.*` | The editor registers `drawUi()` through it |
| G2 | Done | ImGui from the `docking` branch; `ImGuiConfigFlags_DockingEnable` set in GuiPass | root CMake, `gui_pass.cpp` | Dockspace layout. Done 2026-10-23, but `GIT_TAG docking` is a moving branch, not a pinned commit (see T7) |
| G3 | Open | ECS stub: `World` only creates IDs. No component storage, destroy or iteration; `Transform` has no scale/matrix | `src/ecs/ecs.h` | Hierarchy + Inspector need add/get/has/remove, iteration, destroy, a Name component |
| G4 | Open | Rendering is not tied to the ECS: passes draw `std::vector<NativeModel>*` | passes + ecs | Inspector Transform edits must move meshes (e.g. MeshRenderer component read by the passes) |
| G5 | Open | Camera lives inside `ForwardPass` (no getters, aspect hard-coded to 800/600) | `forward_pass.*` | Editor camera object and view/projection matrices (also for gizmos) |
| G6 | Open | Input: one callback per event (re-register replaces); filters on `WantCapture*`; order-dependent with ImGui; cursor mode only via `Global::g_window` | `window.*` | Editor camera + UI sharing input; polling (`isKeyDown`, mouse delta) or multi-listener; `setCursorMode` |
| G7 | Open | No destroy for `NativeModel`/`Texture`/`Buffer` | `graphics.*` | Deleting entities / re-import without leaks |
| G8 | Later | Offscreen viewport: targets lack `SAMPLED_BIT`, render area fixed to swapchain, custom targets not resized, no ImGui texture registration | graphics/pass | Real Viewport panel |
| G9 | Later | Picking: no ID buffer/readback | passes | Click-to-select in the scene |
| G10 | Later | No scene serialization format | engine | Save/load |
| G11 | Open (minor) | `Pass` has no virtual destructor; no GPU cleanup or `vkDeviceWaitIdle` at shutdown | `pass.h`, `graphics.*` | Editor owns passes through `unique_ptr<ConcreteType>` for now (correct today), but polymorphic ownership would be UB |
| G12 | Done | `ROOT="${CMAKE_SOURCE_DIR}/"` must become `${PROJECT_SOURCE_DIR}/` | root CMake | Fixed in the local engine 2026-10-23; the editor now finds the engine's shaders and runs |
| G13 | Open (build time only) | `add_subdirectory(examples)` should only run when Orbital is top level (`if(PROJECT_IS_TOP_LEVEL)` or an option) | root CMake | Otherwise every editor build also compiles all examples |
| G14 | Done | Remove `add_subdirectory(editor)` | root CMake | Gone from engine `main` as of 2026-10-23 |
| G15 | Done | Push `7f0229c` ("add gui layers", contains G1) to `origin/main` | engine repo | On `main` as of 2026-10-23 (followed by `a78f46a`) |
| G16 | Open (minor) | No way to close the window from code (`Window` has no `close()`/`requestClose()`) | `window.*` | File › Exit in the menu bar; left out until this exists |

## Traps
- **T1 — ImGui callback order:** `ImGui_ImplGlfw_InitForVulkan(..., true)` chains onto whatever GLFW callbacks exist at that moment. Every `Window::register*` call must happen **before** GuiPass is constructed. `EditorApplication` creates GuiPass last for this reason.
- **T2 — Frames in flight:** passes hold `std::vector<NativeModel>*`. Adding models is fine; never destroy one that a frame in flight (`FRAMES_IN_FLIGHT = 2`) may still use.
- **T3 — Upload stalls:** `makeImage`/`makeBuffer` wait on a staging fence, so importing mid-loop works but stalls the frame. That's fine for the POC.
- **T4 — Asset root:** `ROOT` points to the engine's source tree (once G12 is done); the editor uses `ROOT "assets/"`, so assets are the engine's `assets/`.
- **T5 — `Window` constructor:** it uses `throw;` with no active exception, so a failed window creation calls `std::terminate`.
- **T7 — Unpinned dependencies:** root CMake fetches fastgltf/imgui/glfw/volk/vma/box3d from moving branches. Any CMake re-configure can pull new upstream code. On 2026-10-12 a reconfigure pulled fastgltf `7e87cbf`, which broke the whole engine build (`simdjson.h: No such file or directory`). Pin `GIT_TAG`s to tags or commits. fastgltf is now pinned to `v0.9.1` (fixed); the others are still unpinned.
- **T8 — UI runs last:** GuiPass is the last pass, so UI edits made in `drawUi()` reach the scene passes one frame late.
- **T6 — ImGui Begin/End:** `End()` must run even when `Begin()` returns false. `Panel::draw()` centralises this.

## Next step
M1 partial: dockspace, menu bar and Stats panel are in. The rest of M1 (Hierarchy, Inspector, entity count) needs **G3** (ECS).
Optional engine items: pin ImGui `docking` to a commit (T7), G13 (examples build with the editor), G16 (window close).
