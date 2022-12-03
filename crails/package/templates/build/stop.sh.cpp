#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class PackageStopSh : public Crails::Template
{
public:
  PackageStopSh(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    application_name(Crails::cast<std::string>(vars, "application_name"))
  {}

  void render()
  {
ecpp_stream << "#!/bin/sh -ex\n\nif [ -z \"$APPLICATION_NAME\" ] ; then export APPLICATION_NAME=\"" << ( application_name );
  ecpp_stream << "\" ; fi\nif [ -z \"$PID_FILE\" ]         ; then export PID_FILE=\"/tmp/$APPLICATION_NAME.pid %>\" ; fi\n\nif [ -f \"$PID_FILE\" ] ; then\n  pid=`cat \"$PID_FILE\"`\n  kill -INT $pid\n  sleep 1\n  while [ $? -ne 0 ] ; do\n    kill -TERM $pid\n  done\n  rm \"$PID_FILE\"\nelse\n  exit -1\nfi\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  std::string application_name;
};

void render_package_stop_sh(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  PackageStopSh(renderer, target, vars).render();
}