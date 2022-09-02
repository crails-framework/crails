#pragma once
#include <crails/cli/scaffold_model.hpp>
#include <boost/process.hpp>
#include "../project_configuration.hpp"
#include <algorithm>
#include <iostream>

class ResourceScaffold : public Crails::ScaffoldModel
{
  ProjectConfiguration configuration;
  std::vector<std::string> formats{"html"};
  std::string model_classname;
  std::string target_folder = "app";
public:
  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("model,m",    boost::program_options::value<std::string>(), "model's classname")
      ("target,t",   boost::program_options::value<std::string>(), "target view folder (defaults to `app`)")
      ("format,f",   boost::program_options::value<std::vector<std::string>>(), "views format (html, json)")
      ("property,p", boost::program_options::value<std::vector<std::string>>()->multitoken(), "properties, such as: -p std::string-name -p 'unsigned int-age'");
  }

  int create(boost::program_options::variables_map& options) override
  {
    if (!options.count("model"))
    {
      std::cerr << "Option --model required" << std::endl;
      return -1;
    }
    model_classname = options["model"].as<std::string>();
    if (options.count("target"))
      target_folder = options["target"].as<std::string>();
    if (options.count("format"))
      formats = options["format"].as<std::vector<std::string>>();
    return scaffold_model(options) == 0
        && scaffold_views(options) == 0
        && scaffold_controller(options) == 0
      ? 0 : -1;
  }

private:
  int scaffold_model(boost::program_options::variables_map& options)
  {
    boost::process::child process(scaffold_command("model", options, true));

    process.wait();
    return process.exit_code();
  }

  int scaffold_views(boost::program_options::variables_map& options)
  {
    for (const std::string& format : formats)
    {
      boost::process::child process(scaffold_command("view", options, true) + "  --format " + format);

      process.wait();
      if (process.exit_code() != 0)
        return process.exit_code();
    }
    return 0;
  }

  int scaffold_controller(boost::program_options::variables_map& options)
  {
    std::string mode = std::find(formats.begin(), formats.end(), std::string("html")) != formats.end() ? "resource" : "crud";
    boost::process::child process(scaffold_command("controller", options, false) + "  --mode " + mode);

    process.wait();
    return process.exit_code();
  }

  std::string scaffold_command(const std::string& type, boost::program_options::variables_map& options, bool with_properties = false)
  {
    std::stringstream stream;

    stream << configuration.crails_bin_path() << "/crails"
           << " scaffold " << type
           << " --model \"" << model_classname << '"'
           << " --target \"" << target_folder << '/' << Crails::pluralize(type) << '"';
    if (with_properties)
      stream << property_option_string(options);
    std::cout << "[RUN] " << stream.str() << std::endl;
    return stream.str();
  }

  std::string property_option_string(boost::program_options::variables_map& options)
  {
    std::stringstream option_stream;

    if (options.count("property"))
    {
      for (const std::string& property : options["property"].as<std::vector<std::string>>())
        option_stream << " -p \"" << property << '"';
    }
    return option_stream.str();
  }
};
