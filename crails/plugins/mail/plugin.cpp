#include "plugin.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include "../../file_renderer.hpp"
#include "../../file_editor.hpp"

using namespace std;

MailPlugin::MailPlugin()
{
  add_command("install",  []() { return make_shared<MailPlugin::Installer>(); });
  add_command("disable",  []() { return make_shared<MailPlugin::Disabler>(); });
}

int MailPlugin::Installer::run()
{
  FileRenderer    renderer;
  CppFileEditor   main_cpp("app/main.cpp");
  CMakeFileEditor cmakefile(configuration);

  main_cpp.load_file();
  main_cpp.set_prefix_pattern("");
  main_cpp.use_symbol("int\\s*main\\s*\\([^)]+\\)\\s*\\{");
  main_cpp.insert("\n  SingletonInstantiator<MailServers> mail_servers;");
  main_cpp.add_include("crails/mail_servers.hpp");
  configuration.add_plugin("libcrails-mail");
  cmakefile.load_file();
  cmakefile.update_plugins();
  renderer.generate_file("config/mailers.cpp");
  configuration.save();
  cmakefile.save_file();
  main_cpp.save_file();
  return 0;
}

int MailPlugin::Disabler::run()
{
  configuration.remove_plugin("libcrails-mail");
  configuration.save();
  if (filesystem::remove("config/mailers.cpp"))
    cout << "[FILE] Removed file config/mailers.cpp" << endl;
  cout << "(i) Don't forget to remove the SingletonInstantiator<MailServers> in app/main.cpp" << endl;
  return 0;
}

string render_mail_config_mailers_cpp(const Crails::Renderer*, Crails::SharedVars&)
{
  return
"#include <crails/mail_servers.hpp>\n\n"
"using namespace Crails;\n\n"
"const MailServers::List MailServers::servers = {\n"
"  {\n"
"    \"example\", MailServer()\n"
"      .hostname(\"smtp.gmail.com\")\n"
"      .port(465)\n"
"      .tls(true)\n"
"      .use_authentication(true)\n"
"      .authentication_protcol(Smtp::Server::LOGIN)\n"
"      .username(\"roger\")\n"
"      .password(\"secret\")\n"
"  }\n"
"};";
}
