#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/params.hpp>
using namespace Crails;
using namespace std;

class render_ProjectAppViewsExceptionJson : public Crails::Template
{
public:
  render_ProjectAppViewsExceptionJson(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    exception_name(Crails::cast<string>(vars, "exception_name")), 
    exception_what(Crails::cast<string>(vars, "exception_what")), 
    params(reinterpret_cast<Params&>(*Crails::cast<Params*>(vars, "params")))
  {}

  void render()
  {
ecpp_stream << "json<std::function<void()> >(\"exception\", [=]()\n{\n  json(\"type\", exception_name);\n  json(\"what\", exception_what);\n\n  if (params[\"controller-data\"].exists())\n  {\n    json<std::function<void()> >(\"controller\", [=]()\n    {\n      json(\"type\",   params[\"controller-data\"][\"name\"].defaults_to<string>(\"\"));\n      json(\"method\", params[\"controller-data\"][\"action\"].defaults_to<string>(\"\"));\n    });\n    params[\"controller-data\"].destroy();\n  }\n\n  if (params[\"backtrace\"].exists())\n  {\n    json(\"backtrace\", params[\"backtrace\"].as<string>());\n    params[\"backtrace\"].destroy();\n  }\n\n  json(\"params\", params.as_data());\n  json(\"session\", params.get_session());\n});\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  string exception_name;
  string exception_what;
  Params& params;
};

void render_project_app_views_exception_json(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectAppViewsExceptionJson(renderer, target, vars).render();
}