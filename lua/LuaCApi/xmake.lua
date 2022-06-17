
-- project
set_project("StudyLuaCApi")

-- version
set_version("1.0", {build = "%Y%m%d%H%M"})

-- set xmake min version
set_xmakever("2.2.5")

-- set warning all as error
set_warnings("all", "error")

-- set language: c99, c++11
-- set_languages("c99")

add_requires("lua")

-- for the windows platform (msvc)
if is_plat("windows") then
    add_cxflags("-MT")
end

target("main")
    set_kind("binary")
    set_languages("c89")    
    add_packages("lua")
    add_files("*.c")

