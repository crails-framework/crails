#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_SidekixSidetaskCpp : public Crails::Template
{
public:
  render_SidekixSidetaskCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    function_name(Crails::cast< std::string >(vars, "function_name"))
  {}

  void render()
  {
    ecpp_stream.reserve(258);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include <crails/params.hpp>\n\nvoid " << ( function_name );
  ecpp_stream << "(Crails::Params& params)\n{\n}\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  std::string function_name;
};

void render_sidekix_sidetask_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_SidekixSidetaskCpp(renderer, target, vars).render();
}