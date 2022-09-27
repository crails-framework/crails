#include "plugin.hpp"
#include "../../file_editor.hpp"
#include "../../file_renderer.hpp"
#include <filesystem>

using namespace std;

SentryPlugin::SentryPlugin()
{
  add_command("install", []() { return make_shared<SentryPlugin::Installer>(); });
  add_command("disable", []() { return make_shared<SentryPlugin::Disabler>(); });
}

int SentryPlugin::Installer::run()
{
  FileRenderer         renderer;
  CMakeFileEditor      cmakefile(configuration);
  RequestPipeCppEditor request_pipe_cpp;

  configuration.add_plugin("libcrails-sentry");
  configuration.save();
  cmakefile.load_file();
  cmakefile.update_plugins();
  cmakefile.save_file();
  request_pipe_cpp.load_file();
  request_pipe_cpp.add_include("crails/sentry.hpp");
  request_pipe_cpp.add_include("crails/environment.hpp");
  request_pipe_cpp.add_to_request_pipe(
    "if (Crails::environment == Crails::Production)\n"
    "  {\n"
    "    exception_catcher.add_exception_catcher<std::exception&>(\n"
    "      [this](Crails::Context& context, const std::exception& error)\n"
    "      {\n"
    "        std::stringstream stream;\n"
    "        Sentry::capture_exception(context.params.as_data(), error);\n"
    "        stream << boost_ext::trace(error);\n"
    "        exception_catcher.default_exception_handler(context, typeid(error).name(), error.what(), stream.str());\n"
    "      }\n"
    "    );\n"
    "  }\n"
  );
  request_pipe_cpp.save_file();
  renderer.generate_file("config/sentry.cpp");
  return 0;
}

int SentryPlugin::Disabler::run()
{
  CMakeFileEditor cmakefile(configuration);

  configuration.remove_plugin("libcrails-sentry");
  configuration.save();
  if (filesystem::remove("config/sentry.cpp"))
    cout << "[FILE] Removed file config/sidekix.cpp" << endl;
  cout << "(i) Don't forget to remove your custom exception handlers from config/request_pipe.cpp" << endl;
  return 0;
}
