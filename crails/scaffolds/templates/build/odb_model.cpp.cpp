#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>
#include <crails/cli/conventions.hpp>

class ScaffoldsOdbModelCpp : public Crails::Template
{
public:
  ScaffoldsOdbModelCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    filename(Crails::cast<std::string>(vars, "filename")), 
    classname(Crails::cast<std::string>(vars, "classname")), 
    odb_at_once(Crails::cast<bool>(vars, "odb_at_once")), 
    metarecord(Crails::cast<bool>(vars, "metarecord",  false)), 
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
  ecpp_stream << "\n";
 if (!metarecord){
  ecpp_stream << "\nconst std::string " << ( classname );
  ecpp_stream << "::resource_name = \"" << ( Crails::naming_convention.keys(classname) );
  ecpp_stream << "\";";
 };
  ecpp_stream << "\n\nodb_instantiable_impl(" << ( classname );
  ecpp_stream << ")\n";
 if (!metarecord){
  ecpp_stream << "\nvoid " << ( classname );
  ecpp_stream << "::" << ( Crails::naming_convention.functions("edit") );
  ecpp_stream << "(Data params)\n{";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\n  if (params[\"" << ( it->first );
  ecpp_stream << "\"].exists())\n    " << ( Crails::naming_convention.functions("set_" + it->first) );
  ecpp_stream << "(params[\"" << ( it->first );
  ecpp_stream << "\"]);";
 };
  ecpp_stream << "\n}";
 };
  ecpp_stream << "\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string filename;
  std::string classname;
  bool odb_at_once;
  bool metarecord;
  std::map<std::string, std::string>& properties;
};

std::string render_scaffolds_odb_model_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldsOdbModelCpp(renderer, vars).render();
}