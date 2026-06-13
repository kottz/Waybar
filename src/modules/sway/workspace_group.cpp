#include "modules/sway/workspace_group.hpp"

#include <spdlog/spdlog.h>

#include <algorithm>
#include <cctype>

namespace waybar::modules::sway {

WorkspaceGroup::WorkspaceGroup(const std::string& id, const Json::Value& config)
    : ALabel(config, "workspace-group", id, "{short_name}", 0, true, false) {
  ipc_.subscribe(R"(["workspace"])");
  ipc_.subscribe(R"(["window"])");
  ipc_.signal_event.connect(sigc::mem_fun(*this, &WorkspaceGroup::onEvent));
  ipc_.signal_cmd.connect(sigc::mem_fun(*this, &WorkspaceGroup::onCmd));
  ipc_.sendCmd(IPC_GET_WORKSPACES);
  ipc_.setWorker([this] {
    try {
      ipc_.handleEvent();
    } catch (const std::exception& e) {
      spdlog::error("WorkspaceGroup: {}", e.what());
    }
  });
}

void WorkspaceGroup::onEvent(const struct Ipc::ipc_response& /*res*/) {
  try {
    ipc_.sendCmd(IPC_GET_WORKSPACES);
  } catch (const std::exception& e) {
    spdlog::error("WorkspaceGroup: {}", e.what());
  }
}

void WorkspaceGroup::onCmd(const struct Ipc::ipc_response& res) {
  if (res.type != IPC_GET_WORKSPACES) {
    return;
  }

  try {
    std::lock_guard<std::mutex> lock(mutex_);
    auto payload = parser_.parse(res.payload);
    active_group_.clear();
    inactive_group_urgent_ = false;
    for (const auto& workspace : payload) {
      if (workspace["active_group"].isString()) {
        active_group_ = workspace["active_group"].asString();
        break;
      }
    }
    if (active_group_.empty()) {
      for (const auto& workspace : payload) {
        if (workspace["focused"].asBool() && workspace["group"].isString()) {
          active_group_ = workspace["group"].asString();
          break;
        }
      }
    }
    if (!active_group_.empty()) {
      for (const auto& workspace : payload) {
        if (!workspace["group"].isString() || !workspace["urgent"].asBool()) {
          continue;
        }
        const auto group = workspace["group"].asString();
        if (!group.empty() && group != active_group_) {
          inactive_group_urgent_ = true;
          break;
        }
      }
    }
    dp.emit();
  } catch (const std::exception& e) {
    spdlog::error("WorkspaceGroup: {}", e.what());
  }
}

auto WorkspaceGroup::update() -> void {
  std::lock_guard<std::mutex> lock(mutex_);
  if (active_group_.empty()) {
    event_box_.hide();
    label_.get_style_context()->remove_class("urgent");
    event_box_.get_style_context()->remove_class("urgent");
    ALabel::update();
    return;
  }

  const auto short_name = shortName(active_group_);
  auto output = fmt::format(fmt::runtime(format_), fmt::arg("group", active_group_),
                            fmt::arg("name", active_group_), fmt::arg("short_name", short_name));
  if (!config_["disable-markup"].asBool()) {
    label_.set_markup(output);
  } else {
    label_.set_text(output);
  }
  if (tooltipEnabled()) {
    label_.set_tooltip_text(active_group_);
  }
  if (inactive_group_urgent_) {
    label_.get_style_context()->add_class("urgent");
    event_box_.get_style_context()->add_class("urgent");
  } else {
    label_.get_style_context()->remove_class("urgent");
    event_box_.get_style_context()->remove_class("urgent");
  }
  event_box_.show();
  ALabel::update();
}

bool WorkspaceGroup::handleToggle(GdkEventButton* const& e) {
  if (e->button != 1) {
    return ALabel::handleToggle(e);
  }
  if (config_["on-click"].isString() && config_["on-click"].asString() != "toggle") {
    return ALabel::handleToggle(e);
  }
  if (e->type != GDK_BUTTON_PRESS) {
    return true;
  }

  try {
    command_ipc_.sendCmd(IPC_COMMAND, "workspace group toggle");
  } catch (const std::exception& ex) {
    spdlog::error("WorkspaceGroup: {}", ex.what());
  }
  return true;
}

bool WorkspaceGroup::handleMouseEnter(GdkEventCrossing* const&) {
  if (auto* module = event_box_.get_child(); module != nullptr) {
    module->set_state_flags(Gtk::StateFlags::STATE_FLAG_PRELIGHT);
  }
  return false;
}

bool WorkspaceGroup::handleMouseLeave(GdkEventCrossing* const&) {
  if (auto* module = event_box_.get_child(); module != nullptr) {
    module->unset_state_flags(Gtk::StateFlags::STATE_FLAG_PRELIGHT);
  }
  return false;
}

void WorkspaceGroup::setCursor(Gdk::CursorType const&) {}

std::string WorkspaceGroup::shortName(const std::string& group) {
  if (group.empty()) {
    return "";
  }
  std::string short_name = group.substr(0, 1);
  short_name[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(short_name[0])));
  return short_name;
}

}  // namespace waybar::modules::sway
