#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>
#include <crails/cli/conventions.hpp>
#include "../../helpers.hpp"

class render_ScaffoldsModelPropertiesHpp : public Crails::Template
{
public:
  render_ScaffoldsModelPropertiesHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast<std::map<std::string, std::string>*>(vars, "properties"))), 
    scalar_types( {"bool","char","unsigned char","char16_t","char32_t","wchar_t","signed char","int","unsigned int","short","unsigned short","long","long long","unsigned long","unsigned long long","double","long double","float","std::size_t","std::time_t"})
  {}

  void render()
  {
ecpp_stream << "";
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
 }else if (it->second == "Crails::Password"){
  ecpp_stream << "\n  void " << ( Crails::naming_convention.functions("set_" + it->first) );
  ecpp_stream << "(const std::string& value) { " << ( it->first );
  ecpp_stream << " = Crails::Password(value); }\n  const std::string& " << ( Crails::naming_convention.functions("get_" + it->first) );
  ecpp_stream << "() const { return " << ( it->first );
  ecpp_stream << "; }";
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
  ecpp_stream << "\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::map<std::string, std::string>& properties;
  std::vector<std::string> scalar_types;
};

void render_scaffolds_model_properties_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsModelPropertiesHpp(renderer, target, vars).render();
}