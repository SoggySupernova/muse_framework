/*
 * SPDX-License-Identifier: GPL-3.0-only
 * MuseScore-CLA-applies
 *
 * MuseScore Studio
 * Music Composition & Notation
 *
 * Copyright (C) 2021 MuseScore Limited and others
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "modularity/imoduleinterface.h"

#include "global/types/ret.h"
#include "global/io/path.h"
#include "global/progress.h"
#include "audiopluginstypes.h"

namespace muse::audioplugins {
struct PluginScanResult {
    io::paths_t newPluginPaths;            // not in cache; will be inserted via subprocess validation
    AudioResourceIdList missingPluginIds;  // in cache but not currently found by any scanner
    AudioResourceIdList rediscoveredPluginIds; // previously Missing entries the scanner found again
};

class IRegisterAudioPluginsScenario : MODULE_CONTEXT_INTERFACE
{
    INTERFACE_ID(IRegisterAudioPluginsScenario)

public:
    virtual ~IRegisterAudioPluginsScenario() = default;

    virtual PluginScanResult scanPlugins(Progress* progress = nullptr) const = 0;

    virtual Ret updatePluginsRegistry() = 0;
    // `validate=false` persists the paths as Discovered placeholders only;
    // out-of-process validation is skipped and the entries will be re-offered
    // for validation on the next scan. Default `true` runs the full scan.
    // Returns the first cache write/load failure encountered, or ok.
    virtual Ret registerNewPlugins(const io::paths_t& pluginPaths, bool validate = true) = 0;
    virtual Ret unregisterRemovedPlugins(const AudioResourceIdList& pluginIds) = 0;

    virtual Ret registerPlugin(const io::path_t& pluginPath) = 0;
    virtual Ret registerFailedPlugin(const io::path_t& pluginPath, int failCode) = 0;
};
}
