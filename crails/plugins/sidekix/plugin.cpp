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
  auto            toolchain = configuration.toolchain_editor();
  MainCppEditor   main_cpp("app/main.cpp");

  configuration.add_plugin("libcrails-sidekix");
  configuration.save();
  toolchain->load_file();
  toolchain->update_plugins();
  toolchain->add_task("sidekix");
  toolchain->save_file();
  main_cpp.load_file();
  main_cpp.add_include("crails/sidekix/process.hpp");
  main_cpp.add_to_main_function("SingletonInstantiator<Sidekix::Process> sidekix(argc, argv);\n");
  main_cpp.save_file();
  renderer.vars["task_name"] = std::string("sidekix");
  renderer.generate_file("config/sidekix.cpp",            "app/config/sidekix.cpp");
  renderer.generate_file("tasks/sidekix/main.cpp",        "exe/sidekix/main.cpp");
  renderer.generate_file("tasks/sidekix/ctpl.h",          "exe/sidekix/ctpl.h");
  renderer.generate_file("tasks/sidekix/sidetasks.hpp",   "exe/sidekix/sidetasks.hpp");
  renderer.generate_file("tasks/sidekix/sidetasks.cpp",   "exe/sidekix/sidetasks.cpp");
  if (configuration.toolchain() == "build2")
    renderer.generate_file("scaffolds/task/buildfile", "exe/sidekix/buildfile");
  else
    renderer.generate_file("scaffolds/task/CMakeLists.txt", "exe/sidekix/CMakeLists.txt");
  return 0;
}

int SidekixPlugin::Disabler::run()
{
  configuration.remove_plugin("libcrails-sidekix");
  configuration.update_plugins();
  configuration.save();
  if (filesystem::remove("config/sidekix.cpp"))
    cout << "[FILE] Removed file config/sidekix.cpp" << endl;
  return 0;
}

int SidekixPlugin::AddTask::run()
{
  FileRenderer renderer;
  CppFileEditor sidetasks_cpp("exe/sidekix/sidetasks.cpp");
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
  renderer.generate_file("tasks/sidekix/sidetask.cpp", "exe/sidekix/sidetasks/" + task_filename + ".cpp");
  return 0;
}
