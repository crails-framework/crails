#include "plugins.hpp"
#include "../plugins/odb/module.hpp"

using namespace std;

PluginManager::PluginManager()
{
  add_command("odb", []() { return make_shared<OdbModule>(); });
}
