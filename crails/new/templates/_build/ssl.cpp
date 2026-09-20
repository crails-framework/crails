#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectConfigSslCpp : public Crails::Template
{
public:
  render_ProjectConfigSslCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(560);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include <boost/asio.hpp>\n#include <boost/asio/ssl.hpp>\n\nusing namespace boost;\n\nnamespace Crails\n{\n  std::string get_ssl_password(std::size_t max_length, asio::ssl::context_base::password_purpose purpose)\n  {\n    return \"your_ssl_password\";\n  }\n}\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_project_config_ssl_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectConfigSslCpp(renderer, target, vars).render();
}