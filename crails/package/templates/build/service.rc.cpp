#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/semantics.hpp>
using namespace Crails;

class PackageServiceRc : public Crails::Template
{
public:
  PackageServiceRc(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    application_name(Crails::cast<std::string>(vars, "application_name")), 
    application_user(Crails::cast<std::string>(vars, "application_user",  "")), 
    bin_directory(Crails::cast<std::string>(vars, "bin_directory")), 
    share_directory(Crails::cast<std::string>(vars, "share_directory")), 
    runtime_path(Crails::cast<std::string>(vars, "runtime_path",  "/var/" + application_name)), 
    pidfile(Crails::cast<std::string>(vars, "pidfile")), 
    environment_file( share_directory + "/environment")
  {}

  std::string render()
  {
ecpp_stream << "#!/bin/sh\n#\n\n# PROVIDE: " << ( underscore(application_name) );
  ecpp_stream << "\n# REQUIRE: bar_service_required_to_precede_foo\n\n. /etc/rc.subr\n\nname=\"" << ( underscore(application_name) );
  ecpp_stream << "\"\nrcvar=" << ( underscore(application_name) );
  ecpp_stream << "_enable\ncommand=\"" << ( underscore(application_name) );
  ecpp_stream << "_start\"\nstop_cmd=\"" << ( underscore(application_name) );
  ecpp_stream << "_stop\"\n" << ( underscore(application_name) );
  ecpp_stream << "_start()\n{\n  cd \"" << ( runtime_path );
  ecpp_stream << "\"\n  su -c \"" << ( bin_directory );
  ecpp_stream << "/start.sh\" - " << ( application_user );
  ecpp_stream << "\n}\n" << ( underscore(application_name) );
  ecpp_stream << "_stop()\n{\n  " << ( bin_directory );
  ecpp_stream << "/stop.sh\n}\n\nload_rc_config $name\nrun_rc_command \"$1\"\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string application_name;
  std::string application_user;
  std::string bin_directory;
  std::string share_directory;
  std::string runtime_path;
  std::string pidfile;
  std::string environment_file;
};

std::string render_package_service_rc(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return PackageServiceRc(renderer, vars).render();
}