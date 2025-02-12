#include "file_editor.hpp"
#include "project_configuration.hpp"
#include <sstream>

using namespace std;

class Build2Editor
{
public:
  Build2Editor(ProjectConfiguration& configuration);

  void update_plugins();
private:
  ProjectConfiguration configuration;
  CrailsFileEditor buildfile, manifest, repositories;
};

Build2Editor::Build2Editor(ProjectConfiguration& configuration) :
  configuration(configuration),
  buildfile("buildfile"),
  manifest("manifest"),
  repositories("repositories.manifest")
{
}

void Build2Editor::update_plugins()
{
  for (const string& plugin : configuration.plugins())
  {
    string git_organization_url = "https://github.com/crails-framework";
    string repository_block =
      ":\n"
      "role: prerequisite\n"
      "location: " + git_organization_url + '/' + plugin + '#' + configuration.version();
    string depends_line = "depends: " + plugin + " ^" + configuration.version();
    string import_line = "import libs += " + plugin + '%' + "lib{" + plugin.substr(3) + '}';

    if (repositories.find(repository_block) == std::string::npos)
      repositories.append('\n' + repository_block);
    if (manifest.find(depends_line) == std::string::npos)
      manifest.append('\n' + depends_line);
    if (plugin.substr(0, 3) == "lib" && buildfile.find(import_line) == std::string::npos)
      buildfile.prepend(import_line + '\n');
  }
}

CMakeFileEditor::CMakeFileEditor(ProjectConfiguration& configuration) : CrailsFileEditor("CMakeLists.txt"), configuration(configuration)
{
  prefix_pattern = "#";
}

string CMakeFileEditor::plugins_config_line() const
{
  stringstream stream;
  list<string> plugins = configuration.plugins();
  string       version = configuration.version();

  stream << "pkg_check_modules(CRAILS REQUIRED";
  for (const string& plugin : plugins)
  {
    if (plugin.substr(0, 3) == "lib")
      stream << ' ' << plugin  << ">=" << version;
  }
  stream << ')' << endl;
  return stream.str();
}

void CMakeFileEditor::update_plugins()
{
  size_t position = contents.find("pkg_check_modules(CRAILS");
  size_t end_position = contents.find("\n", position) + 1;

  contents.erase(position, end_position - position);
  contents.insert(position, plugins_config_line());
}

static const char* dependency_symbol = "Custom dependencies \\(do not modify this line\\)";

static std::string render_add_definitions(const std::set<std::string>& defines)
{
  stringstream stream;

  stream << "add_definitions(";
  for (const auto& define : defines)
  {
    if (define != *defines.begin())
      stream << ", ";
    stream << "-D" << define;
  }
  stream << ')' << endl;
  return stream.str();
}

void CMakeFileEditor::add_definitions(const std::set<std::string>& defines)
{
  use_symbol(dependency_symbol);
  insert(render_add_definitions(defines));
}

void CMakeFileEditor::remove_definitions(const std::set<std::string>& defines)
{
  std::string pattern = render_add_definitions(defines);
  size_t position = contents.find(pattern);
  size_t end_position = contents.find(contents.find("\n", position)) + 1;

  if (position != std::string::npos)
    contents.erase(position, end_position - position);
}

void CMakeFileEditor::add_dependency(const std::string& name, const std::string& category)
{
  stringstream stream;

  stream << "set(" << category << " ${" << category << "} " << name << ')' << endl;
  use_symbol(dependency_symbol);
  insert(stream.str());
}

void CMakeFileEditor::add_task(const std::string& name)
{
  contents += "\nadd_subdirectory(exe/" + name + ')';
}

void CMakeFileEditor::add_module(const std::string& name)
{
  contents += "\nadd_subdirectory(modules/" + name + ')';
}

void CMakeFileEditor::remove_module(const std::string& name)
{
  std::string pattern = "\nadd_subdirectory(modules/" + name + ')';
  int position = contents.find(pattern);
  if (position != std::string::npos)
    contents.erase(position, pattern.length());
}
