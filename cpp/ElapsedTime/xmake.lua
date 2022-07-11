
set_project("TimeRecord")
set_version("1.0", {build = "%Y%m%d%H%M"})
set_xmakever("2.2.5")

set_warnings("all", "error")

set_languages("c11", "cxx17")

if is_plat("windows") then 
    add_cxflags("-MT")
end

target("TimeRecord")
    set_kind("binary")
    add_files("*.cpp")