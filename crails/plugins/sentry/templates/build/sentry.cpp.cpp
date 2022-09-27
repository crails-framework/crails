#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class SentryConfigSentryCpp : public Crails::Template
{
public:
  SentryConfigSentryCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#include <crails/sentry.hpp>\n#include <crails/getenv.hpp>\n\nusing namespace std;\nusing namespace Crails;\n\nconst string Sentry::project_id      = Crails::getenv(\"SENTRY_PROJECT_ID\");\nconst string Sentry::sentry_key      = Crails::getenv(\"SENTRY_KEY\");\nconst string Sentry::sentry_secret   = Crails::getenv(\"SENTRY_SECRET\");\nconst string Sentry::sentry_version  = Crails::getenv(\"SENTRY_VERSION\",  \"7\");\nconst string Sentry::server_protocol = Crails::getenv(\"SENTRY_PROTOCOL\", \"https\");\nconst string Sentry::server_url      = Crails::getenv(\"SENTRY_URL\",      \"sentry.io\");\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_sentry_config_sentry_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return SentryConfigSentryCpp(renderer, vars).render();
}