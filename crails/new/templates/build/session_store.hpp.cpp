#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>

class render_ProjectConfigSessionStoreHpp : public Crails::Template
{
public:
  render_ProjectConfigSessionStoreHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    session_store(Crails::cast<std::string>(vars, "session_store",  "NoSessionStore"))
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <crails/session_store/" << ( Crails::underscore(session_store) );
  ecpp_stream << ".hpp>\n\ntypedef SingletonInstantiator<" << ( Crails::camelize(session_store) );
  ecpp_stream << "> ApplicationSessionStore;\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  std::string session_store;
};

void render_project_config_session_store_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigSessionStoreHpp(renderer, target, vars).render();
}