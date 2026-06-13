---
name: waybar-workspace-groups-maintainer
description: Maintain kottz's Waybar workspace-groups fork, especially rebasing the feature branch, preserving sway/workspaces compatibility, maintaining the compact sway/workspace-group indicator, and validating with Waybar formatting/build checks.
---

# Waybar Workspace Groups Maintainer

Use this skill for Waybar source work in the `kottz/Waybar` fork.

## Workflow

1. Read `AGENTS.md` and `README.md`.
2. Confirm remotes:
   - `origin`: `git@github.com:kottz/Waybar.git`
   - `upstream`: `https://github.com/Alexays/Waybar.git`
3. Fetch upstream tags before rebasing.
4. Rebase `workspace-groups` onto the selected upstream Waybar tag or branch.
5. Resolve conflicts by preserving the feature contract in `AGENTS.md`.
6. Run formatting on touched C++ files:

```bash
clang-format -i include/modules/sway/workspace_group.hpp include/modules/sway/workspaces.hpp src/factory.cpp src/modules/sway/workspace_group.cpp src/modules/sway/workspaces.cpp
```

7. Run:

```bash
meson compile -C build
meson test -C build
git diff --check
```

## Conflict Hotspots

- `sway/workspaces` IPC parsing and format replacement.
- `sway/workspace-group` click handling and urgent class updates.
- Factory module registration.
- Man page module/options lists.

## Invariants

- Default Waybar behavior stays upstream-compatible.
- No internal grouped workspace names are shown when `{display_name}` is used.
- The W/P indicator remains compact and styleable.
- Physical fast clicks toggle once per physical click.
- Inactive-group urgency is visible through the indicator's `urgent` class.
