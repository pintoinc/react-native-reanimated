#pragma once

#include <worklets/WorkletRuntime/WorkletRuntime.h>

#include <jsi/jsi.h>

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>

using namespace facebook;

namespace worklets {

class WorkletEventHandler;

class EventHandlerRegistry {
  std::map<
      std::pair<int, std::string>,
      std::unordered_map<uint64_t, std::shared_ptr<WorkletEventHandler>>>
      eventMappingsWithTag;
  std::map<
      std::string,
      std::unordered_map<uint64_t, std::shared_ptr<WorkletEventHandler>>>
      eventMappingsWithoutTag;
  std::map<uint64_t, std::shared_ptr<WorkletEventHandler>> eventHandlers;
  std::mutex instanceMutex;

 public:
  void registerEventHandler(
      const std::shared_ptr<WorkletEventHandler> &eventHandler);
  void unregisterEventHandler(const uint64_t id);

  void processEvent(
      const std::shared_ptr<WorkletRuntime> &uiWorkletRuntime,
      const double eventTimestamp,
      const std::string &eventName,
      const int emitterReactTag,
      const jsi::Value &eventPayload);

  bool isAnyHandlerWaitingForEvent(
      const std::string &eventName,
      const int emitterReactTag);
};

} // namespace worklets
