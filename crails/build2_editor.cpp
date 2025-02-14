#include "project_configuration.hpp"
#include "build2_editor.hpp"
#include <iomanip>

using namespace std;

Build2Editor::Build2Editor(const ProjectConfiguration& configuration) :
  ToolchainEditor(configuration),
  buildfile("app/buildfile"),
  manifest("manifest"),
  repositories("repositories.manifest"),
  root("build/root.build"),
  test_buildfile("spec/driver/buildfile")
{
}

bool Build2Editor::load_file()
{
  return buildfile.load_file() && manifest.load_file() && repositories.load_file() && root.load_file() && test_buildfile.load_file();
}

void Build2Editor::save_file()
{
  buildfile.save_file();
  manifest.save_file();
  repositories.save_file();
  root.save_file();
  test_buildfile.save_file();
}

string Build2Editor::repository_entry_for(const string& plugin) const
{
  const string git_organization_url = "https://github.com/crails-framework";
  return
    ":\n"
    "role: prerequisite\n"
    "location: " + git_organization_url + '/' + plugin + ".git#" + configuration.version();
}

void Build2Editor::require_repository_for(const string& plugin)
{
  string entry = repository_entry_for(plugin);
  
  if (repositories.find(entry) == string::npos)
    repositories.append('\n' + entry);
}

void Build2Editor::require_depends_for(const string& library, const string& version)
{
  string version_token = version.length() ? ('^' + version) : ">=0.0.0";
  string depends_lookup = "depends: " + library + ' ';
  string depends_line = depends_lookup + version_token;
  auto index = manifest.find(depends_lookup);
  auto end_index = manifest.find("\n", index) + 1;

  if (index != string::npos)
    manifest.erase(index, end_index - index);
  manifest.append('\n' + depends_line);
}

void Build2Editor::import_library(const string& library, CrailsFileEditor& buildfile)
{
  string unprefixed_name;
  string import_line;

  if (library.find("lib") == 0)
    unprefixed_name = library.substr(3);
  else
    unprefixed_name = library;
  import_line = "import intf_libs += "
    + library + '%'
    + "lib{" + unprefixed_name + '}';
  if (buildfile.find(import_line) == std::string::npos)
    buildfile.prepend(import_line + '\n');
}

void Build2Editor::update_plugins()
{
  for (const string& plugin : configuration.plugins())
  {
    require_repository_for(plugin);
    require_depends_for(plugin, configuration.version());
    if (plugin.substr(0, 3) == "lib")
      import_library(plugin, buildfile);
  }
}

void Build2Editor::append_definition_export(const string& definition)
{
  buildfile.use_symbol("cxx.export.poptions =.*");
  buildfile.insert(" \"-D" + definition + '"', -1);
}

void Build2Editor::remove_definition_export(const string& definition)
{
  auto position = buildfile.find("cxx.export.poptions =");
  auto end_position = buildfile.find("\n", position);

  if (position != string::npos)
  {
    const string_view poptions(buildfile.view().substr(position, end_position - position));
    const string token = ' ' + definition;
    auto result = poptions.find(token.c_str());

    if (result != string::npos)
      buildfile.erase(position + result, token.length());
  }
}

static string definition_line(const set<string>& definitions)
{
  ostringstream stream;

  stream << "cxx.poptions +=";
  for (const string& definition : definitions)
    stream << ' ' << quoted("-D" + definition);
  stream << '\n';
  return stream.str();
}

void Build2Editor::add_definitions(const set<string>& definitions)
{
  buildfile.prepend(definition_line(definitions));
  for (const string& definition : definitions)
    append_definition_export(definition);
}

void Build2Editor::remove_definitions(const set<string>& definitions)
{
  string line = definition_line(definitions);
  auto position = buildfile.find(line);
  auto end_position = buildfile.find("\n", position);

  if (position != string::npos)
    buildfile.erase(position, end_position);
  for (const string& definition : definitions)
    remove_definition_export(definition);
}

void Build2Editor::add_dependency(const string& name, const string& category)
{
  if (category == "package")
  {
    require_depends_for(name);
  }
  else if (category == "dependencies")
  {
    require_depends_for(name);
    import_library(name, buildfile);
  }
  else if (category == "tests_dependencies")
  {
    require_depends_for(name); // only for tests ?
    import_library(name, test_buildfile);
  }
}

void Build2Editor::set_config_variable(const string& name, const string& type, const string& value)
{
  const string prefix = "config [" + type + "] " + name + " =";
  auto position = root.find(prefix);
  auto position_end = root.find("\n", position);

  if (position != string::npos)
    root.erase(position, position_end + 1);
  root.prepend(prefix + ' ' + value + '\n');
}
