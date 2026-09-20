#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>
using namespace std;

class render_RendererHpp : public Crails::Template
{
public:
  render_RendererHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    renderer_name(Crails::cast< string >(vars, "renderer_name")), 
    renderer_type(Crails::cast< string >(vars, "renderer_type"))
  {}

  void render()
  {
    ecpp_stream.reserve(594);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include <crails/renderers/" << ( renderer_type );
  ecpp_stream << ".hpp>\n\nclass " << ( renderer_name );
  ecpp_stream << " : public Crails::" << ( Crails::camelize(renderer_type) );
  ecpp_stream << "Renderer\n{\npublic:\n  " << ( renderer_name );
  ecpp_stream << "();\n};\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  string renderer_name;
  string renderer_type;
};

void render_renderer_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_RendererHpp(renderer, target, vars).render();
}