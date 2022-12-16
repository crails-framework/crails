#include "project_configuration.hpp"
#include <crails/render_file.hpp>
#include <crails/read_file.hpp>
#include <crails/cli/process.hpp>
#include <crails/utils/string.hpp>
#include <filesystem>
#include <algorithm>
#include "version.hpp"

using namespace std;

ProjectConfiguration::ProjectConfiguration() : Crails::ProjectVariables(".crails")
{
}

static std::string get_current_process_file()
{
  string process_link;
  string uname;

  if (Crails::run_command("uname -a", uname))
  {
    if (uname.find("Linux") != string::npos)
      process_link = "/proc/self/exe";
    else if (uname.find("FreeBSD") != string::npos)
      process_link = Crails::which("crails");
  }
  if (!process_link.length())
    return filesystem::path();
  return filesystem::canonical(process_link);
}

string ProjectConfiguration::crails_bin_path()
{
  const static filesystem::path path = get_current_process_file();

  return path.parent_path().string();
}

string ProjectConfiguration::application_build_path() const
{
  if (toolchain() == "cmake")
    return project_directory() + "/build";
  else if (toolchain() == "build2")
    return project_directory() + "/package";
  return project_directory();
}

string ProjectConfiguration::project_name() const { return variable_or("name", "MyApplication"); }

string ProjectConfiguration::version() const { return variable_or("crails-version", LIBCRAILS_VERSION_STR); }
void ProjectConfiguration::version(const string& version) { variables["crails-version"] = version; }


string ProjectConfiguration::toolchain() const { return variable_or("build-system", "cmake"); }
void ProjectConfiguration::toolchain(const string& value) { variables["build-system"] = value; }

string ProjectConfiguration::build_type() const { return variable_or("build-type", "Release"); }
void ProjectConfiguration::build_type(const string& value) { variables["build-type"] = value; }

list<string> ProjectConfiguration::plugins() const
{
  auto it = variables.find("plugins");

  return it == variables.end() ? list<string>() : Crails::split(it->second, ',');
}

void ProjectConfiguration::plugins(const list<string>& value)
{
  variables["plugins"] = Crails::join(value, ',');
}

void ProjectConfiguration::add_plugin(const std::string& value)
{
  auto list = plugins();
  if (find(list.begin(), list.end(), value) == list.end()) list.push_back(value);
  plugins(list);
}

void ProjectConfiguration::remove_plugin(const std::string& value)
{
  auto list = plugins();
  auto it = find(list.begin(), list.end(), value);
  if (it != list.end())
  {
    list.erase(it);
    plugins(list);
  }
}

bool ProjectConfiguration::has_plugin(const std::string& name) const
{
  auto list = plugins();
  return find(list.begin(), list.end(), name) != list.end();
}

list<string> ProjectConfiguration::modules() const
{
  if (variables.find("modules") != variables.end())
    return Crails::split(variables.at("modules"), ',');
  return {};
}

void ProjectConfiguration::add_module(const std::string& value)
{
  auto list = plugins();
  if (find(list.begin(), list.end(), value) == list.end()) list.push_back(value);
  variables["modules"] = Crails::join(list, ',');
}

void ProjectConfiguration::remove_module(const std::string& value)
{
  auto list = modules();
  auto it = find(list.begin(), list.end(), value);
  if (it != list.end())
  {
    list.erase(it);
    variables["modules"] = Crails::join(list, ',');
  }
}

list<string> ProjectConfiguration::renderers() const
{
  auto it = variables.find("renderers");

  return it == variables.end() ? list<string>() : Crails::split(it->second, ',');
}

void ProjectConfiguration::renderers(const list<string>& value)
{
  variables["renderers"] = Crails::join(value, ',');
}

void ProjectConfiguration::add_renderer(const string& value)
{
  auto renderer_list = renderers();
  if (find(renderer_list.begin(), renderer_list.end(), value) == renderer_list.end())
    renderer_list.push_back(value);
  renderers(renderer_list);
}

void ProjectConfiguration::remove_renderer(const string& value)
{
  auto renderer_list = renderers();
  auto it = find(renderer_list.begin(), renderer_list.end(), value);
  if (it != renderer_list.end())
  {
    renderer_list.erase(it);
    renderers(renderer_list);
  }
}

list<string> ProjectConfiguration::asset_roots() const
{
  auto it = variables.find("asset-roots");

  return it == variables.end() ? list<string>() : Crails::split(it->second, ';');
}

void ProjectConfiguration::asset_roots(const std::list<std::string>& value)
{
  variables["asset-roots"] = Crails::join(value, ';');
}

string ProjectConfiguration::project_directory()
{
  return ProjectConfiguration().lookup_variable_path();
}

void ProjectConfiguration::move_to_project_directory()
{
  filesystem::current_path(project_directory());
}
