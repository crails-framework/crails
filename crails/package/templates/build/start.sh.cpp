#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_PackageStartSh : public Crails::Template
{
public:
  render_PackageStartSh(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    application_port(Crails::cast< unsigned short >(vars, "application_port",  80)), 
    application_host(Crails::cast< std::string >(vars, "application_host",  "0.0.0.0")), 
    application_name(Crails::cast< std::string >(vars, "application_name")), 
    bin_directory(Crails::cast< std::string >(vars, "bin_directory")), 
    lib_directory(Crails::cast< std::string >(vars, "lib_directory")), 
    share_directory(Crails::cast< std::string >(vars, "share_directory")), 
    runtime_path(Crails::cast< std::string >(vars, "runtime_path",  "/var/lib/" + application_name))
  {}

  void render()
  {
    ecpp_stream.reserve(2252);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#!/bin/sh -ex\n\nexport APPLICATION_BIN=\"$(cd \"$( dirname \"$0\" )\" && pwd)\"\n\nif [ -z \"$VAR_DIRECTORY\" ]    ; then export VAR_DIRECTORY=\"" << ( runtime_path );
  ecpp_stream << "\" ; fi\nif [ -z \"$APPLICATION_HOST\" ] ; then export APPLICATION_HOST=\"" << ( application_host );
  ecpp_stream << "\" ; fi\nif [ -z \"$APPLICATION_PORT\" ] ; then export APPLICATION_PORT=\"" << ( application_port );
  ecpp_stream << "\" ; fi\nif [ -z \"$APPLICATION_NAME\" ] ; then export APPLICATION_NAME=\"" << ( application_name );
  ecpp_stream << "\" ; fi\nif [ -z \"$PID_FILE\" ]         ; then export PID_FILE=\"/tmp/$APPLICATION_NAME.pid\" ; fi\n\ncd \"$VAR_DIRECTORY\"\n\nexport LD_LIBRARY_PATH=\"$LD_LIBRARY_PATH:" << ( lib_directory );
  ecpp_stream << ":" << ( bin_directory );
  ecpp_stream << "\"\nexport PUBLIC_PATH=\"" << ( share_directory );
  ecpp_stream << "/public\"\n\nexec \"" << ( bin_directory );
  ecpp_stream << "/server\" \\\n  --hostname \"$APPLICATION_HOST\" \\\n  --port     \"$APPLICATION_PORT\" \\\n  --pidfile  \"$PID_FILE\" \\\n  --log      \"/var/log/$APPLICATION_NAME/event.log\"\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  unsigned short application_port;
  std::string application_host;
  std::string application_name;
  std::string bin_directory;
  std::string lib_directory;
  std::string share_directory;
  std::string runtime_path;
};

void render_package_start_sh(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_PackageStartSh(renderer, target, vars).render();
}