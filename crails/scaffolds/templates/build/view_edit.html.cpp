#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ScaffoldsViewEditHtml : public Crails::Template
{
public:
  ScaffoldsViewEditHtml(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    classname(Crails::cast<std::string>(vars, "classname")), 
    header(Crails::cast<std::string>(vars, "header")), 
    view_path(Crails::cast<std::string>(vars, "view_path"))
  {}

  std::string render()
  {
ecpp_stream << "#include \"" << ( header );
  ecpp_stream << "\"\n" << ( classname );
  ecpp_stream << "& @model;\n// END LINKING\n<h1>Edit " << ( classname );
  ecpp_stream << "</h1>\n\n<%= partial(\"" << ( view_path );
  ecpp_stream << "/form\", {{\"model\", &model}}) %>\n\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
  std::string header;
  std::string view_path;
};

std::string render_scaffolds_view_edit_html(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldsViewEditHtml(renderer, vars).render();
}