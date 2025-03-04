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
  FileRenderer renderer;
  auto         toolchain = configuration.toolchain_editor();

  if (!std::filesystem::exists("config/databases.cpp"))
  {
    MainCppEditor   main_cpp("app/main.cpp");

    renderer.generate_file("config/databases.hpp");
    renderer.generate_file("config/databases.cpp");
    main_cpp.add_include("config/databases.hpp");
    main_cpp.add_to_main_function("SingletonInstantiator<ApplicationDatabases> databases;\n");
    main_cpp.save_file();
  }
  configuration.add_plugin("libcrails-mongodb");
  configuration.add_plugin("libcrails-databases");
  configuration.add_plugin("libcrails-database-url");
  configuration.save();
  toolchain->load_file();
  toolchain->update_plugins();
  toolchain->add_definitions({"WITH_MONGODB"});
  toolchain->save_file();
  add_database(configuration);
  return 0;
}

int MongodbModule::Disabler::run()
{
  auto toolchain = configuration.toolchain_editor();

  configuration.remove_plugin("libcrails-mongodb");
  configuration.save();
  toolchain->load_file();
  toolchain->update_plugins();
  toolchain->remove_definitions({"WITH_MONGODB"});
  toolchain->save_file();
  return 0;
}
