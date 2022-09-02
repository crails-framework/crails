#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class ScaffoldsMetarecordModelRb : public Crails::Template
{
public:
  ScaffoldsMetarecordModelRb(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    resource_name(Crails::cast<string>(vars, "resource_name")), 
    classname(Crails::cast<string>(vars, "classname")), 
    include(Crails::cast<string>(vars, "include")), 
    properties(reinterpret_cast<map<string, string>&>(*Crails::cast<map<string, string>*>(vars, "properties")))
  {}

  std::string render()
  {
ecpp_stream << "Model.add '" << ( classname );
  ecpp_stream << "', hpp: '" << ( include );
  ecpp_stream << "' do\n  resource_name '" << ( resource_name );
  ecpp_stream << "'\n  #order_by 'property'\n\n  visibility :protected";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "";
   if (it->second == "DataTree"){
  ecpp_stream << "\n  property 'DataTree', '" << ( it->first );
  ecpp_stream << "', db: { type: \"TEXT\" }";
 }else{
  ecpp_stream << "\n  property '" << ( it->second );
  ecpp_stream << "', '" << ( it->first );
  ecpp_stream << "'";
 };
  ecpp_stream << "";
 };
  ecpp_stream << "\nend\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  string resource_name;
  string classname;
  string include;
  map<string, string>& properties;
};

std::string render_scaffolds_metarecord_model_rb(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldsMetarecordModelRb(renderer, vars).render();
}