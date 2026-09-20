#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_PackageStopSh : public Crails::Template
{
public:
  render_PackageStopSh(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    application_name(Crails::cast< std::string >(vars, "application_name"))
  {}

  void render()
  {
    ecpp_stream.reserve(866);
    // BEGIN TEMPLATE BODY
ecpp_stream << "#!/bin/sh -ex\n\nif [ -z \"$APPLICATION_NAME\" ] ; then export APPLICATION_NAME=\"" << ( application_name );
  ecpp_stream << "\" ; fi\nif [ -z \"$PID_FILE\" ]         ; then export PID_FILE=\"/tmp/$APPLICATION_NAME.pid\" ; fi\n\nif [ -f \"$PID_FILE\" ] ; then\n  pid=`cat \"$PID_FILE\"`\n  kill -INT $pid\n  sleep 1\n  while [ $? -ne 0 ] ; do\n    kill -TERM $pid\n  done\n  rm \"$PID_FILE\"\nelse\n  exit -1\nfi\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  std::string application_name;
};

void render_package_stop_sh(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_PackageStopSh(renderer, target, vars).render();
}