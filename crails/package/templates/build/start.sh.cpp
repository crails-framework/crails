#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class PackageStartSh : public Crails::Template
{
public:
  PackageStartSh(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    application_port(Crails::cast<unsigned short>(vars, "application_port",  3001)), 
    application_host(Crails::cast<std::string>(vars, "application_host",  "0.0.0.0")), 
    application_name(Crails::cast<std::string>(vars, "application_name")), 
    bin_directory(Crails::cast<std::string>(vars, "bin_directory")), 
    lib_directory(Crails::cast<std::string>(vars, "lib_directory")), 
    share_directory(Crails::cast<std::string>(vars, "share_directory")), 
    runtime_path(Crails::cast<std::string>(vars, "runtime_path",  "/var/" + application_name)), 
    pidfile(Crails::cast<std::string>(vars, "pidfile")), 
    environment_file( share_directory + "/environment")
  {}

  std::string render()
  {
ecpp_stream << "#!/bin/sh -ex\n\nif [ -f \"" << ( environment_file );
  ecpp_stream << "\" ] ; then\n  . \"" << ( environment_file );
  ecpp_stream << "\"\nfi\n\ncd \"" << ( runtime_path );
  ecpp_stream << "\"\n\nexport LD_LIBRARY_PATH=\"$LD_LIBRARY_PATH:" << ( lib_directory );
  ecpp_stream << ":" << ( bin_directory );
  ecpp_stream << "\"\nexport PUBLIC_PATH=\"" << ( share_directory );
  ecpp_stream << "/public\"\nif [ -z \"$APPLICATION_HOSTNAME\" ] ; then export APPLICATION_HOSTNAME=\"" << ( application_host );
  ecpp_stream << "\" ; fi\nif [ -z \"$APPLICATION_PORT\" ]     ; then export APPLICATION_PORT=\"" << ( application_port );
  ecpp_stream << "\" ; fi\n\nexec \"" << ( bin_directory );
  ecpp_stream << "/server\" \\\n  --hostname \"$APPLICATION_HOSTNAME\" \\\n  --port     \"$APPLICATION_PORT\" \\\n  --pidfile  \"" << ( pidfile );
  ecpp_stream << "\" \\\n  --log      \"/var/log/" << ( application_name );
  ecpp_stream << "/`date +%y-%m-%d_%Hh%Mm%S`_crails.log\"\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  unsigned short application_port;
  std::string application_host;
  std::string application_name;
  std::string bin_directory;
  std::string lib_directory;
  std::string share_directory;
  std::string runtime_path;
  std::string pidfile;
  std::string environment_file;
};

std::string render_package_start_sh(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return PackageStartSh(renderer, vars).render();
}