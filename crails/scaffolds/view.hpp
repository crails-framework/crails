#pragma once
#include "scaffold_model.hpp"
#include "../file_renderer.hpp"
#include <crails/utils/string.hpp>
#include <iostream>

class ViewScaffold : public ScaffoldModel
{
  FileRenderer renderer;
  std::string format = "html";
  std::string model_classname;
  std::string model_header;
  std::string target_folder;
  std::string view_path;
  std::map<std::string,std::string> properties;
public:
  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("model,m",   boost::program_options::value<std::string>(), "model's classname")
      ("header,i", boost::program_options::value<std::string>(), "path to the model header (defaults to `app/models/{classname}.hpp`)")
      ("target,t", boost::program_options::value<std::string>(), "target view folder (defaults to `app/views`)")
      ("view-path,v", boost::program_options::value<std::string>(), "view path (defaults to classname)")
      ("format,f", boost::program_options::value<std::string>(), "views format (html, json)")
      ("property,p", boost::program_options::value<std::vector<std::string>>(), "properties, such as: -p std::string-name -p 'unsigned int-age'");
  }

  int create(boost::program_options::variables_map& options) override
  {
    if (options.count("model"))
      model_classname = options["model"].as<std::string>();
    else
    {
      std::cerr << "Option --name required" << std::endl;
      return -1;
    }
    if (options.count("format"))
      format = options["format"].as<std::string>();
    if (options.count("header"))
      model_header = options["header"].as<std::string>();
    else
      model_header = "app/models/" + Crails::underscore(model_classname) + ".hpp";
    if (options.count("view-path"))
      view_path = options["view-path"].as<std::string>();
    else
      view_path = Crails::underscore(model_classname);
    if (options.count("target"))
      target_folder = options["target"].as<std::string>() + '/' + view_path;
    else
      target_folder = "app/views/" + view_path;
    if (options.count("property"))
    {
      for (const std::string& property_decl : options["property"].as<std::vector<std::string>>())
      {
        auto parts = Crails::split(property_decl, '-');
        properties.emplace(*parts.rbegin(), *parts.begin());
      }
    }
    renderer.vars["classname"]  = model_classname;
    renderer.vars["header"]     = model_header;
    renderer.vars["properties"] = &properties;
    renderer.vars["view_path"]  = view_path;
    renderer.vars["route"]      = Crails::underscore(model_classname);
    if (format == "html")
      create_html();
    else if (format == "json")
      create_json();
    else
    {
      std::cerr << "Unknown format " << format << std::endl;
      return -1;
    }
    return 0;
  }

private:
  int create_html()
  {
    renderer.generate_file("scaffolds/view_index.html", target_folder + "/index.html");
    renderer.generate_file("scaffolds/view_show.html",  target_folder + "/show.html");
    renderer.generate_file("scaffolds/view_edit.html",  target_folder + "/edit.html");
    renderer.generate_file("scaffolds/view_new.html",   target_folder + "/new.html");
    renderer.generate_file("scaffolds/view_form.html",  target_folder + "/form.html");
    return 0;
  }

  int create_json()
  {
    renderer.generate_file("scaffolds/view_index.json", target_folder + "/index.json");
    renderer.generate_file("scaffolds/view_show.json",  target_folder + "/show.json");
    return 0;
  }
};
