
set_project("BitArray")
set_version("1.0", {build = "%Y%m%d%H%M"})
set_xmakever("2.2.5")

set_warnings("all", "error")
-- set_languages("c99")

add_requires("lua")

if is_plat("windows") then 
    add_cxflags("-MT")
end

target("bitarray")
    set_kind("shared")
    set_languages("c89")
    add_packages("lua")
    add_files("*.c")

    after_build(function(target) 
            -- https://xmake.io/#/zh-cn/manual/target_instance
            -- print("kind:", target:kind())
            -- print("targetfile:", target:targetfile())
            -- print("targetdir:", target:targetdir())
            -- print("basename:", target:basename())
            -- print("filename:", target:filename())
            -- https://xmake.io/#/zh-cn/manual/builtin_variables
            -- print("builddir: $(buildir)")
            -- print("curdir: $(curdir)")
            -- print("projectdir: $(projectdir)")
            
            -- https://xmake.io/#/zh-cn/manual/builtin_modules?id=oscp
            print("mv", target:filename(), "to curdir")
            os.cp(target:targetfile(), "$(curdir)")        
    end)
    