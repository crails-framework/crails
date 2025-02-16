#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>

class render_ScaffoldsViewShowHtml : public Crails::Template
{
public:
  render_ScaffoldsViewShowHtml(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast< std::string >(vars, "classname")), 
    header(Crails::cast< std::string >(vars, "header")), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast< std::map<std::string, std::string>* >(vars, "properties")))
  {}

  void render()
  {
ecpp_stream << "#include \"" << ( header );
  ecpp_stream << "\"\n" << ( classname );
  ecpp_stream << "& @model;\n// END LINKING";
 if (properties.find("name") != properties.end()){
  ecpp_stream << "\n<h1><%= model.get_" << ( properties.find("name")->first );
  ecpp_stream << "() %></h1>\n<h2>" << ( classname );
  ecpp_stream << "</h2>";
 }else{
  ecpp_stream << "\n<h1>" << ( classname );
  ecpp_stream << "</h1>";
 };
  ecpp_stream << "\n";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\n<div class=\"attribute-group\">\n  <label>" << ( it->first );
  ecpp_stream << ":</label>\n  <%= model.get_" << ( it->first );
  ecpp_stream << "() %>\n</div>";
 };
  ecpp_stream << "\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
  std::string header;
  std::map<std::string, std::string>& properties;
};

void render_scaffolds_view_show_html(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsViewShowHtml(renderer, target, vars).render();
}