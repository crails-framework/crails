#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>
#include <crails/utils/string.hpp>

class ScaffoldsViewFormHtml : public Crails::Template
{
public:
  ScaffoldsViewFormHtml(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    classname(Crails::cast<std::string>(vars, "classname")), 
    resource_name(Crails::cast<std::string>(vars, "resource_name",  Crails::underscore(classname))), 
    header(Crails::cast<std::string>(vars, "header")), 
    route(Crails::cast<std::string>(vars, "route")), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast<std::map<std::string, std::string>*>(vars, "properties")))
  {}

  std::string render()
  {
ecpp_stream << "#include \"" << ( header );
  ecpp_stream << "\"\n#include <boost/lexical_cast.hpp>\n\nusing namespace std;\n" << ( classname );
  ecpp_stream << "& @model;\nstring method = model.is_persistent() ? \"put\" : \"post\";\nstring action = \"/" << ( route );
  ecpp_stream << "\";\n// END LINKING\n<% if (model.is_persistent()) { action += '/' + boost::lexical_cast<string>(model.get_id()); } -%>\n<%= form({{\"method\",method}, {\"action\",action}}) yields %>";
 for (auto it = properties.begin() ; it != properties.end() ; ++it){
  ecpp_stream << "\n  <div class=\"form-group\">\n    <%= tag(\"label\", {{\"for\",\"" << ( resource_name );
  ecpp_stream << "[" << ( it->first );
  ecpp_stream << "]\"}}) yields %>" << ( it->first );
  ecpp_stream << "<% yields-end %>\n    <%= tag(\"input\", {\n          {\"class\",\"form-control\"},\n          {\"type\",\"text\"},\n          {\"name\",\"" << ( resource_name );
  ecpp_stream << "[" << ( it->first );
  ecpp_stream << "]\"},\n          {\"value\",boost::lexical_cast<string>(model.get_" << ( it->first );
  ecpp_stream << "())}}\n        ) %>\n  </div>";
 };
  ecpp_stream << "\n  <button type=\"submit\" class=\"btn btn-primary\">Save</button>\n<% yields-end %>\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
  std::string resource_name;
  std::string header;
  std::string route;
  std::map<std::string, std::string>& properties;
};

std::string render_scaffolds_view_form_html(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldsViewFormHtml(renderer, vars).render();
}