#!/usr/bin/env python
env = SConscript("godot-cpp/SConstruct")

env.Append(CPPPATH=["."])
sources = Glob("*.cpp")

library = env.SharedLibrary(
    "bin/librmvkh{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
    source=sources,
)

Default(library)
