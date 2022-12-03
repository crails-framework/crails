#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ProjectPublicIndexHtml : public Crails::Template
{
public:
  ProjectPublicIndexHtml(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "<html>\n  <head>\n    <title>Crails Application</title>\n    <style>\n      body {\n        padding: 0;\n        margin: 0;\n        height: 100%;\n        width: 100%;\n        text-align: center;\n        font-family: Arial;\n	background-color: #ced5ff;\n      }\n\n      .crails-logo {\n        position: relative;\n        background-color: #ff0000;\n	color: white;\n	font-weight: bold;\n	font-size: 2em;\n	width: 100px;\n	margin: 50px auto 25px;\n	padding: 5px 0 15px;\n	line-height: 0.8em;\n	border-top: 10px #f10000 solid;\n      }\n\n      .crails-logo-layer {\n        position: absolute;\n        background-color: #d10000;\n        top: 0;\n        bottom: 0;\n        left: 0;\n        width: 50%;\n        opacity: 0.3;\n      }\n\n      .crails-logo, .crails-logo-layer { border-bottom-left-radius: 60px; }\n      .crails-logo { border-bottom-right-radius: 60px; }\n    </style>\n  </head>\n  <body>\n    <div class=\"crails-logo\">\n      <div class=\"crails-logo-layer\"></div>\n      C<br/>\n      +<br/>\n      +<br/>\n    </div>\n    <h1>Welcome to your new Crails Application</h1>\n  </body>\n</html>\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_project_public_index_html(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  ProjectPublicIndexHtml(renderer, target, vars).render();
}