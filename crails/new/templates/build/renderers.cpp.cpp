#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class ProjectConfigRenderersCpp : public Crails::Template
{
public:
  ProjectConfigRenderersCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    default_format(Crails::cast<string>(vars, "default_format",  "text/html")), 
    renderers(reinterpret_cast<vector<pair<string, string>>&>(*Crails::cast<vector<pair<string, string>>*>(vars, "renderers")))
  {}

  void render()
  {
ecpp_stream << "#include <crails/renderer.hpp>\n";
 for (auto renderer : renderers){
  ecpp_stream << "" << ( "#include <crails/renderers/" + renderer.first + ".hpp>\n" );
  ecpp_stream << "";
 };
  ecpp_stream << "\n\nusing namespace Crails;\n\nconst std::string Renderer::default_format = \"" << ( default_format );
  ecpp_stream << "\";\n\nvoid Renderer::initialize()\n{\n  // Append renderers";
 for (auto renderer : renderers){
  ecpp_stream << "\n  " << ( "renderers.push_back(new " + renderer.second + ");\n" );
  ecpp_stream << "";
 };
  ecpp_stream << "\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  string default_format;
  vector<pair<string, string>>& renderers;
};

void render_project_config_renderers_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ProjectConfigRenderersCpp(renderer, target, vars).render();
}