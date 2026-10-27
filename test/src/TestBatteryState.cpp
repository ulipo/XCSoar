// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Device/BatteryState.hpp"
#include "TestUtil.hpp"

#include <limits>

static DeviceBatteryState
MakeState() noexcept
{
  DeviceBatteryState state{};
  state.clock = TimeStamp{FloatDuration{100}};
  state.alive.Update(state.clock);
  state.voltage_available.Update(state.clock);
  state.battery_level_available.Update(state.clock);
  state.voltage = 3.86;
  state.battery_level = 57;
  return state;
}

int
main()
{
  plan_tests(19);

  const auto initial = ReadDeviceBattery(DeviceBatteryState{});
  ok1(!initial.voltage_available && !initial.percent_available);

  auto state = MakeState();
  auto battery = ReadDeviceBattery(state);
  ok1(battery.voltage_available);
  ok1(battery.percent_available);
  ok1(battery.voltage == 3.86);
  ok1(battery.percent == 57);
  ok1(battery == ReadDeviceBattery(state));

  state.voltage_available.Clear();
  battery = ReadDeviceBattery(state);
  ok1(!battery.voltage_available && battery.percent_available);

  state = MakeState();
  state.battery_level_available.Clear();
  battery = ReadDeviceBattery(state);
  ok1(battery.voltage_available && !battery.percent_available);

  state = MakeState();
  state.alive.Clear();
  battery = ReadDeviceBattery(state);
  ok1(!battery.voltage_available && !battery.percent_available);

  state = MakeState();
  state.clock = TimeStamp{FloatDuration{111}};
  battery = ReadDeviceBattery(state);
  ok1(!battery.voltage_available && !battery.percent_available);

  state = MakeState();
  state.clock = TimeStamp{FloatDuration{401}};
  state.alive.Update(state.clock);
  state.battery_level_available.Update(state.clock);
  battery = ReadDeviceBattery(state);
  ok1(!battery.voltage_available && battery.percent_available);

  state = MakeState();
  state.clock = TimeStamp{FloatDuration{401}};
  state.alive.Update(state.clock);
  state.voltage_available.Update(state.clock);
  battery = ReadDeviceBattery(state);
  ok1(battery.voltage_available && !battery.percent_available);

  state = MakeState();
  state.voltage = std::numeric_limits<double>::quiet_NaN();
  ok1(!ReadDeviceBattery(state).voltage_available);

  state.voltage = std::numeric_limits<double>::infinity();
  ok1(!ReadDeviceBattery(state).voltage_available);

  state.voltage = 0;
  ok1(!ReadDeviceBattery(state).voltage_available);

  state = MakeState();
  state.battery_level = -1;
  ok1(!ReadDeviceBattery(state).percent_available);

  state.battery_level = 101;
  ok1(!ReadDeviceBattery(state).percent_available);

  state.battery_level = 0;
  ok1(ReadDeviceBattery(state).percent_available);

  state.battery_level = 100;
  ok1(ReadDeviceBattery(state).percent_available);

  return exit_status();
}
