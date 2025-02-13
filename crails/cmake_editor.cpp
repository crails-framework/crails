#include "project_configuration.hpp"
#include "cmake_editor.hpp"
#include <sstream>

using namespace std;

CmakeEditor::CmakeEditor(const ProjectConfiguration& configuration) :
  ToolchainEditor(configuration),
  root(configuration)
{
}

string CmakeEditor::plugins_config_line() const
{
  ostringstream stream;
  list<string>  plugins = configuration.plugins();
  string        version = configuration.version();

  stream << "pkg_check_modules(CRAILS REQUIRED";
  for (const string& plugin : plugins)
  {
    if (plugin.substr(0, 3) == "lib")
      stream << ' ' << plugin  << ">=" << version;
  }
  stream << ')' << endl;
  return stream.str();
}

void CmakeEditor::update_plugins()
{
  root.update_plugins(plugins_config_line());
}

void CmakeEditor::add_definitions(const std::set<std::string>& defines)
{
  root.add_definitions(defines);
}

void CmakeEditor::remove_definitions(const std::set<std::string>& defines)
{
  root.remove_definitions(defines);
}

void CmakeEditor::add_dependency(const std::string& name, const std::string& category)
{
  if (category != "package")
  {
    if (name.find("lib") == 0)
      root.add_dependency(name.substr(3), category);
    else
      root.add_dependency(name, category);
  }
}

void CmakeEditor::add_task(const std::string& name)
{
  root.add_task(name);
}

void CmakeEditor::add_module(const std::string& name)
{
  root.add_module(name);
}

void CmakeEditor::remove_module(const std::string& name)
{
  root.remove_module(name);
}
