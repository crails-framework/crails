#include "formats.hpp"
#include <crails/utils/string.hpp>
#include <iostream>
#include "../file_editor.hpp"
#include <crails/utils/semantics.hpp>
#include "app_renderer_name.hpp"

using namespace std;

void TemplateFormatsManager::options_description(boost::program_options::options_description& desc) const
{
  desc.add_options()
    ("add,a",    boost::program_options::value<string>(), "list of renderers to add (ex: html,json)")
    ("remove,r", boost::program_options::value<string>(), "list of renderers to remove");
}

class RendererConfigEditor : public CppFileEditor
{
  const ProjectConfiguration& configuration;
public:
  RendererConfigEditor(const ProjectConfiguration& configuration) : CppFileEditor("config/renderers.cpp"), configuration(configuration) {}

  std::string renderer_line(const std::string& name) const
  {
    return "  renderers.push_back(make_unique<" + app_renderer_classname(configuration, name) + ">());\n";
  }

  std::string include_path(const std::string& name) const
  {
    return "autogen/renderers/" + app_renderer_filename(configuration, name) + ".hpp";
  }

  void add_renderer(const std::string& name)
  {
    add_include(include_path(name));
    use_symbol("Append renderers");
    insert(renderer_line(name));
  }

  void remove_renderer(const std::string& name)
  {
    string line = renderer_line(name);
    string include = "#include \"" + include_path(name) + "\"\n";
    size_t position_line = contents.find(line);
    size_t position_include = contents.find(include);

    if (position_line != string::npos)
      contents.erase(position_line, line.length());
    if (position_include != string::npos)
      contents.erase(position_include, include.length());
  }
};

int TemplateFormatsManager::run()
{
  RendererConfigEditor config_renderers(configuration);

  if (!options.count("add") && !options.count("remove"))
  {
    cout << Crails::join(configuration.renderers(), ',') << endl;
    return 0;
  }

  config_renderers.load_file();

  if (options.count("add"))
  {
    if (!configuration.has_plugin("libcrails-templates"))
      configuration.add_plugin("libcrails-templates");
    for (const auto& entry : Crails::split(options["add"].as<string>(), ','))
    {
      configuration.add_renderer(entry);
      configuration.add_plugin("libcrails-" + entry + "-views");
      config_renderers.add_renderer(entry);
    }
    configuration.update_plugins();
  }
  if (options.count("remove"))
  {
    for (const auto& entry : Crails::split(options["remove"].as<string>(), ','))
    {
      configuration.remove_renderer(entry);
      config_renderers.remove_renderer(entry);
    }
  }

  if (configuration.renderers().size() == 0)
    configuration.remove_plugin("libcrails-templates");

  configuration.save();
  config_renderers.save_file();
  return 0;
}
