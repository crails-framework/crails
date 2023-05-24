#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>
#include <crails/utils/random_string.hpp>
using namespace Crails;
using namespace std;

class render_ProjectConfigSessionStoreCpp : public Crails::Template
{
public:
  render_ProjectConfigSessionStoreCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    session_store(Crails::cast<string>(vars, "session_store",  "NoSessionStore")), 
    is_cookie_store( Crails::camelize(session_store) == "CookieStore"), 
    charset( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_")
  {}

  void render()
  {
ecpp_stream << "#include \"session_store.hpp\"\n\nusing namespace std;\n";
 if (is_cookie_store){
  ecpp_stream << "\nconst string SessionStoreImpl::password = \"" << ( generate_random_string(charset, 50) );
  ecpp_stream << "\";\nconst string SessionStoreImpl::salt     = \"" << ( generate_random_string(charset, 8) );
  ecpp_stream << "\";";
 };
  ecpp_stream << "";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  string session_store;
  bool is_cookie_store;
  string charset;
};

void render_project_config_session_store_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigSessionStoreCpp(renderer, target, vars).render();
}