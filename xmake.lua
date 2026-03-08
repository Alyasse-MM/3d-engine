add_rules("mode.debug", "mode.release")

add_requires("sfml 3", {configs = {graphics = true, window = true, system = true}})

target("3d-engine")
    set_kind("binary")
    set_languages("c++17")
    add_includedirs("src")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_packages("sfml")

task("vsrefresh")
    set_category("plugin")
    
    on_run(function ()
        import("core.base.task")
        
        os.exec("xmake project -k vsxmake -a x64")
        
        print("Visual Studio project regenerated")
    end)

    set_menu({
        usage = "xmake vsrefresh",
        description = "Regenerate the Visual Studio project"
    })