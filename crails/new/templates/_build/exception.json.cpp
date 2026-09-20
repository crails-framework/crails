#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectAppViewsExceptionJson : public Crails::Template
{
public:
  render_ProjectAppViewsExceptionJson(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(1818);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include <crails/params.hpp>\n\nusing namespace Crails;\nusing namespace std;\n\nstring  @exception_name;\nstring  @exception_what;\nParams& @params;\n\n// END LINKING\n\njson<std::function<void()> >(\"exception\", [=]()\n{\n  json(\"type\", exception_name);\n  json(\"what\", exception_what);\n\n  if (params[\"controller-data\"].exists())\n  {\n    json<std::function<void()> >(\"controller\", [=]()\n    {\n      json(\"type\",   params[\"controller-data\"][\"name\"].defaults_to<string>(\"\"));\n      json(\"method\", params[\"controller-data\"][\"action\"].defaults_to<string>(\"\"));\n    });\n    params[\"controller-data\"].destroy();\n  }\n\n  if (params[\"backtrace\"].exists())\n  {\n    json(\"backtrace\", params[\"backtrace\"].as<string>());\n    params[\"backtrace\"].destroy();\n  }\n\n  json(\"params\", params.as_data());\n  json(\"session\", params.get_session());\n});\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_project_app_views_exception_json(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectAppViewsExceptionJson(renderer, target, vars).render();
}