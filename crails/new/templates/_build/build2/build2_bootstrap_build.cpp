#include <crails/template_stream.hpp>
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
    ecpp_stream.reserve(288);
    // BEGIN TEMPLATE BODY
ecpp_stream << "project = lib" << ( project_name );
  ecpp_stream << "\n\nusing version\nusing config\nusing test\nusing install\nusing dist\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  std::string project_name;
};

void render_build2_build_bootstrap_build(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_Build2BuildBootstrapBuild(renderer, target, vars).render();
}