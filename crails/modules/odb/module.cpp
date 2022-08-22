#include "module.hpp"
#include <algorithm>
#include <iostream>
#include "../../file_renderer.hpp"
#include "../../file_editor.hpp"

using namespace std;

const vector<string> OdbModule::supported_backends{"sqlite","pgsql","mysql","oracle"};

OdbModule::OdbModule()
{
  add_command("install",  []() { return make_shared<OdbInstaller>(); });
  add_command("disable",  []() { return make_shared<OdbDisabler>(); });
  add_command("backends", []() { return make_shared<OdbBackendManager>(); });
}

static void add_database(const ProjectConfiguration& configuration, const std::string& backend)
{
  CppFileEditor databases("config/databases.cpp", "");
  stringstream  desc;
  const std::map<std::string, unsigned short> ports = {{"mysql", 3306}, {"pgsql", 5432}};

  desc << "{\"odb\",{";
  desc << "{\"type\",\" << backend << \"}";
  desc << ",{\"name\", \"" << configuration.variable("name") << "\"}";
  if (backend != "sqlite")
    desc << ",{\"host\",\"127.0.0.1\"}";
  if (ports.find(backend) != ports.end())
    desc << ",{\"port\", static_cast<unsigned int>(" << ports.at(backend) << ")}";
  desc << "}}\n";
  databases.use_symbol("Development, \\{");
  databases.insert(desc.str());
  databases.save_file();
}

int OdbModule::OdbInstaller::run()
{
  list<string>    backends;
  FileRenderer    renderer;
  CMakeFileEditor cmakefile(configuration);

  if (options.count("backends"))
    backends = Crails::split(options["backends"].as<string>(), ',');
  if (!check_backends_validity(backends))
    return -1;
  renderer.vars["task_name"] = string("odb_migrate");
  if (!boost::filesystem::exists("config/databases.cpp"))
    renderer.generate_file("config/databases.cpp");
  renderer.generate_file("config/odb.hpp");
  renderer.generate_file("config/odb.cpp");
  renderer.generate_file("tasks/odb_migrate/main.cpp");
  renderer.generate_file("scaffolds/task/CMakeLists.txt", "tasks/odb_migrate/CMakeLists.txt");
  configuration.remove_module("libcrails-databases"); // must be included after libcrails-odb
  configuration.add_module("libcrails-odb");
  configuration.add_module("libcrails-databases");
  configuration.variable("odb-backends", Crails::join(backends, ','));
  configuration.variable("odb-at-once", "1");
  configuration.variable("odb-default-pointer", "std::shared_ptr");
  configuration.save();
  cmakefile.load_file();
  cmakefile.update_modules();
  cmakefile.add_dependency("odb");
  for (const string& backend : backends)
    cmakefile.add_dependency("odb-" + backend);
  cmakefile.add_task("odb_migrate");
  cmakefile.save_file();
  add_database(configuration, *backends.begin());
  cout << "(i) Don't forget to run `build/tasks/odb_migrate/task -c odb` to create or update your database." << endl;
  return 0;
}

int OdbModule::OdbDisabler::run()
{
  configuration.remove_module("libcrails-odb");
  configuration.save();
  return 0;
}

int OdbModule::OdbBackendManager::run()
{
  CMakeFileEditor cmakefile(configuration);
  auto backends = Crails::split(configuration.variable("odb-backends"), ',');

  if (!options.count("add") && !options.count("remove"))
    cout << configuration.variable("odb-backends") << endl;
  if (options.count("add"))
  {
    auto new_backends = Crails::split(options["add"].as<string>(), ',');

    cmakefile.load_file();
    for (const auto& new_backend : new_backends)
    {
      if (find(backends.begin(), backends.end(), new_backend) == backends.end())
      {
        backends.push_back(new_backend);
        cmakefile.add_dependency("odb-" + new_backend);
      }
    }
    cmakefile.save_file();
  }
  if (options.count("remove"))
  {
    auto remove_backends = Crails::split(options["remove"].as<string>(), ',');
    for (const auto& remove_backend : remove_backends)
    {
      auto it = find(backends.begin(), backends.end(), remove_backend);
      if (it != backends.end()) backends.erase(it);
    }
  }
  if (OdbModule::check_backends_validity(backends))
  {
    configuration.variable("odb-backends", Crails::join(backends, ','));
    configuration.save();
    return 0;
  }
  return -1;
}

bool OdbModule::check_backends_validity(const list<string>& backends)
{
  stringstream supported_values;

  supported_values << "Supported values are: " << Crails::join(OdbModule::supported_backends, ' ');
  if (backends.size() == 0)
  {
    cerr << "Must use at least one backend. " << supported_values.str() << endl;
    return false;
  }
  for (const std::string& backend : backends)
  {
    if (std::find(supported_backends.begin(), supported_backends.end(), backend) == supported_backends.end())
    {
      cerr << "Unknown backend `" << backend << "`. " << supported_values.str() << endl;
      return false;
    }
  }
  return true;
}

std::string render_odb_hpp(const Crails::Renderer*, Crails::SharedVars&)
{
  return
"#pragma once\n"
"#pragma db model version(1,1)\n"
"#pragma db map type(\"INTEGER\\\\[\\\\]\") as(\"TEXT\") to(\"(?)::INTEGER[]\") from(\"(?)::TEXT\")\n";
}

std::string render_odb_cpp(const Crails::Renderer*, Crails::SharedVars&)
{
  return
"#include <crails/odb/transaction.hpp>\n\n"
"const bool Crails::Odb::Transaction::use_session = false;\n";
}
