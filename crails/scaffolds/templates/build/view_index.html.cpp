#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>

class ScaffoldsViewIndexHtml : public Crails::Template
{
public:
  ScaffoldsViewIndexHtml(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast<std::string>(vars, "classname")), 
    resource_name(Crails::cast<std::string>(vars, "resource_name",  Crails::underscore(classname))), 
    header(Crails::cast<std::string>(vars, "header")), 
    properties(reinterpret_cast<std::map<std::string, std::string>&>(*Crails::cast<std::map<std::string, std::string>*>(vars, "properties"))), 
    name_property(Crails::cast<std::string>(vars, "name_property",  "name")), 
    has_name( properties.find(name_property) != properties.end())
  {}

  void render()
  {
ecpp_stream << "#include \"" << ( header );
  ecpp_stream << "\"\n#include <boost/lexical_cast.hpp>\n\nusing namespace std;\n\nvector<" << ( classname );
  ecpp_stream << ">& @models;\nstring route = \"" << ( resource_name );
  ecpp_stream << "\";\n// END LINKING\n<h1>" << ( classname );
  ecpp_stream << " index</h1>\n\n<%= tag(\"a\", {{\"href\", '/' + route + \"/new\"}}) yields %>\n  New\n<% yields-end %>\n\n<table class=\"table\">\n  <thead>\n    <th>ID</th>";
 if (has_name){
  ecpp_stream << "\n    <th>" << ( name_property );
  ecpp_stream << "</th>";
 };
  ecpp_stream << "\n    <th></th>\n  </thead>\n  <tbody>\n<% for (const " << ( classname );
  ecpp_stream << "& model : models) do %>\n<%   const string id = boost::lexical_cast<string>(model.get_id()) %>\n    <tr>\n      <td><%= model.get_id() %></td>";
 if (has_name){
  ecpp_stream << "\n      <td><%= model.get_" << ( name_property );
  ecpp_stream << "() %></td>";
 };
  ecpp_stream << "\n      <td>\n        <%= tag(\"a\", {{\"href\", '/' + route + '/' + id}}) yields %>Show<% yend %>\n        <%= tag(\"a\", {{\"href\", '/' + route + '/' + id + \"/edit\"}}) yields %>Edit<% yend %>\n      </td>\n    </tr>\n<% end %>\n  </tbody>\n</table>\n\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
  std::string resource_name;
  std::string header;
  std::map<std::string, std::string>& properties;
  std::string name_property;
  bool has_name;
};

void render_scaffolds_view_index_html(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ScaffoldsViewIndexHtml(renderer, target, vars).render();
}