#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/random_string.hpp>
using namespace Crails;
using namespace std;

class render_ProjectConfigCookiesCpp : public Crails::Template
{
public:
  render_ProjectConfigCookiesCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    charset( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_")
  {}

  void render()
  {
ecpp_stream << "#include <crails/cookie_data.hpp>\n\nusing namespace Crails;\nusing namespace std;\n\nconst bool   CookieData::use_encryption = true;\nconst string CookieData::password       = \"" << ( generate_random_string(charset, 50) );
  ecpp_stream << "\";\nconst string CookieData::salt           = \"" << ( generate_random_string(charset, 8) );
  ecpp_stream << "\";\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  const std::string charset;
};

void render_project_config_cookies_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigCookiesCpp(renderer, target, vars).render();
}