
set_project("CountDown")
set_version("1.0", {build = "%Y%m%d%H%M"})
set_xmakever("2.2.5")

set_warnings("all", "error")

set_languages("c11", "cxx17")

-- add_requires("lua")

if is_plat("windows") then 
    add_cxflags("-MT")
end

target("CountDown")
    set_kind("binary")
    -- add_packages("lua")
    -- add_files("*.cpp")
    add_files("*.cpp")