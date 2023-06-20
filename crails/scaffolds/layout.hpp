#pragma once
#include <crails/cli/scaffold_model.hpp>
#include "../file_renderer.hpp"
#include <boost/process.hpp>

class LayoutScaffold : public Crails::ScaffoldModel
{
  FileRenderer renderer;
  std::string name = "application";
  std::string target_folder = "app";
  std::string toolkit = "bootstrap";
public:
  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("name,n",    boost::program_options::value<std::string>(), "layout's name (defaults to `application`)")
      ("target,t",  boost::program_options::value<std::string>(), "target view folder (defaults to `app`)")
      ("toolkit,k", boost::program_options::value<std::vector<std::string>>(), "toolkit to use (defaults to bootstrap)");
  }

  int create(boost::program_options::variables_map& options) override
  {
    if (options.count("toolkit"))
      toolkit = options["toolkit"].as<std::string>();
    if (options.count("target"))
      target_folder = options["target"].as<std::string>();
    if (options.count("name"))
      name = options["name"].as<std::string>();
    return create_layout_from_toolkit();
  }
  typedef int (LayoutScaffold::*initializer)();
private:
  static const std::map<std::string, initializer> initializers;

  int create_layout_from_toolkit();
  int create_bootstrap_layout();
  int create_bare_layout();
  int create_materialize_layout();
  int download_bootstrap();
};


