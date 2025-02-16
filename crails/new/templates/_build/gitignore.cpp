#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectGitignore : public Crails::Template
{
public:
  render_ProjectGitignore(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    build_system(Crails::cast< std::string >(vars, "build_system"))
  {}

  void render()
  {
ecpp_stream << "/app/autogen/\n/lib-client/\n/logs/\n/public/assets/\n/docker/build-*\n*.d\n*.o\n";
 if (build_system == "cmake"){
  ecpp_stream << "\n/build/";
 };
  ecpp_stream << "";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string build_system;
};

void render_project_gitignore(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectGitignore(renderer, target, vars).render();
}