#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>

class ScaffoldsViewIndexJson : public Crails::Template
{
public:
  ScaffoldsViewIndexJson(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    classname(Crails::cast<std::string>(vars, "classname")), 
    resource_name(Crails::cast<std::string>(vars, "resource_name",  Crails::underscore(classname))), 
    header(Crails::cast<std::string>(vars, "header")), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast<std::map<std::string, std::string>*>(vars, "properties")))
  {}

  std::string render()
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
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
  std::string resource_name;
  std::string header;
  std::map<std::string, std::string>& properties;
};

std::string render_scaffolds_view_index_json(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldsViewIndexJson(renderer, vars).render();
}