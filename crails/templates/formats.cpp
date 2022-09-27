#include "formats.hpp"
#include <crails/utils/string.hpp>
#include <iostream>
#include "../file_editor.hpp"
#include <crails/utils/semantics.hpp>

using namespace std;

void TemplateFormatsManager::options_description(boost::program_options::options_description& desc) const
{
  desc.add_options()
    ("add,a",    boost::program_options::value<string>(), "list of renderers to add (ex: html,json)")
    ("remove,r", boost::program_options::value<string>(), "list of renderers to remove");
}

class RendererConfigEditor : public CppFileEditor
{
public:
  RendererConfigEditor() : CppFileEditor("config/renderers.cpp") {}

  std::string renderer_line(const std::string& name) const
  {
    return "renderers.push_back(new " + Crails::camelize(name) + "Renderer);";
  }

  void add_renderer(const std::string& name)
  {
    add_include("crails/renderers/" + name + "_renderer.hpp");
    use_symbol("Append renderers");
    insert(renderer_line(name));
  }

  void remove_renderer(const std::string& name)
  {
    string line = renderer_line(name), include = "#include \"crails/renderers/" + name + "_renderer.hpp\"\n";
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
  RendererConfigEditor config_renderers;

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
    if (configuration.toolchain() == "cmake")
    {
      CMakeFileEditor cmakefile(configuration);

      cmakefile.load_file();
      cmakefile.update_plugins();
      cmakefile.save_file();
    }
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
