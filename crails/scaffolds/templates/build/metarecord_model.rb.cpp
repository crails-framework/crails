#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class render_ScaffoldsMetarecordModelRb : public Crails::Template
{
public:
  render_ScaffoldsMetarecordModelRb(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    resource_name(Crails::cast< string >(vars, "resource_name")), 
    classname(Crails::cast< string >(vars, "classname")), 
    include(Crails::cast< string >(vars, "include")), 
    properties(reinterpret_cast<map<string, string>&>(*Crails::cast< map<string, string>* >(vars, "properties")))
  {}

  void render()
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
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  string resource_name;
  string classname;
  string include;
  map<string, string>& properties;
};

void render_scaffolds_metarecord_model_rb(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsMetarecordModelRb(renderer, target, vars).render();
}