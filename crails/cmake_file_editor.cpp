#include "file_editor.hpp"
#include "project_configuration.hpp"
#include <sstream>

using namespace std;

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
