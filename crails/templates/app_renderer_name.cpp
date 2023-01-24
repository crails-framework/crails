#include "app_renderer_name.hpp"
#include "../project_configuration.hpp"

using namespace std;
using namespace Crails;

string app_renderer_filename(const ProjectConfiguration& configuration, const string& renderer)
{
  return naming_convention.filenames(configuration.project_name()) + "_" + renderer + "_renderer";
}

string app_renderer_classname(const ProjectConfiguration& configuration, const string& renderer)
{
  return naming_convention.classnames(app_renderer_filename(configuration, renderer));
}
