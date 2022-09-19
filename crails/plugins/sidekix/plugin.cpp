#include "plugin.hpp"
#include "../../file_renderer.hpp"
#include "../../file_editor.hpp"
#include <crails/cli/conventions.hpp>

using namespace std;

SidekixPlugin::SidekixPlugin()
{
  add_command("install",  []() { return make_shared<SidekixPlugin::Installer>(); });
  add_command("disable",  []() { return make_shared<SidekixPlugin::Disabler>(); });
  add_command("add-task", []() { return make_shared<SidekixPlugin::AddTask>(); });
}

int SidekixPlugin::Installer::run()
{
  FileRenderer    renderer;
  CMakeFileEditor cmakefile(configuration);
  MainCppEditor   main_cpp("app/main.cpp");

  configuration.add_plugin("libcrails-sidekix");
  configuration.save();
  cmakefile.load_file();
  cmakefile.update_plugins();
  cmakefile.add_task("sidekix");
  cmakefile.save_file();
  main_cpp.load_file();
  main_cpp.add_include("crails/sidekix/process.hpp");
  main_cpp.add_to_main_function("SingletonInstantiator<Sidekix::Process> sidekix;\n");
  main_cpp.save_file();
  renderer.vars["task_name"] = std::string("sidekix");
  renderer.generate_file("config/sidekix.cpp");
  renderer.generate_file("tasks/sidekix/main.cpp");
  renderer.generate_file("tasks/sidekix/ctpl.h");
  renderer.generate_file("tasks/sidekix/sidetasks.hpp");
  renderer.generate_file("tasks/sidekix/sidetasks.cpp");
  renderer.generate_file("scaffolds/task/CMakeLists.txt", "tasks/sidekix/CMakeLists.txt");
  return 0;
}

int SidekixPlugin::Disabler::run()
{
  CMakeFileEditor cmakefile(configuration);

  configuration.remove_plugin("libcrails-sidekix");
  configuration.save();
  if (filesystem::remove("config/sidekix.cpp"))
    cout << "[FILE] Removed file config/sidekix.cpp" << endl;
  return 0;
}

int SidekixPlugin::AddTask::run()
{
  FileRenderer renderer;
  CppFileEditor sidetasks_cpp("tasks/sidekix/sidetasks.cpp");
  std::string task_name, task_key, task_function, task_filename;

  if (!options.count("name"))
  {
    cerr << "missing required --name parameter" << endl;
    return -1;
  }
  task_name = options["name"].as<string>();
  task_function = Crails::naming_convention.functions(task_name);
  task_filename = Crails::naming_convention.filenames(task_name);
  task_key = Crails::naming_convention.keys(task_name);
  sidetasks_cpp.load_file();
  sidetasks_cpp.use_symbol("Sidetask declarations");
  sidetasks_cpp.insert("void " + task_function + "(Params&);\n");
  sidetasks_cpp.use_symbol("Sidetask map");
  sidetasks_cpp.insert("  {\"" + task_key + "\", " + task_function + "},\n");
  sidetasks_cpp.save_file();
  renderer.vars["function_name"] = task_function;
  renderer.generate_file("tasks/sidekix/sidetask.cpp", "tasks/sidekix/sidetasks/" + task_filename + ".cpp");
  return 0;
}
