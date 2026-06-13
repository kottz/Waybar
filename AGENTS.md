# AGENTS.md

This fork carries kottz's Sway workspace-groups Waybar support on
`workspace-groups`.

## Remotes

- `origin`: `git@github.com:kottz/Waybar.git`
- `upstream`: `https://github.com/Alexays/Waybar.git`

When maintaining this fork, fetch upstream tags before deciding what to rebase
onto.

## Feature Contract

- Existing `sway/workspaces` behavior remains unchanged unless grouped options
  are enabled.
- With grouped options enabled, workspace buttons can show Sway IPC
  `display_name` values instead of internal names like `work:1`.
- `sway/workspaces` can filter to the active group only.
- `sway/workspace-group` renders a compact W/P-style indicator, supports
  click-to-toggle, avoids cursor changes, and marks inactive-group urgency.
- Fast clicks on the W/P indicator should count physical clicks, not synthetic
  GTK double-click events.

## Important Files

- `src/modules/sway/workspaces.cpp`
- `include/modules/sway/workspaces.hpp`
- `src/modules/sway/workspace_group.cpp`
- `include/modules/sway/workspace_group.hpp`
- `src/factory.cpp`
- `man/waybar-sway-workspaces.5.scd`
- `man/waybar-sway-workspace-group.5.scd`

## Verification

```bash
clang-format -i include/modules/sway/workspace_group.hpp include/modules/sway/workspaces.hpp src/factory.cpp src/modules/sway/workspace_group.cpp src/modules/sway/workspaces.cpp
meson compile -C build
meson test -C build
git diff --check
```

Follow Waybar's repo style: Google C++ style via `.clang-format`.
