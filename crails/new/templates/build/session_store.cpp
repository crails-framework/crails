#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>

class ProjectConfigSessionStoreCpp : public Crails::Template
{
public:
  ProjectConfigSessionStoreCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    session_store(Crails::cast<std::string>(vars, "session_store",  "NoSessionStore"))
  {}

  void render()
  {
ecpp_stream << "#include <crails/session_store/" << ( Crails::underscore(session_store) );
  ecpp_stream << ".hpp>\n#include <crails/session_store.hpp>\n\nusing namespace Crails;\n\nUSE_SESSION_STORE(" << ( session_store );
  ecpp_stream << ")\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  std::string session_store;
};

void render_project_config_session_store_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ProjectConfigSessionStoreCpp(renderer, target, vars).render();
}