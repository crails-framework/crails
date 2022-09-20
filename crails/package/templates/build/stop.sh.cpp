#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class PackageStopSh : public Crails::Template
{
public:
  PackageStopSh(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    pidfile(Crails::cast<std::string>(vars, "pidfile"))
  {}

  std::string render()
  {
ecpp_stream << "#!/bin/sh -ex\n\npidfile=\"" << ( pidfile );
  ecpp_stream << "\"\n\nif [ -f \"$pidfile\" ] ; then\n  pid=`cat \"$pidfile\"`\n  kill -INT $pid\n  sleep 1\n  while [ $? -ne 0 ] ; do\n    kill -TERM $pid\n  done\n  rm \"$pidfile\"\nelse\n  exit -1\nfi\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string pidfile;
};

std::string render_package_stop_sh(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return PackageStopSh(renderer, vars).render();
}