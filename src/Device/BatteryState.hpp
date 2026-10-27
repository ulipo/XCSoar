// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include "Device/Features.hpp"
#include "time/Validity.hpp"
#include "time/Stamp.hpp"

#include <chrono>
#include <cmath>

/** Per-device battery measurements copied during GPS blackboard updates. */
struct DeviceBatteryState {
  TimeStamp clock;
  Validity alive;
  Validity voltage_available;
  Validity battery_level_available;
  double voltage;
  double battery_level;
};

// The UI only needs the battery measurements, not the complete NMEA state.
struct DeviceBatteryReading {
  double voltage = 0;
  double percent = 0;
  bool voltage_available = false;
  bool percent_available = false;

  bool operator==(const DeviceBatteryReading &) const noexcept = default;
};

/** Validate connection freshness and independent measurement lifetimes. */
[[gnu::pure]]
inline DeviceBatteryReading
ReadDeviceBattery(const DeviceBatteryState &source) noexcept
{
  DeviceBatteryReading result;

  if (source.alive.IsOlderThan(source.clock, std::chrono::seconds(10)))
    return result;

  if (!source.voltage_available.IsOlderThan(source.clock,
                                            std::chrono::minutes(5)) &&
      std::isfinite(source.voltage) && source.voltage > 0) {
    result.voltage_available = true;
    result.voltage = source.voltage;
  }

  if (!source.battery_level_available.IsOlderThan(source.clock,
                                                  std::chrono::minutes(5)) &&
      std::isfinite(source.battery_level) && source.battery_level >= 0 &&
      source.battery_level <= 100) {
    result.percent_available = true;
    result.percent = source.battery_level;
  }

  return result;
}
