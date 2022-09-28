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

void CMakeFileEditor::add_dependency(const std::string& name)
{
  stringstream stream;

  stream << "set(dependencies ${dependencies} " << name << ')' << endl;
  use_symbol("Custom dependencies \\(do not modify this line\\)");
  insert(stream.str());
}

void CMakeFileEditor::add_task(const std::string& name)
{
  contents += "\nadd_subdirectory(tasks/" + name + ')';
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
