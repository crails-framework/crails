#include "plugins.hpp"
#include "../plugins/odb/module.hpp"
#include "../plugins/comet/plugin.hpp"

using namespace std;

PluginManager::PluginManager()
{
  add_command("odb", []() { return make_shared<OdbModule>(); });
  add_command("comet", []() { return make_shared<CometPlugin>(); });
}
