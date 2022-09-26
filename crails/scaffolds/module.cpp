#include "module.hpp"
#include <crails/utils/string.hpp>
#include "../file_editor.hpp"

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
    "  // Append routes here (do not remove this line)\n"
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
    "    static void initialize_router(Crails::Router&);\n"
    "  };\n"
    "}\n"
    "#endif";
}

std::string render_scaffolds_module_cmakelists_txt(const Crails::Renderer*, Crails::SharedVars& vars)
{
  return
    "file(GLOB_RECURSE module_files *.cpp *.cxx)\n"
    "set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -D" + Crails::cast<std::string>(vars, "define") + "\" PARENT_SCOPE)\n"
    "set(crails_app ${crails_app} ${module_files} PARENT_SCOPE)";
}

int ModuleScaffold::create(boost::program_options::variables_map& options)
{
  ProjectConfiguration configuration;

  if (!options.count("name"))
  {
    std::cerr << "Option --name required." << std::endl;
    return -1;
  }
  configuration.initialize();
  name           = options["name"].as<std::string>();
  define.reserve(name.length());
  std::transform(name.begin(), name.end(), std::back_inserter(define), [](unsigned char c) { return std::toupper(c); });
  define         = "WITH_MODULE_" + define;
  namespace_name = Crails::camelize(name);
  path           = std::filesystem::path("modules/" + name);
  std::filesystem::create_directories(path);
  renderer.vars = {{"name", name}, {"namespace_name", namespace_name}, {"define", define}};
  renderer.generate_file("scaffolds/modules/module.hpp", path.string() + "/module.hpp");
  renderer.generate_file("scaffolds/modules/routes.cpp", path.string() + "/routes.cpp");
  update_router();
  if (configuration.toolchain() == "cmake")
    update_cmake(configuration);
  else
    std::cerr << "Toolchain `" << configuration.toolchain() << "`not supported.." << std::endl;
  configuration.add_module(name);
  configuration.save();
  return 0;
}

void ModuleScaffold::update_router()
{
  CppFileEditor router("app/routes.cpp");

  if (router.load_file() && router.use_symbol("Append routes here \\(do not remove this line\\)"))
  {
    router.insert(
      "  #ifdef " + define + "\n"
      "  " + namespace_name + "::Module::initialize_router(*this);\n"
      "  #endif\n"
    );
    router.add_include(path.string() + "/module.hpp");
    router.save_file();
  }
  else
    std::cout << "Cannot update app/routes.cpp" << std::endl;
}

void ModuleScaffold::update_cmake(ProjectConfiguration& configuration)
{
  CMakeFileEditor cmakefile(configuration);

  renderer.generate_file("scaffolds/modules/CMakeLists.txt", path.string() + "/CMakeLists.txt");
  cmakefile.load_file();
  cmakefile.add_module(name);
  cmakefile.save_file();
}
