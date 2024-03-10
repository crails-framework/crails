#include "plugin.hpp"
#include <iostream>
#include "../../file_renderer.hpp"
#include "../../file_editor.hpp"

using namespace std;

SyncPlugin::SyncPlugin()
{
  add_command("install",  []() { return make_shared<SyncPlugin::Installer>(); });
  add_command("disable",  []() { return make_shared<SyncPlugin::Disabler>(); });
}

int SyncPlugin::Installer::run()
{
  FileRenderer    renderer;
  MainCppEditor   main_cpp("app/main.cpp");
  CMakeFileEditor cmakefile(configuration);

  main_cpp.load_file();
  main_cpp.add_to_main_function("SingletonInstantiator<Sync::Channels> sync_channels;\n");
  main_cpp.add_include("crails/sync/channels.hpp");
  configuration.add_plugin("libcrails-sync");
  cmakefile.load_file();
  cmakefile.update_plugins();
  renderer.generate_file("config/sync.cpp");
  configuration.save();
  cmakefile.save_file();
  main_cpp.save_file();
  return 0;
}

int SyncPlugin::Disabler::run()
{
  configuration.remove_plugin("libcrails-sync");
  configuration.save();
  if (filesystem::remove("config/sync.cpp"))
    cout << "[FILE] Removed file config/sync.cpp" << endl;
  cout << "(i) Don't forget to remove the SingletonInstantiator<Sync::MailServers> in app/main.cpp" << endl;
  return 0;
}

void render_sync_config_sync_cpp(const Crails::Renderer&, Crails::RenderTarget& target, Crails::SharedVars&)
{
  target.set_body(std::string(
"#include <crails/sync/task.hpp>\n"
"#include <crails/getenv.hpp>\n\n"
"using namespace Crails;\n\n"
"const std::string    Sync::Task::Settings::hostname = Crails::getenv(\"APPLICATION_HOST\", \"0.0.0.0\");\n"
"const unsigned short Sync::Task::Settings::port = Crails::getenv_as<unsigned short>(\"APPLICATION_PORT\", 3001);\n"
"const bool           Sync::Task::Settings::ssl = false;\n"));
}
