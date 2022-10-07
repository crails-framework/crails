#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class PackageSystemdService : public Crails::Template
{
public:
  PackageSystemdService(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    application_name(Crails::cast<std::string>(vars, "application_name")), 
    application_user(Crails::cast<std::string>(vars, "application_user",  "")), 
    application_group(Crails::cast<std::string>(vars, "application_group",  "")), 
    bin_directory(Crails::cast<std::string>(vars, "bin_directory")), 
    share_directory(Crails::cast<std::string>(vars, "share_directory")), 
    runtime_path(Crails::cast<std::string>(vars, "runtime_path",  "/var/" + application_name))
  {}

  std::string render()
  {
ecpp_stream << "[Unit]\nDescription=" << ( application_name );
  ecpp_stream << " Web Application\nAfter=network.target\n\n[Service]\nWorkingDirectory=" << ( runtime_path );
  ecpp_stream << "";
 if (application_user.length() > 0){
  ecpp_stream << "\nUser=" << ( application_user );
  ecpp_stream << "";
 };
  ecpp_stream << "";
 if (application_group.length() > 0){
  ecpp_stream << "\nGroup=" << ( application_group );
  ecpp_stream << "";
 };
  ecpp_stream << "\nExecStart=" << ( bin_directory );
  ecpp_stream << "/start.sh\nExecStop=" << ( bin_directory );
  ecpp_stream << "/stop.sh\nPIDFile=/tmp/crails.pid\nRestart=always\n\n[Install]\nWantedBy=multi-user.target\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string application_name;
  std::string application_user;
  std::string application_group;
  std::string bin_directory;
  std::string share_directory;
  std::string runtime_path;
};

std::string render_package_systemd_service(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return PackageSystemdService(renderer, vars).render();
}