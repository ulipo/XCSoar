// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "BatteryEdit.hpp"
#include "Form/DataField/Enum.hpp"
#include "Form/DataField/Listener.hpp"
#include "Widget/RowFormWidget.hpp"
#include "InfoBoxes/InfoBoxManager.hpp"
#include "InfoBoxes/InfoBoxSettings.hpp"
#include "Interface.hpp"
#include "UIState.hpp"
#include "UIGlobals.hpp"
#include "Language/Language.hpp"
#include "Profile/Current.hpp"
#include "Profile/InfoBoxConfig.hpp"
#include "Hardware/PowerFeatures.hpp"

class BatteryEditPanel final : public RowFormWidget,
                               private DataFieldListener {
  enum Controls { SOURCE, DISPLAY, CHEMISTRY };
  const unsigned id;
  const unsigned panel_index;

public:
  explicit BatteryEditPanel(unsigned _id) noexcept
    : RowFormWidget(UIGlobals::GetDialogLook()), id(_id),
      panel_index(CommonInterface::GetUIState().panel_index) {}

  void Prepare(ContainerWindow &parent,
               const PixelRect &rc) noexcept override {
    RowFormWidget::Prepare(parent, rc);
    const auto &settings = CommonInterface::GetUISettings().info_boxes
                             .panels[panel_index].battery[id];

    using Source = InfoBoxBatterySettings::Source;
    static constexpr StaticEnumChoice sources[] = {
      { Source::AUTOMATIC, N_("Default") },
#ifdef HAVE_BATTERY
      { Source::INTERNAL, N_("Internal battery") },
#endif
      { Source::DEVICE_A, N_("Device A") },
      { Source::DEVICE_B, N_("Device B") },
      { Source::DEVICE_C, N_("Device C") },
      { Source::DEVICE_D, N_("Device D") },
      { Source::DEVICE_E, N_("Device E") },
      { Source::DEVICE_F, N_("Device F") },
      { Source::DEVICE_G, N_("Device G") },
      nullptr
    };

    using Display = InfoBoxBatterySettings::Display;
    static constexpr StaticEnumChoice displays[] = {
      { Display::AUTOMATIC, N_("Automatic") },
      { Display::PERCENT, N_("Percentage") },
      { Display::VOLTAGE, N_("Voltage") },
      nullptr
    };

    using Chemistry = InfoBoxBatterySettings::Chemistry;
    static constexpr StaticEnumChoice chemistries[] = {
      { Chemistry::UNKNOWN, N_("No voltage-to-percent estimate") },
      { Chemistry::LI_ION_1S, N_("Li-ion / LiPo (1 cell)") },
      nullptr
    };

    auto source = settings.source;
#ifndef HAVE_BATTERY
    if (source == Source::INTERNAL)
      source = Source::AUTOMATIC;
#endif

    AddEnum(_("Source"), nullptr, sources,
            static_cast<unsigned>(source), this);
    AddEnum(_("Display"), nullptr, displays,
            static_cast<unsigned>(settings.display), this);
    AddEnum(_("Battery type"),
            _("The estimate is only applicable to a single Li-ion/LiPo cell."),
            chemistries, static_cast<unsigned>(settings.chemistry), this);
    UpdateRows();
  }

private:
  void UpdateRows() noexcept {
    const auto source = static_cast<InfoBoxBatterySettings::Source>(
      GetValueEnum(SOURCE));
    const bool external = source >= InfoBoxBatterySettings::Source::DEVICE_A;
    SetRowAvailable(DISPLAY, external);
    SetRowAvailable(CHEMISTRY, external);
  }

  void OnModified(DataField &) noexcept override {
    auto &panel = CommonInterface::SetUISettings().info_boxes
                    .panels[panel_index];
    auto &settings = panel.battery[id];

    const auto source = static_cast<InfoBoxBatterySettings::Source>(
      GetValueEnum(SOURCE));
    const auto display = static_cast<InfoBoxBatterySettings::Display>(
      GetValueEnum(DISPLAY));
    const auto chemistry = static_cast<InfoBoxBatterySettings::Chemistry>(
      GetValueEnum(CHEMISTRY));

    if (settings.source == source && settings.display == display &&
        settings.chemistry == chemistry)
      return;

    settings.source = source;
    settings.display = display;
    settings.chemistry = chemistry;
    UpdateRows();
    Profile::Save(Profile::map, panel, panel_index);
    InfoBoxManager::SetDirty();
    InfoBoxManager::ScheduleRedraw();
  }
};

std::unique_ptr<Widget>
LoadBatteryEditPanel(unsigned id)
{
  return std::make_unique<BatteryEditPanel>(id);
}
