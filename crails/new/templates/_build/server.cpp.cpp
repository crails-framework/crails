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
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_config_server_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigServerCpp(renderer, target, vars).render();
}