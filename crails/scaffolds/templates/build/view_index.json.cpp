#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>

class render_ScaffoldsViewIndexJson : public Crails::Template
{
public:
  render_ScaffoldsViewIndexJson(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast<std::string>(vars, "classname")), 
    resource_name(Crails::cast<std::string>(vars, "resource_name",  Crails::underscore(classname))), 
    header(Crails::cast<std::string>(vars, "header")), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast<std::map<std::string, std::string>*>(vars, "properties")))
  {}

  void render()
  {
ecpp_stream << "#include \"" << ( header );
  ecpp_stream << "\"\n\nstd::vector<" << ( classname );
  ecpp_stream << ">& @models;\n// END LINKING\njson_array(\"" << ( Crails::pluralize(resource_name) );
  ecpp_stream << "\", [&](const <" << ( classname );
  ecpp_stream << ">& model)\n{";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\n  json(\"" << ( it->first );
  ecpp_stream << "\", model.get_" << ( it->first );
  ecpp_stream << "());";
 };
  ecpp_stream << "\n});\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
  std::string resource_name;
  std::string header;
  std::map<std::string, std::string>& properties;
};

void render_scaffolds_view_index_json(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsViewIndexJson(renderer, target, vars).render();
}