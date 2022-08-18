#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectAppRoutesCpp : public Crails::Template
{
public:
  ProjectAppRoutesCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#include <crails/router.hpp>\n\nvoid Crails::Router::initialize(void)\n{\n  // Append routes here (do not remove this line)\n}\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_project_app_routes_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ProjectAppRoutesCpp(renderer, vars).render();
}