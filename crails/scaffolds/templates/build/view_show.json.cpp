#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>

class ScaffoldsViewShowJson : public Crails::Template
{
public:
  ScaffoldsViewShowJson(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    classname(Crails::cast<std::string>(vars, "classname")), 
    header(Crails::cast<std::string>(vars, "header")), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast<std::map<std::string, std::string>*>(vars, "properties")))
  {}

  std::string render()
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
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
  std::string header;
  std::map<std::string, std::string>& properties;
};

std::string render_scaffolds_view_show_json(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldsViewShowJson(renderer, vars).render();
}