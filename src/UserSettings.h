#pragma once

#include <optional>

#include <juce_core/juce_core.h>

namespace rgain::settings
{
    std::optional<int> loadDefaultThemeIndex();
    void saveDefaultThemeIndex(int themeIndex);
}
