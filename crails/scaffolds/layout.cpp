#include "layout.hpp"
#include <crails/cli/process.hpp>
#include <iostream>

using namespace std;

const std::map<std::string, LayoutScaffold::initializer> LayoutScaffold::initializers = {
  {"bootstrap", &LayoutScaffold::create_bootstrap_layout},
  {"materialize", &LayoutScaffold::create_materialize_layout},
  {"bare", &LayoutScaffold::create_bare_layout}
};

void render_scaffolds_layout_bootstrap_html(const Crails::Renderer&, Crails::RenderTarget& target, Crails::SharedVars&)
{
  target.set_body(string(
    "#include \"lib/assets.hpp\"\n\n"
    "const char* @yield = nullptr;\n"
    "// END LINKING\n"
    "<html>\n"
    "  <head>\n"
    "    <meta charset=\"utf-8\" />\n"
    "    <%= tag(\"link\", {{\"rel\",\"stylesheet\"},{\"href\", Assets::stylesheets_bootstrap_bootstrap_scss}}) %>\n"
    "  </head>\n"
    "  <body>\n"
    "    <div class=\"container-fluid\">\n"
    "      <% if (yield != nullptr) do %>\n"
    "        <%= yield %>\n"
    "      <% end %>\n"
    "    </div>\n"
    "  </body>\n"
    "</html>"));
}

void render_scaffolds_layout_materialize_html(const Crails::Renderer&, Crails::RenderTarget& target, Crails::SharedVars&)
{
  target.set_body(std::string(
    "const char* @yield = nullptr;\n"
    "// END LINKING\n"
    "<html>\n"
    "  <head>\n"
    "    <meta charset=\"utf-8\" />\n"
    "    <link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/css/materialize.min.css\">\n"
    "    <script src=\"https://cdnjs.cloudflare.com/ajax/libs/materialize/1.0.0/js/materialize.min.js\"></script>\n"
    "  </head>\n"
    "  <body>\n"
    "    <% if (yield != nullptr) do %>\n"
    "      <%= yield %>\n"
    "    <% end %>\n"
    "  </body>\n"
    "</html>"));
}

void render_scaffolds_layout_bare_html(const Crails::Renderer&, Crails::RenderTarget& target, Crails::SharedVars&)
{
  target.set_body(std::string(
    "const char* @yield = nullptr;\n"
    "// END LINKING\n"
    "<html>\n"
    "  <head>\n"
    "    <meta charset=\"utf-8\" />\n"
    "  </head>\n"
    "  <body>\n"
    "    <% if (yield != nullptr) do %>\n"
    "      <%= yield %>\n"
    "    <% end %>\n"
    "  </body>\n"
    "</html>"));
}

int LayoutScaffold::create_layout_from_toolkit()
{
  auto it = initializers.find(toolkit);

  if (it != initializers.end())
  {
    initializer method = it->second;
    return (this->*method)();
  }
  std::cerr << "Unknown toolkit " << toolkit << std::endl;
  return -1;
}

int LayoutScaffold::create_bare_layout()
{
  renderer.generate_file("scaffolds/layouts/bare.html", target_folder + "/views/layouts/" + name + ".html");
  return 0;
}

int LayoutScaffold::create_materialize_layout()
{
  renderer.generate_file("scaffolds/layouts/materialize.html", target_folder + "/views/layouts/" + name + ".html");
  return 0;
}

int LayoutScaffold::create_bootstrap_layout()
{
  const std::string asset_dir = "app/assets";

  renderer.generate_file("scaffolds/layouts/bootstrap.html", target_folder + "/views/layouts/" + name + ".html");
  if (!boost::filesystem::exists(asset_dir + "/stylesheets/bootstrap/bootstrap.scss"))
    return download_bootstrap();
  return 0;
}

static bool command_exists(const std::string& command)
{
  if (Crails::which(command).length() == 0)
  {
    std::cout << "Requires `" << command << "` to be installed." << std::endl;
    return false;
  }
  return true;
}

static bool download_archive(const std::string& url, std::function<void()> callback)
{
  boost::filesystem::path tmp_file = boost::filesystem::unique_path();
  boost::process::child download_process("curl -o \"" + tmp_file.string() + "\" -L  \"" + url + '"');

  download_process.wait();
  if (download_process.exit_code() == 0)
  {
    boost::process::child extract_process("unzip \"" + tmp_file.string() + '"');

    extract_process.wait();
    std::remove(tmp_file.string().c_str());
    if (extract_process.exit_code() == 0)
    {
      callback();
      return true;
    }
  }
  return false;
}

int LayoutScaffold::download_bootstrap()
{
  boost::filesystem::path css_output_dir = "app/assets/stylesheets/bootstrap";
  boost::filesystem::path font_output_dir = "app/assets/fonts";

  if (command_exists("curl") && command_exists("unzip"))
  {
    const std::string css_version = "5.0.2";
    const std::string css_url = "https://github.com/twbs/bootstrap/archive/v" + css_version + ".zip";
    const std::string icon_version = "1.9.1";
    const std::string icon_url = "https://github.com/twbs/icons/releases/download/v" + icon_version + "/bootstrap-icons-" + icon_version + ".zip";
    bool success;

    success = download_archive(css_url, [&]()
    {
      boost::filesystem::create_directories(css_output_dir.parent_path());
      boost::filesystem::rename("bootstrap-" + css_version + "/scss", css_output_dir);
      boost::filesystem::remove_all("bootstrap-" + css_version);
    });
    if (!success) return -2;
    success = download_archive(icon_url, [&]()
    {
      boost::filesystem::create_directories(font_output_dir);
      boost::filesystem::rename("bootstrap-" + icon_version + "/fonts/bootstrap-icons.woff",  font_output_dir.string() + "/bootstrap-icons.woff");
      boost::filesystem::rename("bootstrap-" + icon_version + "/fonts/bootstrap-icons.woff2", font_output_dir.string() + "/bootstrap-icons.woff2");
      boost::filesystem::rename("bootstrap-" + icon_version + "/bootstrap-icons.scss",        css_output_dir.string() + "/icons.scss");
      boost::filesystem::remove_all("bootstrap-" + icon_version);
    });
    return !success ? -2 : 0;
  }
  return -1;
}
