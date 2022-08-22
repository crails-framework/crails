#include "module.hpp"

std::string render_scaffolds_module_routes_cpp(const Crails::Renderer*, Crails::SharedVars& vars)
{
  return
    "#include <crails/router.hpp>\n"
    "#include \"module.hpp\"\n"
    "\n"
    "using namespace " + Crails::cast<std::string>(vars, "namespace_name") + ";\n"
    "\n"
    "void Module::initialize_router(Crails::Router& router)\n"
    "{\n"
    "  // Append routes here (do not remove this line)"
    "}";
}

std::string render_scaffolds_module_module_hpp(const Crails::Renderer*, Crails::SharedVars& vars)
{
  return
    "#pragma once\n"
    "#ifdef " + Crails::cast<std::string>(vars, "define") + "\n"
    "\n"
    "namespace Crails { class Router; }\n"
    "\n"
    "namespace " + Crails::cast<std::string>(vars, "namespace_name") + "\n"
    "{\n"
    "  struct Module\n"
    "  {\n"
    "    static void initialize_router(Crails::Router&);"
    "  ;}\n"
    "}\n"
    "#endif";
}

std::string render_scaffolds_module_cmakelists_txt(const Crails::Renderer*, Crails::SharedVars& vars)
{
  return
    "file(GLOB_RECURSE module_files *.cpp *.cxx)\n"
    "set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -D" + Crails::cast<std::string>(vars, "define") + " PARENT_SCOPE)\n"
    "set(crails_app ${crails_app} ${module_files} PARENT_SCOPE)";
}
