#include <sstream>
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
ecpp_stream << "#include \"" << ( header );
  ecpp_stream << "\"\n" << ( classname );
  ecpp_stream << "& @model;\n// END LINKING";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\njson(\"" << ( it->first );
  ecpp_stream << "\", model.get_" << ( it->first );
  ecpp_stream << "());";
 };
  ecpp_stream << "\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
  std::string header;
  std::map<std::string, std::string>& properties;
};

void render_scaffolds_view_show_json(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsViewShowJson(renderer, target, vars).render();
}