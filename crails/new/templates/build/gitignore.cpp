#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectGitignore : public Crails::Template
{
public:
  ProjectGitignore(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "/build/\n/docker/build-*\n/lib/\n/lib-client/\n/logs/\n/package/\n/public/assets/\n*.d\n*.o\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_project_gitignore(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ProjectGitignore(renderer, vars).render();
}