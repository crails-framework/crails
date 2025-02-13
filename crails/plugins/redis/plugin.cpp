#include <filesystem>
#include "plugin.hpp"
#include "../../file_renderer.hpp"
#include "../../file_editor.hpp"

RedisPlugin::RedisPlugin()
{
  add_command("install",  []() { return make_shared<RedisInstaller>(); });
  add_command("disable",  []() { return make_shared<RedisDisabler>(); });
}

static void add_database(const ProjectConfiguration& configuration)
{
  CppFileEditor databases("config/databases.cpp", "");
  stringstream  desc;
  const std::map<std::string, unsigned short> ports = {{"mysql", 3306}, {"pgsql", 5432}};

  desc << "      ";
  desc << "{\"redis\",{";
  desc << "{\"host\",\"127.0.0.1\"}";
  desc << ",{\"port\", static_cast<unsigned int>(6379)}";
  desc << "}}\n";
  databases.load_file();
  databases.use_symbol("Development, \\{");
  databases.insert(desc.str());
  databases.save_file();
}

int RedisPlugin::RedisInstaller::run()
{
  FileRenderer renderer;

  if (!std::filesystem::exists("config/databases.cpp"))
    renderer.generate_file("config/databases.cpp");
  configuration.remove_plugin("libcrails-databases"); // must be included after libcrails-odb
  configuration.add_plugin("libcrails-redis");
  configuration.add_plugin("libcrails-databases");
  configuration.add_plugin("libcrails-database-url");
  configuration.update_plugins();
  add_database(configuration);
  return 0;
}

int RedisPlugin::RedisDisabler::run()
{
  configuration.remove_plugin("libcrails-redis");
  configuration.save();
  return 0;
}
