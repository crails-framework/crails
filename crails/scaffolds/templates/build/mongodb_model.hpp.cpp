#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>
#include <crails/cli/conventions.hpp>
#include "../../helpers.hpp"

class render_ScaffoldsMongodbModelHpp : public Crails::Template
{
public:
  render_ScaffoldsMongodbModelHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast< std::string >(vars, "classname")), 
    superclass(Crails::cast< std::string >(vars, "superclass",  "public Crails::MongoDB::ModelInterface")), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast< std::map<std::string, std::string>* >(vars, "properties")))
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <crails/mongodb/model.hpp>" << ( includes_for_properties(properties) );
  ecpp_stream << "\nclass " << ( classname );
  ecpp_stream << " : " << ( superclass );
  ecpp_stream << "\n{\npublic:\n  static const std::string collection_name;\n  static const std::string scope;\n  static const std::string plural_scope;\n  static const std::string view;\n\n  void set_id(const std::string& value) { id = value; }\n  std::string get_id() const override { return id; }\n  std::string to_json() const override;\n  void edit(Data) override;\n  void merge_data(Data) const;\n  " << ( partial("scaffolds/_model_properties.hpp") );
  ecpp_stream << "\n\nprivate:\n  std::string id;";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "";
   if (it->second == "Crails::Password"){
  ecpp_stream << "\n  std::string " << ( it->first );
  ecpp_stream << ";";
 }else{
  ecpp_stream << "\n  " << ( it->second );
  ecpp_stream << " " << ( it->first );
  ecpp_stream << ";";
 };
  ecpp_stream << "";
 };
  ecpp_stream << "\n};\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
  std::string superclass;
  std::map<std::string, std::string>& properties;
};

void render_scaffolds_mongodb_model_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsMongodbModelHpp(renderer, target, vars).render();
}