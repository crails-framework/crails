#include "plugin.hpp"
#include <filesystem>
#include <iostream>
#include <crails/utils/random_string.hpp>
#include "../../file_renderer.hpp"
#include "../../file_editor.hpp"

using namespace std;

void require_encrypt_plugin(ProjectConfiguration& configuration)
{
  if (!configuration.has_plugin("libcrails-encrypt"))
  {
    MainCppEditor main_cpp("app/main.cpp");

    configuration.add_plugin("libcrails-encrypt");
    main_cpp.load_file();
    main_cpp.add_include("crails/cipher.hpp");
    main_cpp.add_to_main_function("Cipher::initialize();\n");
    main_cpp.save_file();
  }
}

void require_cookie_plugin(ProjectConfiguration& configuration)
{
  if (!configuration.has_plugin("libcrails-cookies"))
  {
    FileRenderer renderer;

    require_encrypt_plugin(configuration);
    configuration.add_plugin("libcrails-cookies");
    renderer.vars["session_store"] = "CookieStore";
    renderer.generate_file("config/cookies.cpp");
    renderer.generate_file("config/session_store.cpp");
  }
}

SigninPlugin::SigninPlugin()
{
  add_command("install",  []() { return make_shared<SigninPlugin::Installer>(); });
  add_command("disable",  []() { return make_shared<SigninPlugin::Disabler>(); });
}

int SigninPlugin::Installer::run()
{
  FileRenderer    renderer;
  CMakeFileEditor cmakefile(configuration);

  cmakefile.load_file();
  require_cookie_plugin(configuration);
  configuration.add_plugin("libcrails-cookies");
  configuration.add_plugin("libcrails-signin");
  configuration.save();
  cmakefile.update_plugins();
  cmakefile.save_file();
  renderer.generate_file("config/signin.cpp");
  return 0;
}

int SigninPlugin::Disabler::run()
{
  configuration.remove_plugin("libcrails-signin");
  configuration.save();
  if (filesystem::remove("config/signin.cpp"))
    cout << "[FILE] Removed file config/signin.cpp" << endl;
  return 0;
}

string render_signin_config_signin_cpp(const Crails::Renderer*, Crails::SharedVars&)
{
  const std::string salt_charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  return
"#include <crails/signin/model.hpp>\n"
"#include <crails/signin/password.hpp>\n"
"#include <chrono>\n\n"
"using namespace Crails;\n"
"using namespace std;\n\n"
"const time_t AuthenticableModel::session_duration = chrono::seconds(24h).count();\n\n"
"const string Password::encrypt_key = \"" + Crails::generate_random_string(16) + "\";\n"
"const string Password::encrypt_salt = \"" + Crails::generate_random_string(salt_charset, 8) + "\";\n";
}
