#pragma once

#include <fmt/format.h>

#include <mutex>

#include "ALabel.hpp"
#include "modules/sway/ipc/client.hpp"
#include "util/json.hpp"

namespace waybar::modules::sway {

class WorkspaceGroup : public ALabel, public sigc::trackable {
 public:
  WorkspaceGroup(const std::string&, const Json::Value&);
  ~WorkspaceGroup() override = default;
  auto update() -> void override;

 private:
  void onEvent(const struct Ipc::ipc_response&);
  void onCmd(const struct Ipc::ipc_response&);
  bool handleToggle(GdkEventButton* const&) override;
  bool handleMouseEnter(GdkEventCrossing* const&) override;
  bool handleMouseLeave(GdkEventCrossing* const&) override;
  void setCursor(Gdk::CursorType const&) override;
  static std::string shortName(const std::string&);

  std::string active_group_;
  bool inactive_group_urgent_ = false;
  util::JsonParser parser_;
  std::mutex mutex_;
  Ipc ipc_;
  Ipc command_ipc_;
};

}  // namespace waybar::modules::sway
