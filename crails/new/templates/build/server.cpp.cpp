#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectConfigServerCpp : public Crails::Template
{
public:
  render_ProjectConfigServerCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#include \"server.hpp\"\n#include <crails/logger.hpp>\n\nusing namespace Crails;\n\nApplicationServer::ApplicationServer()\n{\n  logger.set_log_level(Logger::Info);\n  temporary_path = \"/tmp\";\n  initialize_request_pipe();\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_config_server_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigServerCpp(renderer, target, vars).render();
}