// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "LiveBlackboard.hpp"
#include "Device/BatteryState.hpp"

#include <array>

class InterfaceBlackboard : public LiveBlackboard
{
  std::array<DeviceBatteryState, INTERNAL_DEVICE_SLOT> device_batteries{};

public:
  void SetDeviceBatteries(
    const std::array<DeviceBatteryState, INTERNAL_DEVICE_SLOT> &batteries
  ) noexcept {
    device_batteries = batteries;
  }

  const auto &GetDeviceBatteries() const noexcept {
    return device_batteries;
  }

  void ReadBlackboardBasic(const MoreData &nmea_info) noexcept;
  void ReadBlackboardCalculated(const DerivedInfo &derived_info) noexcept;

  [[gnu::const]]
  SystemSettings &SetSystemSettings() noexcept {
    return system_settings;
  }

  [[gnu::const]]
  ComputerSettings& SetComputerSettings() noexcept {
    return computer_settings;
  }

  [[gnu::const]]
  UISettings &SetUISettings() noexcept {
    return ui_settings;
  }

  inline void ReadCommonStats(const CommonStats &common_stats) noexcept {
    calculated_info.common_stats = common_stats;
  }

  void ReadComputerSettings(const ComputerSettings &settings) noexcept;
};
