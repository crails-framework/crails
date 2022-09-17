#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>
#include <crails/cli/conventions.hpp>

class ScaffoldsOdbModelHpp : public Crails::Template
{
public:
  ScaffoldsOdbModelHpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    classname(Crails::cast<std::string>(vars, "classname")), 
    superclass(Crails::cast<std::string>(vars, "superclass",  "public Crails::Odb::Model")), 
    include(Crails::cast<std::string>(vars, "include",  "")), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast<std::map<std::string, std::string>*>(vars, "properties"))), 
    scalar_types( {"bool","char","unsigned char","char16_t","char32_t","wchar_t","signed char","int","unsigned int","short","unsigned short","long","long long","unsigned long","unsigned long long","double","long double","float","std::size_t","std::time_t"}), 
    metarecord(Crails::cast<bool>(vars, "metarecord",  false))
  {}

  std::string render()
  {
ecpp_stream << "#pragma once";
 if (metarecord){
  ecpp_stream << "\n#include \"" << ( include );
  ecpp_stream << "\"";
 };
  ecpp_stream << "\n#include <crails/odb/model.hpp>\n#include <crails/datatree.hpp>\n\n#pragma db object\nclass " << ( classname );
  ecpp_stream << " : " << ( superclass );
  ecpp_stream << "\n{\n  odb_instantiable()\npublic:";
 if (!metarecord){
  ecpp_stream << "\n  static const std::string scope;\n  static const std::string plural_scope;\n  static const std::string view;";
 };
  ecpp_stream << "\n\n  #pragma db view object(" << ( classname );
  ecpp_stream << ")\n  struct Count\n  {\n    #pragma db column(\"count(\" + " << ( classname );
  ecpp_stream << "::id + \")\")\n    size_t value;\n  };\n";
 if (!metarecord){
  ecpp_stream << "\n  void " << ( Crails::naming_convention.functions("edit") );
  ecpp_stream << "(Data);\n  void " << ( Crails::naming_convention.functions("merge_data") );
  ecpp_stream << "(Data) const;\n  std::string " << ( Crails::naming_convention.functions("to_json") );
  ecpp_stream << "() const;";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "";
   if (it->second == "DataTree"){
  ecpp_stream << "\n  DataTree& " << ( Crails::naming_convention.functions("get_" + it->first) );
  ecpp_stream << "() { return " << ( it->first );
  ecpp_stream << "; }\n  const DataTree& " << ( Crails::naming_convention.functions("get_" + it->first) );
  ecpp_stream << "() const { return " << ( it->first );
  ecpp_stream << "; }\n  void " << ( Crails::naming_convention.functions("set_" + it->first) );
  ecpp_stream << "(Data value) { this->" << ( it->first );
  ecpp_stream << ".clear(); this->" << ( it->first );
  ecpp_stream << ".as_data().merge(value); }";
 }else if (std::find(scalar_types.begin(), scalar_types.end(), it->second) != scalar_types.end()){
  ecpp_stream << "\n  void " << ( Crails::naming_convention.functions("set_" + it->first) );
  ecpp_stream << "(" << ( it->second );
  ecpp_stream << " value) { this->" << ( it->first );
  ecpp_stream << " = value; }\n  " << ( it->second );
  ecpp_stream << " " << ( Crails::naming_convention.functions("get_" + it->first) );
  ecpp_stream << "() const { return " << ( it->first );
  ecpp_stream << "; }";
 }else{
  ecpp_stream << "\n  void " << ( Crails::naming_convention.functions("set_" + it->first) );
  ecpp_stream << "(const " << ( it->second );
  ecpp_stream << "& value) { this->" << ( it->first );
  ecpp_stream << " = value; }\n  const " << ( it->second );
  ecpp_stream << "& " << ( Crails::naming_convention.functions("get_" + it->first) );
  ecpp_stream << "() const { return " << ( it->first );
  ecpp_stream << "; }";
 };
  ecpp_stream << "";
 };
  ecpp_stream << "\n\nprivate:";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\n  " << ( it->second );
  ecpp_stream << " " << ( it->first );
  ecpp_stream << ";";
 };
  ecpp_stream << "";
 };
  ecpp_stream << "\n};\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
  std::string superclass;
  std::string include;
  std::map<std::string, std::string>& properties;
  std::vector<std::string> scalar_types;
  bool metarecord;
};

std::string render_scaffolds_odb_model_hpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldsOdbModelHpp(renderer, vars).render();
}