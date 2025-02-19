#include "plugin.hpp"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <crails/datatree.hpp>
#include <crails/render_file.hpp>
#include "../../file_renderer.hpp"
#include "../../file_editor.hpp"

I18nPlugin::I18nPlugin()
{
  add_command("install", []() { return std::make_shared<I18nPlugin::Installer>(); });
  add_command("disable", []() { return std::make_shared<I18nPlugin::Disabler>(); });
}

int I18nPlugin::Installer::run()
{
  FileRenderer renderer;
  MainCppEditor main_cpp("app/main.cpp");

  main_cpp.load_file();
  main_cpp.add_to_main_function("SingletonInstantiator<ApplicationI18n> translations;");
  main_cpp.add_include("app/config/i18n.hpp");
  renderer.generate_file("app/config/i18n.hpp");
  renderer.generate_file("app/config/i18n.cpp");
  configuration.add_plugin("libcrails-i18n");
  configuration.update_plugins();
  configuration.save();
  main_cpp.save_file();
  return 0;
}

int I18nPlugin::Disabler::run()
{
  configuration.remove_plugin("libcrails-i18n");
  configuration.save();
  if (std::filesystem::remove("app/config/i18n.cpp"))
    cout << "[FILE] Removed file app/config/i18n.cpp" << endl;
  return 0;
}

static std::string locale_from_filename(std::string filename)
{
  string locale;
  filename = filename.substr(0, filename.length() - std::string(".json").length());
  for (auto it = filename.rbegin() ; it != filename.rbegin() && *it != '.' ; ++it)
    locale.insert(locale.begin(), *it);
  return locale;
}

static void collect_translations_from(const std::filesystem::path path, std::map<std::string, DataTree>& translations)
{
  if (std::filesystem::is_directory(path))
  {
    std::filesystem::recursive_directory_iterator dir(path);

    for (auto& entry : dir)
    {
      if (std::filesystem::is_directory(entry.path()))
        collect_translations_from(entry.path(), translations);
      else if (entry.path().string().find(".json") != std::string::npos)
      {
        std::string locale = locale_from_filename(entry.path().filename().string());
        auto translations_data = translations.find(locale);
        DataTree filedata;

        if (translations_data == translations.end())
          translations_data = translations.emplace(locale, DataTree()).first;
        filedata.from_json_file(std::filesystem::canonical(entry.path()).string());
        translations_data->second.as_data().merge(filedata.as_data());
      }
    }
  }
}

bool I18nPlugin::build(const ProjectConfiguration& configuration)
{
  std::map<std::string, DataTree> translations;

  collect_translations_from("app/translations", translations);
  for (const string& mod : configuration.modules())
    collect_translations_from("modules/" + mod + "/translations", translations);
  for (const auto& entry : translations)
  {
    std::string filepath = "app/autogen/assets/translations." + entry.first + ".json";
    Crails::RenderFile output;

    std::filesystem::create_directories("app/autogen/assets");
    if (output.open(filepath))
    {
      std::string json = entry.second.to_json();

      output.set_body(json.c_str(), json.length());
    }
    else
    {
      std::cerr << "[crails-i18n] cannot create file `" << filepath << '`' << std::endl;
      return false;
    }
  }
  return true;
}
