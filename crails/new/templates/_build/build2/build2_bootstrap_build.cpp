#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_Build2BuildBootstrapBuild : public Crails::Template
{
public:
  render_Build2BuildBootstrapBuild(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    project_name(Crails::cast< std::string >(vars, "project_name"))
  {}

  void render()
  {
ecpp_stream << "project = lib" << ( project_name );
  ecpp_stream << "\n\nusing version\nusing config\nusing test\nusing install\nusing dist\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string project_name;
};

void render_build2_build_bootstrap_build(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_Build2BuildBootstrapBuild(renderer, target, vars).render();
}