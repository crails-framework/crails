#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>

class render_ScaffoldsViewShowJson : public Crails::Template
{
public:
  render_ScaffoldsViewShowJson(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast< std::string >(vars, "classname")), 
    header(Crails::cast< std::string >(vars, "header")), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast< std::map<std::string, std::string>* >(vars, "properties")))
  {}

  void render()
  {
    ecpp_stream.reserve(702);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include \"" << ( header );
  ecpp_stream << "\"\n" << ( classname );
  ecpp_stream << "& @model;\n// END LINKING";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\njson(\"" << ( it->first );
  ecpp_stream << "\", model.get_" << ( it->first );
  ecpp_stream << "());";
 };
  ecpp_stream << "\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  std::string classname;
  std::string header;
  std::map<std::string, std::string>& properties;
};

void render_scaffolds_view_show_json(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsViewShowJson(renderer, target, vars).render();
}