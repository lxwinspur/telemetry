#pragma once

#include "interfaces/report_manager.hpp"
#include "interfaces/trigger_factory.hpp"
#include "interfaces/trigger_manager.hpp"

#include <sdbusplus/asio/object_server.hpp>

#include <memory>
#include <vector>

class TriggerManager : public interfaces::TriggerManager
{
  public:
    TriggerManager(
        std::unique_ptr<interfaces::TriggerFactory> triggerFactory,
        std::unique_ptr<interfaces::JsonStorage> triggerStorage,
        const std::shared_ptr<sdbusplus::asio::object_server>& objServer);

    TriggerManager(TriggerManager&) = delete;
    TriggerManager(TriggerManager&&) = delete;
    TriggerManager& operator=(TriggerManager&) = delete;
    TriggerManager& operator=(TriggerManager&&) = delete;

    void removeTrigger(const interfaces::Trigger* trigger) override;

  private:
    std::unique_ptr<interfaces::TriggerFactory> triggerFactory;
    std::unique_ptr<interfaces::JsonStorage> triggerStorage;
    std::unique_ptr<sdbusplus::asio::dbus_interface> managerIface;
    std::vector<std::unique_ptr<interfaces::Trigger>> triggers;

    void verifyAddTrigger(const std::string& triggerName, bool isDiscrete,
                          const LabeledTriggerThresholdParams& thresholdParams);

    interfaces::Trigger&
        addTrigger(const std::string& triggerName, bool isDiscrete,
                   bool logToJournal, bool logToRedfish, bool updateReport,
                   const std::vector<LabeledSensorInfo>& labeledSensors,
                   const std::vector<std::string>& reportNames,
                   const LabeledTriggerThresholdParams& labeledThresholdParams);
    void loadFromPersistent();

  public:
    static constexpr size_t maxTriggers{TELEMETRY_MAX_TRIGGERS};
    static constexpr const char* triggerManagerIfaceName =
        "xyz.openbmc_project.Telemetry.TriggerManager";
    static constexpr const char* triggerManagerPath =
        "/xyz/openbmc_project/Telemetry/Triggers";
};
