#include "layout.hpp"
#include <crails/cli/process.hpp>
#include <iostream>

using namespace std;

std::string render_scaffolds_layout_bootstrap_html(const Crails::Renderer*, Crails::SharedVars& vars)
{
  return
    "#include \"lib/assets.hpp\"\n\n"
    "const char* @yield = nullptr;\n"
    "// END LINKING\n"
    "<html>\n"
    "  <head>\n"
    "    <%= tag(\"link\", {{\"rel\",\"stylesheet\"},{\"href\", Assets::stylesheets_bootstrap_bootstrap_scss}}) %>\n"
    "  </head>\n"
    "  <body>\n"
    "    <div class=\"container-fluid\">\n"
    "      <% if (yield != nullptr) do %>\n"
    "        <%= yield %>\n"
    "      <% end %>\n"
    "    </div>\n"
    "  </body>\n"
    "</html>";
}

std::string render_scaffolds_layout_bare_html(const Crails::Renderer*, Crails::SharedVars& vars)
{
  return
    "const char* @yield = nullptr;\n"
    "// END LINKING\n"
    "<html>\n"
    "  <head>\n"
    "  </head>\n"
    "  <body>\n"
    "    <% if (yield != nullptr) do %>\n"
    "      <%= yield %>\n"
    "    <% end %>\n"
    "  </body>\n"
    "</html>";
}

int LayoutScaffold::create_bare_layout()
{
  renderer.generate_file("scaffolds/layouts/bare.html", target_folder + "/views/layouts/" + name + ".html");
  return 0;
}

int LayoutScaffold::create_bootstrap_layout()
{
  const std::string bootstrap_output_dir = "app/assets/stylesheets/bootstrap";

  renderer.generate_file("scaffolds/layouts/bootstrap.html", target_folder + "/views/layouts/" + name + ".html");
  if (!boost::filesystem::exists(bootstrap_output_dir + "/bootstrap.scss"))
    return download_bootstrap(bootstrap_output_dir);
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

int LayoutScaffold::download_bootstrap(const boost::filesystem::path& output_dir)
{
  if (command_exists("curl") && command_exists("unzip"))
  {
    const std::string version = "5.0.2";
    const std::string url = "https://github.com/twbs/bootstrap/archive/v" + version + ".zip";
    boost::filesystem::path tmp_file = boost::filesystem::unique_path();
    boost::process::child download_process("curl -o \"" + tmp_file.string() + "\" -L  \"" + url + '"');

    download_process.wait();
    if (download_process.exit_code() == 0)
    {
      boost::process::child extract_process("unzip \"" + tmp_file.string() + '"');

      extract_process.wait();
      if (extract_process.exit_code() == 0)
      {
        boost::filesystem::create_directories(output_dir.parent_path());
        boost::filesystem::rename("bootstrap-" + version + "/scss", output_dir);
        return 0;
      }
    }
  }
  return -1;
}
