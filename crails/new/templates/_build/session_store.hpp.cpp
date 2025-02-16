#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>
using namespace Crails;
using namespace std;

class render_ProjectConfigSessionStoreHpp : public Crails::Template
{
public:
  render_ProjectConfigSessionStoreHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    session_store(Crails::cast< string >(vars, "session_store",  "NoSessionStore")), 
    is_cookie_store( Crails::camelize(session_store) == "CookieStore")
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <crails/session_store/" << ( Crails::underscore(session_store) );
  ecpp_stream << ".hpp>\n\nclass SessionStoreImpl : public Crails::" << ( camelize(session_store) );
  ecpp_stream << "\n{\n  SESSION_STORE_IMPLEMENTATION(SessionStoreImpl)\n";
 if (is_cookie_store){
  ecpp_stream << "\n  static const std::string password;\n  static const std::string salt;";
 };
  ecpp_stream << "\npublic:\n";
 if (is_cookie_store){
  ecpp_stream << "\n  SessionStoreImpl() : Crails::" << ( camelize(session_store) );
  ecpp_stream << "(password, salt)\n  {\n  }";
 }else{
  ecpp_stream << "\n  SessionStoreImpl() {}";
 };
  ecpp_stream << "\n};\n\ntypedef SingletonInstantiator<SessionStoreImpl::Factory> ApplicationSessionStore;\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
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