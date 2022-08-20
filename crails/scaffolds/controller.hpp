#pragma once
#include "scaffold_model.hpp"
#include "../file_renderer.hpp"
#include "../file_editor.hpp"
#include <crails/utils/string.hpp>

class ControllerScaffold : public ScaffoldModel
{
  FileRenderer renderer;
  std::string resource_name;
  std::string classname;
  std::string path_name;
  std::string view_path;
  std::string target_folder = "app/controllers";
  std::string router_path;
public:
  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("name,n",   boost::program_options::value<std::string>(), "classname")
      ("model,m",  boost::program_options::value<std::string>(), "model class to use")
      ("header,i", boost::program_options::value<std::string>(), "model include path (defaults to `app/models/{model}.hpp`)")
      ("target,t", boost::program_options::value<std::string>(), "target folder (defaults to `app/controllers`)")
      ("mode,z",   boost::program_options::value<std::string>(), "generates methods and routes `crud` or `resource`");
  }

  int create(boost::program_options::variables_map& options) override
  {
    if (!options.count("name") && !options.count("model"))
    {
      std::cerr << "Option --name or --model required" << std::endl;
      return -1;
    }
    resource_name = options.count("name") ? options["name"].as<std::string>() : options["model"].as<std::string>();
    classname     = resource_name + "Controller";
    router_path   = Crails::underscore(resource_name);
    path_name     = Crails::underscore(resource_name);
    if (options.count("target"))
      target_folder = options["target"].as<std::string>();
    renderer.vars["classname"]   = classname;
    renderer.vars["router_path"] = router_path;
    renderer.vars["path"]        = path_name;
    renderer.vars["view_path"]   = path_name;
    if (options.count("mode"))
    {
      renderer.vars["crud_scaffold"] = options["mode"].as<std::string>() == "crud";
      renderer.vars["resource_scaffold"] = options["mode"].as<std::string>() == "resource";
    }
    if (options.count("model"))
    {
      std::string model_class = options["model"].as<std::string>();
      renderer.vars["model_class"] = model_class;
      renderer.vars["model_header"] = ("app/models/" + Crails::underscore(model_class) + ".hpp");
      renderer.vars["view_path"] = Crails::underscore(model_class);
      renderer.vars["parent_class"] = std::string("Crails::Odb::Controller<ApplicationController>");
    }
    if (options.count("header"))
      renderer.vars["model_header"] = options["header"].as<std::string>();
    renderer.generate_file(
      "scaffolds/controller.cpp",
      target_folder + '/' + path_name + ".cpp"
    );
    renderer.generate_file(
      "scaffolds/controller.hpp",
      target_folder + '/' + path_name + ".hpp"
    );
    if (options.count("mode"))
      update_router(options);
    return 0;
  }

  void update_router(boost::program_options::variables_map& options)
  {
    CppFileEditor router("app/routes.cpp");

    if (router.load_file() && router.use_symbol("Append routes here \\(do not remove this line\\)"))
    {
      if (options["mode"].as<std::string>() == "crud")
        router.insert("  crud_actions(" + router_path + ", " + classname + ");\n");
      else if (options["mode"].as<std::string>() == "resource")
        router.insert("  resource_actions(" + router_path + ", " + classname + ");\n");
      router.add_include(target_folder + '/' + path_name + ".hpp");
      router.save_file();
    }
    else
      std::cout << "Cannot update app/routes.cpp" << std::endl;
  }
};
