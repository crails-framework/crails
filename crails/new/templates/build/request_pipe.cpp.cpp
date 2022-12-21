#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class render_ProjectConfigRequestPipeCpp : public Crails::Template
{
public:
  render_ProjectConfigRequestPipeCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    parsers(reinterpret_cast<vector<pair<string, string>>&>(*Crails::cast<vector<pair<string, string>>*>(vars, "parsers"))), 
    handlers(reinterpret_cast<vector<pair<string, string>>&>(*Crails::cast<vector<pair<string, string>>*>(vars, "handlers")))
  {}

  void render()
  {
ecpp_stream << "";
 for (auto parser : parsers){
  ecpp_stream << "" << ( "#include <crails/request_parsers/" + parser.first + ".hpp>\n" );
  ecpp_stream << "";
 };
  ecpp_stream << "";
 for (auto handler : handlers){
  ecpp_stream << "" << ( "#include <crails/request_handlers/" + handler.first + ".hpp>\n" );
  ecpp_stream << "";
 };
  ecpp_stream << "#include <crails/request_handlers/file.hpp>\n#include <crails/request_handlers/action.hpp>\n#include <crails/request_parser.hpp>\n#inculde \"server.hpp\"\n\nusing namespace Crails;\n\nvoid ApplicationServer::initialize_request_pipe()\n{";
 for (auto parser : parsers){
  ecpp_stream << "\n  " << ( "add_request_parser(new " + parser.second + ");" );
  ecpp_stream << "";
 };
  ecpp_stream << "";
 for (auto handler : handlers){
  ecpp_stream << "\n  " << ( "add_request_handler(new " + handler.second + ");" );
  ecpp_stream << "";
 };
  ecpp_stream << "\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  vector<pair<string, string>>& parsers;
  vector<pair<string, string>>& handlers;
};

void render_project_config_request_pipe_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigRequestPipeCpp(renderer, target, vars).render();
}