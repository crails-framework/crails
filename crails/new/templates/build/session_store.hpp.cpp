#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>
#include <crails/utils/random_string.hpp>
using namespace Crails;
using namespace std;

class render_ProjectConfigSessionStoreHpp : public Crails::Template
{
public:
  render_ProjectConfigSessionStoreHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    session_store(Crails::cast<string>(vars, "session_store",  "NoSessionStore")), 
    is_cookie_store( Crails::camelize(session_store) == "CookieStore")
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <crails/session_store/" << ( Crails::underscore(session_store) );
  ecpp_stream << ".hpp>\n\nclass SessionStoreImpl : public Crails::" << ( camelize(session_store) );
  ecpp_stream << "\n{\n  SESSION_STORE_IMPLEMENTATION(SessionStoreImpl)\n";
 if (is_cookie_store){
  ecpp_stream << "\n  static const string password;\n  static const string salt;\n\n  SessionStoreImpl() : Crails::" << ( camelize(session_store) );
  ecpp_stream << "(password, salt)\n  {\n  }";
 };
  ecpp_stream << "\n};\n";
 if (is_cookie_store){
  ecpp_stream << "\nconst string SessionStoreImpl::password = \"" << ( generate_random_string(charset, 50) );
  ecpp_stream << "\";\nconst string SessionStoreImpl::salt     = \"" << ( generate_random_string(charset, 8) );
  ecpp_stream << "\";";
 };
  ecpp_stream << "\ntypedef SingletonInstantiator<SessionStoreImpl::Factory> ApplicationSessionStore;\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  string session_store;
  bool is_cookie_store;
};

void render_project_config_session_store_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigSessionStoreHpp(renderer, target, vars).render();
}