#include <sstream>
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
ecpp_stream << "#include <crails/renderers/" << ( renderer_type );
  ecpp_stream << ".hpp>\n\nclass " << ( renderer_name );
  ecpp_stream << " : public Crails::" << ( Crails::camelize(renderer_type) );
  ecpp_stream << "Renderer\n{\npublic:\n  " << ( renderer_name );
  ecpp_stream << "();\n};\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  string renderer_name;
  string renderer_type;
};

void render_renderer_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_RendererHpp(renderer, target, vars).render();
}