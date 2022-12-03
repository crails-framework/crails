#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>
#include <crails/utils/string.hpp>

class ScaffoldsViewFormHtml : public Crails::Template
{
public:
  ScaffoldsViewFormHtml(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    numerical_types( {"int","unsigned int","short","unsigned short","long","long long","unsigned long","unsigned long long","float","double"}), 
    classname(Crails::cast<std::string>(vars, "classname")), 
    resource_name(Crails::cast<std::string>(vars, "resource_name",  Crails::underscore(classname))), 
    header(Crails::cast<std::string>(vars, "header")), 
    route(Crails::cast<std::string>(vars, "route")), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast<std::map<std::string, std::string>*>(vars, "properties")))
  {}

  void render()
  {
ecpp_stream << "#include \"" << ( header );
  ecpp_stream << "\"\n#include <crails/html_form_builder.hpp>\n\nusing namespace std;\n" << ( classname );
  ecpp_stream << "& @model;\nCrails::FormBuilder<" << ( classname );
  ecpp_stream << "> form = Crails::FormBuilder(this, model);\nstring action = \"/" << ( route );
  ecpp_stream << "\";\n// END LINKING\n<%= form_for(model, \"/" << ( route );
  ecpp_stream << "\") yields %>";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\n  <div class=\"form-group\">\n    <%= form.label_for(\"" << ( it->first );
  ecpp_stream << "\") %>";
  if (it->second == "std::string"){
  ecpp_stream << "\n    <%= form.text_field(\"" << ( it->first );
  ecpp_stream << "\", &" << ( classname );
  ecpp_stream << "::get_" << ( it->first );
  ecpp_stream << ") %>";
 }else if (it->second == "std::time_t"){
  ecpp_stream << "\n    <%= form.date_field(\"" << ( it->first );
  ecpp_stream << "\", &" << ( classname );
  ecpp_stream << "::get_" << ( it->first );
  ecpp_stream << ") %>";
 }else if (std::find(numerical_types.begin(), numerical_types.end(), std::string(it->second)) != numerical_types.end()){
  ecpp_stream << "";
    if (it->second == "double" || it->second == "float"){
  ecpp_stream << "\n    <%= form.number_field(\"" << ( it->first );
  ecpp_stream << "\", &" << ( classname );
  ecpp_stream << "::get_" << ( it->first );
  ecpp_stream << ", {{\"step\",\"0.1\"}}) %>";
 }else{
  ecpp_stream << "\n    <%= form.number_field(\"" << ( it->first );
  ecpp_stream << "\", &" << ( classname );
  ecpp_stream << "::get_" << ( it->first );
  ecpp_stream << ") %>";
 };
  ecpp_stream << "";
 }else if (it->second == "Crails::Password"){
  ecpp_stream << "\n    <%= form.password_field(\"" << ( it->first );
  ecpp_stream << "\", string()) %>";
 };
  ecpp_stream << "\n  </div>";
 };
  ecpp_stream << "\n  <button type=\"submit\" class=\"btn btn-primary\">Save</button>\n<% yields-end %>\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  std::vector<std::string> numerical_types;
  std::string classname;
  std::string resource_name;
  std::string header;
  std::string route;
  std::map<std::string, std::string>& properties;
};

void render_scaffolds_view_form_html(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ScaffoldsViewFormHtml(renderer, target, vars).render();
}