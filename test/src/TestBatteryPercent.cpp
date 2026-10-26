// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "Math/BatteryLevel.hpp"
#include "TestUtil.hpp"

#include <cmath>
#include <limits>

static bool
Approx(double value, double expected) noexcept
{
  return std::abs(value - expected) < 1e-6;
}

int
main()
{
  plan_tests(16);
  ok1(Approx(EstimateLiIon1SPercent(2.0), 0));
  ok1(Approx(EstimateLiIon1SPercent(3.6), 0));
  ok1(Approx(EstimateLiIon1SPercent(3.65), 2));
  ok1(Approx(EstimateLiIon1SPercent(3.7), 4));
  ok1(Approx(EstimateLiIon1SPercent(3.8), 37.3333333333333));
  ok1(Approx(EstimateLiIon1SPercent(3.86), 57.3333333333333));
  ok1(Approx(EstimateLiIon1SPercent(3.9), 70.6666666666667));
  ok1(Approx(EstimateLiIon1SPercent(4.0), 80));
  ok1(Approx(EstimateLiIon1SPercent(4.1), 90));
  ok1(Approx(EstimateLiIon1SPercent(4.2), 100));
  ok1(Approx(EstimateLiIon1SPercent(4.3), 100));
  ok1(Approx(EstimateLiIon1SPercent(4.199), 99.9));
  ok1(CanEstimateLiIon1SPercent(3.86));
  ok1(!CanEstimateLiIon1SPercent(2.4));
  ok1(!CanEstimateLiIon1SPercent(4.31));
  ok1(!CanEstimateLiIon1SPercent(
    std::numeric_limits<double>::quiet_NaN()));
  return exit_status();
}
