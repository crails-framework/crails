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
    with_cookies(Crails::cast<bool>(vars, "with_cookies",  false)), 
    with_databases(Crails::cast<bool>(vars, "with_databases",  false))
  {}

  void render()
  {
ecpp_stream << "#include <crails/logger.hpp>\n#include \"config/renderers.hpp\"\n#include \"config/server.hpp\"";
 if (with_databases){
  ecpp_stream << "\n#include \"config/databases.hpp\"";
 };
  ecpp_stream << "";
 if (with_action){
  ecpp_stream << "\n#include <crails/router.hpp>";
 };
  ecpp_stream << "";
 if (with_cookies){
  ecpp_stream << "\n#include <crails/cipher.hpp>";
 };
  ecpp_stream << "\n\nusing namespace std;\nusing namespace Crails;\n\nint main(int argc, const char **argv)\n{\n  SingletonInstantiator<ApplicationServer> server;\n  SingletonInstantiator<ApplicationRenderers> renderers;";
 if (with_databases){
  ecpp_stream << "\n  SingletonInstantiator<ApplicationDatabases> databases;\n";
 };
  ecpp_stream << "";
 if (with_action){
  ecpp_stream << "\n  SingletonInstantiator<Router> router;\n\n  router->initialize();";
 };
  ecpp_stream << "\n  // Initializers";
 if (with_cookies){
  ecpp_stream << "\n  Cipher::initialize();";
 };
  ecpp_stream << "\n  // Application loop\n  try\n  {\n    server->launch(argc, argv);\n  }\n  catch (const exception& error)\n  {\n    logger << Logger::Error << \">> Server crashed: \" << error.what() << Logger::endl;\n    return -1;\n  }\n  return 0;\n}\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  bool with_action;
  bool with_cookies;
  bool with_databases;
};

void render_project_app_main_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectAppMainCpp(renderer, target, vars).render();
}