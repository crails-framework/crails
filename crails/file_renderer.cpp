#include "file_renderer.hpp"
#include <boost/filesystem.hpp>
#include <crails/cli/filesystem.hpp>
#include <iostream>

using namespace std;

bool FileRenderer::require_folder(boost::filesystem::path path)
{
  boost::filesystem::path directory(path.parent_path());

  if (directory.string().length() > 0 && !boost::filesystem::is_directory(directory))
  {
    boost::system::error_code ec;

    boost::filesystem::create_directories(directory, ec);
    if (ec)
    {
      cout << "[DIR] Failed to create directory " << directory.generic_string() << ": " << ec.message() << endl;
      return false;
    }
    cout << "[DIR] Created directory " << directory.generic_string() << endl;
  }
  return true;
}

bool FileRenderer::render_file(string_view template_name, boost::filesystem::path path)
{
  Crails::RenderFile render_target;

  if (renderer.can_render("", template_name.data()))
  {
    if (require_folder(path))
    {
      render_target.open(path.string());
      renderer.render_template(template_name.data(), render_target, vars);
      std::cout << "[FILE] `" << path.string() << "` generated." << std::endl;
      return true;
    }
  }
  else
    cout << "[FILE] `" << path.string() << "` could not be generated: template not found." << endl;
  return false;
}

bool FileRenderer::generate_file(string_view template_name, string_view filepath)
{
  boost::filesystem::path path(filepath.data());

  return (should_overwrite || Crails::prompt_write_file(path.string()))
      && render_file(template_name, path);
}
