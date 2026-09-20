#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ScaffoldsViewNewHtml : public Crails::Template
{
public:
  render_ScaffoldsViewNewHtml(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast< std::string >(vars, "classname")), 
    header(Crails::cast< std::string >(vars, "header")), 
    view_path(Crails::cast< std::string >(vars, "view_path"))
  {}

  void render()
  {
    ecpp_stream.reserve(548);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#include \"" << ( header );
  ecpp_stream << "\"\n" << ( classname );
  ecpp_stream << " model;\n// END LINKING\n<h1>New " << ( classname );
  ecpp_stream << "</h1>\n\n<%= partial(\"" << ( view_path );
  ecpp_stream << "/form\", {{\"model\", &model}}) %>\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  std::string classname;
  std::string header;
  std::string view_path;
};

void render_scaffolds_view_new_html(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsViewNewHtml(renderer, target, vars).render();
}