#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectConfigServerHpp : public Crails::Template
{
public:
  render_ProjectConfigServerHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <crails/server.hpp>\n#include \"session_store.hpp\"\n\nclass ApplicationServer : public Crails::Server\n{\n  SINGLETON_IMPLEMENTATION(ApplicationServer, Crails::Server)\n\n  ApplicationServer();\n  void initialize_request_pipe();\n\n  ApplicationSessionStore session_store;\n};\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_config_server_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigServerHpp(renderer, target, vars).render();
}