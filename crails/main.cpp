#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include "command_index.hpp"
#include "commands/new.hpp"
#include "commands/scaffold.hpp"
#include "commands/build.hpp"
#include "commands/plugins.hpp"
#include "commands/package.hpp"
#include "commands/deploy.hpp"
#include "templates/index.hpp"

using namespace Crails;
using namespace std;

const string Renderer::default_format = "*";

class Index : public CommandIndex
{
public:
  Index()
  {
    add_command("templates", []() { return make_shared<TemplatesIndex>(); });
    add_command("scaffold",  []() { return make_shared<::Scaffold>(); });
    add_command("build",     []() { return make_shared<BuildManager>(); });
    add_command("package",   []() { return make_shared<Package>(); });
    add_command("plugins",   []() { return make_shared<PluginManager>(); });
    add_command("new",       []() { return make_shared<New>(); });
    if (Deploy::is_available())
      add_command("deploy",  []() { return make_shared<Deploy>(); });
  }
};

int main(int argc, char **argv)
{
  Index index;

  if (index.initialize(argc, const_cast<const char**>(argv)))
    return index.run();
  return -1;
}
