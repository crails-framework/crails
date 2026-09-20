#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectAppRoutesCpp : public Crails::Template
{
public:
  render_ProjectAppRoutesCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(298);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include \"config/router.hpp\"\n\nApplicationRouter::ApplicationRouter()\n{\n  // Append routes here (do not remove this line)\n}\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_project_app_routes_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectAppRoutesCpp(renderer, target, vars).render();
}