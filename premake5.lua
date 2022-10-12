-- premake5.lua
workspace "CactusRT"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "CactusRT"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "WalnutExternal.lua"
include "CactusRT"