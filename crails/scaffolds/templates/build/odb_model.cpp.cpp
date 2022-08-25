#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>

class ScaffoldsOdbModelCpp : public Crails::Template
{
public:
  ScaffoldsOdbModelCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    filename(Crails::cast<std::string>(vars, "filename")), 
    classname(Crails::cast<std::string>(vars, "classname")), 
    odb_at_once(Crails::cast<bool>(vars, "odb_at_once")), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast<std::map<std::string, std::string>*>(vars, "properties")))
  {}

  std::string render()
  {
ecpp_stream << "#include \"" << ( filename );
  ecpp_stream << ".hpp\"";
 if (odb_at_once){
  ecpp_stream << "\n#include \"lib/odb/application-odb.hxx\"";
 }else{
  ecpp_stream << "\n#include \"lib/odb/" << ( filename );
  ecpp_stream << "-odb.hxx\"";
 };
  ecpp_stream << "\n\nconst std::string " << ( classname );
  ecpp_stream << "::resource_name = \"" << ( Crails::underscore(classname) );
  ecpp_stream << "\";\n\nodb_instantiable_impl(" << ( classname );
  ecpp_stream << ")\n\nvoid " << ( classname );
  ecpp_stream << "::edit(Data params)\n{";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\n  if (params[\"" << ( it->first );
  ecpp_stream << "\"].exists())\n    set_" << ( it->first );
  ecpp_stream << "(params[\"" << ( it->first );
  ecpp_stream << "\"]);";
 };
  ecpp_stream << "\n}\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string filename;
  std::string classname;
  bool odb_at_once;
  std::map<std::string, std::string>& properties;
};

std::string render_scaffolds_odb_model_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldsOdbModelCpp(renderer, vars).render();
}