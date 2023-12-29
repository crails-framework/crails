#include "module.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "../../file_renderer.hpp"
#include "../../file_editor.hpp"

using namespace std;

MongodbModule::MongodbModule()
{
  add_command("install",  []() { return make_shared<Installer>(); });
  add_command("disable",  []() { return make_shared<Disabler>(); });
}

static void add_database(const ProjectConfiguration& configuration)
{
  CppFileEditor databases("config/databases.cpp", "");
  stringstream  desc;

  desc << "      ";
  desc << "{\"mongo\",{";
  desc << ",{\"name\", \"" << configuration.variable("name") << "\"}";
  desc << ",{\"port\", static_cast<unsigned int>(27017)}";
  desc << "}}\n";
  databases.load_file();
  databases.use_symbol("Development, \\{");
  databases.insert(desc.str());
  databases.save_file();
}

int MongodbModule::Installer::run()
{
  CMakeFileEditor cmakefile(configuration);
  FileRenderer    renderer;

  if (!std::filesystem::exists("config/databases.cpp"))
    renderer.generate_file("config/databases.cpp");
  configuration.add_plugin("libcrails-mongodb");
  configuration.add_plugin("libcrails-databases");
  configuration.save();
  cmakefile.load_file();
  cmakefile.update_plugins();
  cmakefile.add_definitions({"WITH_MONGODB"});
  cmakefile.save_file();
  add_database(configuration);
  return 0;
}

int MongodbModule::Disabler::run()
{
  CMakeFileEditor cmakefile(configuration);

  configuration.remove_plugin("libcrails-mongodb");
  configuration.save();
  cmakefile.load_file();
  cmakefile.update_plugins();
  cmakefile.remove_definitions({"WITH_MONGODB"});
  cmakefile.save_file();
  return 0;
}
