---
--- This file registers hooks called by DCS on simulation actions.
--- Created by George Brooker <brookerg123@gmail.com>.
--- DateTime: 19/06/2024 2:57 AM NZST
---

dcs_install = lfs.writedir()
entryLoc = dcs_install.."Mods\\aircraft\\A-4E-C\\entry.lua"

log.write("Community.A4E-C.Hooks", log.INFO, "A4E-C Hooks loaded")

-- get current version from the entry.lua file
function get_version()
    local version = nil
    local entryFile = io.open(entryLoc, "r")

    if not entryFile then
        log.write("Community.A4E-C.Hooks", log.ERROR, "Could not open entry.lua file")
        return "Unknown"
    else
        for line in entryFile:lines() do
            version = string.match(line, 'version%s*=%s*"([^"]+)"')
            if version then
                break
            end
        end

        if version then
            return version
        else
            return "Unknown"
        end
    end
end

-- Log the version of the module on DCS startup
local version = 'Community A4E-C Version ' .. get_version()
log.write("Community.A4E-C", log.INFO, version)

-- Register the callbacks for the module
do
    if isA4EModuleInitialized~=true then
        local A4ECallbacks = {}

        -- Use these to log info based on sim events (such as mission start, etc) This is where the radio info will be logged etc
        function A4ECallbacks.onSimulationStart()
            log.write("Community.A4E-C.Hooks", log.INFO, version.."Simulation Started")
        end

        -- Finish the callbacks and register them, log successful registration
        DCS.setUserCallbacks(A4ECallbacks)
        log.write("Community.A4E-C.Hooks", log.INFO, "A4E-C Callbacks Registered")

        isA4EModuleInitialized = true
    end
end