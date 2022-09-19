#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/random_string.hpp>
using namespace Crails;
using namespace std;

class ProjectConfigCookiesCpp : public Crails::Template
{
public:
  ProjectConfigCookiesCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    charset( "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_")
  {}

  std::string render()
  {
ecpp_stream << "#include <crails/cookie_data.hpp>\n\nusing namespace Crails;\nusing namespace std;\n\nconst bool   CookieData::use_encryption = true;\nconst string CookieData::password       = \"" << ( generate_random_string(charset, 50) );
  ecpp_stream << "\";\nconst string CookieData::salt           = \"" << ( generate_random_string(charset, 8) );
  ecpp_stream << "\";\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  const std::string charset;
};

std::string render_project_config_cookies_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ProjectConfigCookiesCpp(renderer, vars).render();
}