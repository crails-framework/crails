#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ScaffoldsViewEditHtml : public Crails::Template
{
public:
  render_ScaffoldsViewEditHtml(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast<std::string>(vars, "classname")), 
    header(Crails::cast<std::string>(vars, "header")), 
    view_path(Crails::cast<std::string>(vars, "view_path"))
  {}

  void render()
  {
ecpp_stream << "#include \"" << ( header );
  ecpp_stream << "\"\n" << ( classname );
  ecpp_stream << "& @model;\n// END LINKING\n<h1>Edit " << ( classname );
  ecpp_stream << "</h1>\n\n<%= partial(\"" << ( view_path );
  ecpp_stream << "/form\", {{\"model\", &model}}) %>\n\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
  std::string header;
  std::string view_path;
};

void render_scaffolds_view_edit_html(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsViewEditHtml(renderer, target, vars).render();
}