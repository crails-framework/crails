#include "modules.hpp"
#include "../modules/odb/module.hpp"

using namespace std;

ModuleManager::ModuleManager()
{
  add_command("odb", []() { return make_shared<OdbModule>(); });
}
