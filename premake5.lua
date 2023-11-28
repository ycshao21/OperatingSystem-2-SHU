workspace "OperatingSystem"
    architecture "x64"
    startproject "Exp01-ProcessScheduling"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Experiments/Exp01-ProcessScheduling"
include "Experiments/Exp02-Deadlock"