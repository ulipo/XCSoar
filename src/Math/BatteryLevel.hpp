// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#pragma once

#include <cmath>

/**
 * Approximate charge of a single Li-ion/LiPo cell using the voltage
 * curve originally provided by the BlueFlyVario driver.
 * Not a fuel gauge: load, temperature and cell age affect accuracy.
 */
[[gnu::const]]
constexpr double
EstimateLiIon1SPercent(double voltage) noexcept
{
  const double mV = voltage * 1000.;
  if (mV > 3900)
    return mV >= 4200 ? 100 : 70 + (mV - 3900) / 10.;
  if (mV > 3700)
    return 4 + (mV - 3700) / 3.;
  if (mV > 3600)
    return 0.04 * (mV - 3600);
  return 0;
}

/** Disallow invalid measurements for generic voltage-to-charge estimates. */
[[gnu::const]]
inline bool
CanEstimateLiIon1SPercent(double voltage) noexcept
{
  return std::isfinite(voltage) && voltage >= 2.5 && voltage <= 4.3;
}
