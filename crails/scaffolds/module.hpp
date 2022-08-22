#pragma once
#include "scaffold_model.hpp"
#include <crails/utils/string.hpp>
#include "../project_configuration.hpp"
#include "../file_renderer.hpp"
#include "../file_editor.hpp"

class ModuleScaffold : public ScaffoldModel
{
  FileRenderer renderer;
  boost::filesystem::path path;
  std::string name;
  std::string define;
  std::string namespace_name;
public:
  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("name,n",   boost::program_options::value<std::string>(), "module name");
  }

  int create(boost::program_options::variables_map& options) override
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
    std::transform(name.begin(), name.end(), define.begin(), [](unsigned char c) { return std::toupper(c); });
    define         = "WITH_MODULE_" + define;
    namespace_name = Crails::uppercase(name);
    path           = boost::filesystem::path("modules/" + name);
    boost::filesystem::create_directories(path);
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

  void update_router()
  {
    CppFileEditor router("app/routes.cpp");

    if (router.load_file() && router.use_symbol("Append routes here \\(do not remove this line\\)"))
    {
      router.insert(
        "  #ifdef " + define + "\n"
        "  " + namespace_name + "::Module::initialize_router(*this);\n"
        "  #endif"
      );
      router.add_include(path.string() + "/module.hpp");
      router.save_file();
    }
    else
      std::cout << "Cannot update app/routes.cpp" << std::endl;
  }

  void update_cmake(ProjectConfiguration& configuration)
  {
    CMakeFileEditor cmakefile(configuration);

    renderer.generate_file("scaffolds/modules/CMakeLists.txt", path.string() + "/CMakeLists.txt");
    cmakefile.load_file();
    cmakefile.add_module(name);
    cmakefile.save_file();
  }
};
