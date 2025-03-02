workspace "SpriteSheet-Generator"
    architecture   "x64"
    startproject   "SpriteSheet-Generator"
    configurations { "Debug", "Release", "Dist" }
    flags          { "MultiProcessorCompile"    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["spdlog"]    = "vendor/Quirk/Quirk/vendor/spdlog/include"
IncludeDir["Glad"]      = "vendor/Quirk/Quirk/vendor/Glad/include"
IncludeDir["Imgui"]     = "vendor/Quirk/Quirk/vendor/imgui"
IncludeDir["Glm"]       = "vendor/Quirk/Quirk/vendor/glm"
IncludeDir["Opengl"]    = "vendor/Quirk/Quirk/vendor/OpenGL"
IncludeDir["Stb_Image"] = "vendor/Quirk/Quirk/vendor/stb_image"
IncludeDir["Entt"]      = "vendor/Quirk/Quirk/vendor/entt/include"
IncludeDir["Yaml_CPP"]  = "vendor/Quirk/Quirk/vendor/yaml-cpp/include"

include "vendor/Quirk/Quirk"
include "SpriteSheet-Generator"
