#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectAppMainCpp : public Crails::Template
{
public:
  render_ProjectAppMainCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    with_action(Crails::cast<bool>(vars, "with_action",  true)), 
    with_cookies(Crails::cast<bool>(vars, "with_cookies",  false))
  {}

  void render()
  {
ecpp_stream << "#include \"config/renderers.hpp\"\n#include \"config/server.hpp\"";
 if (with_action){
  ecpp_stream << "\n#include <crails/router.hpp>";
 };
  ecpp_stream << "";
 if (with_cookies){
  ecpp_stream << "\n#include <crails/cipher.hpp>";
 };
  ecpp_stream << "\n\nusing namespace std;\nusing namespace Crails;\n\nint main(int argc, const char **argv)\n{\n  SingletonInstantiator<ApplicationServer> server;\n  SingletonInstantiator<ApplicationRenderers> renderers;";
 if (with_action){
  ecpp_stream << "\n  SingletonInstantiator<Router> router;\n\n  router->initialize();";
 };
  ecpp_stream << "\n  // Initializers";
 if (with_cookies){
  ecpp_stream << "\n  Cipher::initialize();";
 };
  ecpp_stream << "\n  // Application loop\n  server->launch(argc, argv);\n\n  return 0;\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  bool with_action;
  bool with_cookies;
};

void render_project_app_main_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectAppMainCpp(renderer, target, vars).render();
}