#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <list>
using namespace std;

class DockerBuildEnvironment : public Crails::Template
{
public:
  DockerBuildEnvironment(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    with_odb(Crails::cast<bool>(vars, "with_odb",  false)), 
    with_comet(Crails::cast<bool>(vars, "with_comet",  false)), 
    packages(reinterpret_cast<list<string>&>(*Crails::cast<list<string>*>(vars, "packages"))), 
    sql_backends(reinterpret_cast<list<string>&>(*Crails::cast<list<string>*>(vars, "sql_backends")))
  {}

  std::string render()
  {
ecpp_stream << "crails_packages=(";
 for (const auto& package : packages){
  ecpp_stream << "\n  " << ( package );
  ecpp_stream << "";
 };
  ecpp_stream << ")";
 if (with_odb){
  ecpp_stream << "\n\nsql_backends=(";
 for (const auto& backend : sql_backends){
  ecpp_stream << "\n  " << ( backend );
  ecpp_stream << "";
 };
  ecpp_stream << "\n)";
 };
  ecpp_stream << "";
 if (with_comet){
  ecpp_stream << "\n\ncomet_packages=(\n  libcrails-semantics\n  libcrails-router\n  libcomet\n)";
 };
  ecpp_stream << "\n\nsystem_packages=(\n  ?sys:libboost-any\n  ?sys:libboost-beast\n  ?sys:libboost-config\n  ?sys:libboost-filesystem\n  ?sys:libboost-fusion\n  ?sys:libboost-json\n  ?sys:libboost-lexical-cast\n  ?sys:libboost-program-options\n  ?sys:libboost-property-tree\n  ?sys:libboost-process\n  ?sys:libboost-random\n  ?sys:libssl";
 if (with_odb){
  ecpp_stream << "\n  ?sys:libsqlite3\n  ?sys:libpq\n  ?sys:libmysqlclient";
 };
  ecpp_stream << "\n)\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  bool with_odb;
  bool with_comet;
  list<string>& packages;
  list<string>& sql_backends;
};

std::string render_docker_build_environment(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return DockerBuildEnvironment(renderer, vars).render();
}