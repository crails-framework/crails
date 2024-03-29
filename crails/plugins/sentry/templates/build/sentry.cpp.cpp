#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_SentryConfigSentryCpp : public Crails::Template
{
public:
  render_SentryConfigSentryCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#include <crails/sentry.hpp>\n#include <crails/getenv.hpp>\n\nusing namespace std;\nusing namespace Crails;\n\nconst string Sentry::project_id      = Crails::getenv(\"SENTRY_PROJECT_ID\");\nconst string Sentry::sentry_key      = Crails::getenv(\"SENTRY_KEY\");\nconst string Sentry::sentry_secret   = Crails::getenv(\"SENTRY_SECRET\");\nconst string Sentry::sentry_version  = Crails::getenv(\"SENTRY_VERSION\",  \"7\");\nconst string Sentry::server_protocol = Crails::getenv(\"SENTRY_PROTOCOL\", \"https\");\nconst string Sentry::server_url      = Crails::getenv(\"SENTRY_URL\",      \"sentry.io\");\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
};

void render_sentry_config_sentry_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_SentryConfigSentryCpp(renderer, target, vars).render();
}