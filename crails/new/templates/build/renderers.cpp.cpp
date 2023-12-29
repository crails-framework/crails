#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class render_ProjectConfigRenderersCpp : public Crails::Template
{
public:
  render_ProjectConfigRenderersCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    default_format(Crails::cast<string>(vars, "default_format",  "text/html")), 
    renderers(reinterpret_cast<vector<pair<string, string>>&>(*Crails::cast<vector<pair<string, string>>*>(vars, "renderers")))
  {}

  void render()
  {
ecpp_stream << "#include \"renderers.hpp\"\n";
 for (auto renderer : renderers){
  ecpp_stream << "" << ( "#include \"lib/renderers/" + renderer.first + ".hpp\"\n" );
  ecpp_stream << "";
 };
  ecpp_stream << "\n\nusing namespace std;\nusing namespace Crails;\n\nApplicationRenderers::ApplicationRenderers()\n{\n  default_format = \"" << ( default_format );
  ecpp_stream << "\";\n  // Append renderers";
 for (auto renderer : renderers){
  ecpp_stream << "\n  " << ( "renderers.push_back(make_unique<" + renderer.second + ">());\n" );
  ecpp_stream << "";
 };
  ecpp_stream << "\n}\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  string default_format;
  vector<pair<string, string>>& renderers;
};

void render_project_config_renderers_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigRenderersCpp(renderer, target, vars).render();
}