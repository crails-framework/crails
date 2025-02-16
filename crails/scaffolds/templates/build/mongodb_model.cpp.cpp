#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>
#include <crails/cli/conventions.hpp>

class render_ScaffoldsMongodbModelCpp : public Crails::Template
{
public:
  render_ScaffoldsMongodbModelCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    filename(Crails::cast< std::string >(vars, "filename")), 
    classname(Crails::cast< std::string >(vars, "classname")), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast< std::map<std::string, std::string>* >(vars, "properties")))
  {}

  void render()
  {
ecpp_stream << "#include \"" << ( filename );
  ecpp_stream << ".hpp\"\n\nconst std::string " << ( classname );
  ecpp_stream << "::collection_name = \"" << ( classname );
  ecpp_stream << "\";\nconst std::string " << ( classname );
  ecpp_stream << "::scope = \"" << ( Crails::naming_convention.keys(classname) );
  ecpp_stream << "\";\nconst std::string " << ( classname );
  ecpp_stream << "::plural_scope = \"" << ( Crails::pluralize(Crails::naming_convention.keys(classname)) );
  ecpp_stream << "\";\nconst std::string " << ( classname );
  ecpp_stream << "::view = \"\";\n\nvoid " << ( classname );
  ecpp_stream << "::" << ( Crails::naming_convention.functions("edit") );
  ecpp_stream << "(Data params)\n{";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\n  if (params[\"" << ( it->first );
  ecpp_stream << "\"].exists())\n    " << ( Crails::naming_convention.functions("set_" + it->first) );
  ecpp_stream << "(params[\"" << ( it->first );
  ecpp_stream << "\"]);";
 };
  ecpp_stream << "\n}\n\nvoid " << ( classname );
  ecpp_stream << "::" << ( Crails::naming_convention.functions("merge_data") );
  ecpp_stream << "(Data out) const\n{";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "";
   if (it->second == "DataTree"){
  ecpp_stream << "\n  out[\"" << ( it->first );
  ecpp_stream << "\"].merge(this->" << ( it->first );
  ecpp_stream << ");";
 }else if (it->second == "Crails::Password"){
  ecpp_stream << "";
     continue ;
  ecpp_stream << "";
 }else{
  ecpp_stream << "\n  out[\"" << ( it->first );
  ecpp_stream << "\"] = this->" << ( it->first );
  ecpp_stream << ";";
 };
  ecpp_stream << "";
 };
  ecpp_stream << "\n}\n\nstd::string " << ( classname );
  ecpp_stream << "::" << ( Crails::naming_convention.functions("to_json") );
  ecpp_stream << "() const\n{\n  DataTree out;\n\n  merge_data(out);\n  return out.to_json();\n}\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string filename;
  std::string classname;
  std::map<std::string, std::string>& properties;
};

void render_scaffolds_mongodb_model_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsMongodbModelCpp(renderer, target, vars).render();
}