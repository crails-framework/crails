#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>
using namespace std;

class render_RendererCpp : public Crails::Template
{
public:
  render_RendererCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    renderer_name(Crails::cast<string>(vars, "renderer_name")), 
    renderer_filename(Crails::cast<string>(vars, "renderer_filename")), 
    function_prefix(Crails::cast<string>(vars, "function_prefix",  "render")), 
    targets(reinterpret_cast<map<string, string>&>(*Crails::cast<map<string, string>*>(vars, "targets")))
  {}

  void render()
  {
ecpp_stream << "#include \"" << ( renderer_filename );
  ecpp_stream << ".hpp\"\n#define declare_renderer(name) void " << ( function_prefix );
  ecpp_stream << "_##name(const Crails::Renderer&, Crails::RenderTarget&, Crails::SharedVars&)\n#define add_renderer(path, name) templates.insert(std::pair<std::string, Generator>(path, " << ( function_prefix );
  ecpp_stream << "_##name))\n";
 for (auto it = targets.begin() ; it != targets.end() ; ++it){
  ecpp_stream << "\ndeclare_renderer(" << ( it->second );
  ecpp_stream << ");";
 };
  ecpp_stream << "\n" << ( renderer_name );
  ecpp_stream << "::" << ( renderer_name );
  ecpp_stream << "()\n{";
 for (auto it = targets.begin() ; it != targets.end() ; ++it){
  ecpp_stream << "\n  add_renderer(\"" << ( it->first );
  ecpp_stream << "\", " << ( it->second );
  ecpp_stream << ");";
 };
  ecpp_stream << "\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  string renderer_name;
  string renderer_filename;
  string function_prefix;
  map<string, string>& targets;
};

void render_renderer_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_RendererCpp(renderer, target, vars).render();
}