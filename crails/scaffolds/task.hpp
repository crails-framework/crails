#pragma once
#include <crails/cli/scaffold_model.hpp>
#include "../file_renderer.hpp"
#include "../file_editor.hpp"
#include "../project_configuration.hpp"
#include <crails/utils/string.hpp>

class TaskScaffold : public ScaffoldModel
{
  ProjectConfiguration configuration;
  FileRenderer renderer;
  std::string task_name;
  std::string target_path;
public:
  TaskScaffold()
  {
    configuration.initialize();
  }

  void options_description(boost::program_options::options_description& desc) const override
  {
    desc.add_options()
      ("name,n",   boost::program_options::value<std::string>(), "task's name");
  }

  int create(boost::program_options::variables_map& options)
  {
    CMakeFileEditor cmakefile(configuration);

    if (!options.count("name"))
    {
      std::cerr << "Option --name required" << std::endl;
      return -1;
    }
    task_name = options["name"].as<std::string>();
    target_path = "tasks/" + task_name;
    renderer.vars["task_name"] = task_name;
    renderer.generate_file("scaffolds/task/CMakeLists.txt", target_path + "/CMakeLists.txt");
    renderer.generate_file("scaffolds/task/main.cpp", target_path + "/main.cpp");
    cmakefile.add_task(task_name);
    cmakefile.save_file();
    return 0;
  }
};
