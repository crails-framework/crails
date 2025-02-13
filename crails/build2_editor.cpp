#include "build2_editor.hpp"
#include <iomanip>

using namespace std;

Build2Editor::Build2Editor(const ProjectConfiguration& configuration) :
  configuration(configuration),
  buildfile("app/buildfile"),
  manifest("manifest"),
  repositories("repositories.manifest")
{
}

bool Build2Editor::load_file()
{
  return buildfile.load_file() && manifest.load_file() && repositories.load_file();
}

void Build2Editor::save_file()
{
  buildfile.save_file();
  manifest.save_file();
  repositories.save_file();
}

void Build2Editor::update_plugins()
{
  for (const string& plugin : configuration.plugins())
  {
    string git_organization_url = "https://github.com/crails-framework";
    string repository_block =
      ":\n"
      "role: prerequisite\n"
      "location: " + git_organization_url + '/' + plugin + ".git#" + configuration.version();
    string depends_line = "depends: " + plugin + " ^" + configuration.version();
    string import_line = "import intf_libs += " + plugin + '%' + "lib{" + plugin.substr(3) + '}';

    if (repositories.find(repository_block) == std::string::npos)
      repositories.append('\n' + repository_block);
    if (manifest.find(depends_line) == std::string::npos)
      manifest.append('\n' + depends_line);
    if (plugin.substr(0, 3) == "lib" && buildfile.find(import_line) == std::string::npos)
      buildfile.prepend(import_line + '\n');
  }
}

void Build2Editor::add_definitions(const set<string>& definitions)
{
  ostringstream stream;

  stream << "cxx.poptions +=";
  for (const string& definition : definitions)
    stream << ' ' << quoted("-D" + definition);
  stream << '\n';
  buildfile.prepend(stream.str());
}
