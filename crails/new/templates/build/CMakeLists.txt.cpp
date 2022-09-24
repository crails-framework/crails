#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>
#include <list>

class ProjectCmakelistsTxt : public Crails::Template
{
public:
  ProjectCmakelistsTxt(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    crails_version(Crails::cast<std::string>(vars, "crails_version")), 
    project_name(Crails::cast<std::string>(vars, "project_name")), 
    configuration_type(Crails::cast<std::string>(vars, "configuration_type",  "full")), 
    cpp_version(Crails::cast<std::string>(vars, "cpp_version",  "c++17")), 
    formats(reinterpret_cast<std::list<std::string>&>(*Crails::cast<std::list<std::string>*>(vars, "formats"))), 
    plugins(reinterpret_cast<std::list<std::string>&>(*Crails::cast<std::list<std::string>*>(vars, "plugins")))
  {}

  std::string render()
  {
ecpp_stream << "cmake_minimum_required(VERSION 3.0)\n\nproject(" << ( project_name );
  ecpp_stream << ")\n\nset(CMAKE_CXX_FLAGS \"-std=" << ( cpp_version );
  ecpp_stream << " -Wall -Wno-unknown-pragmas -Wno-deprecated-declarations -pedantic\")\n\nfind_package(PkgConfig)\nfind_package(Boost COMPONENTS thread filesystem program_options random REQUIRED)\npkg_check_modules(CRAILS REQUIRED";
 for (auto plugin : plugins){
  ecpp_stream << " " << ( plugin );
  ecpp_stream << ">=" << ( crails_version );
  ecpp_stream << "";
 };
  ecpp_stream << ")\npkg_check_modules(CRAILS_TESTS QUIET libcrails-tests)\n\ninclude_directories(/usr/local/include ${Boost_INCLUDE_DIRS} ${CRAILS_INCLUDE_DIRS} .)\n\nfile(GLOB server_files app/main.cpp)\nfile(GLOB_RECURSE crails_app\n  app/routes.cpp\n  app/controllers/*.cpp app/controllers/*.cxx\n  app/models/*.cpp      app/models/*.cxx\n  app/views/*.cpp       app/views/*.cxx\n  modules/*.cpp         modules/*.cxx\n  config/*.cpp          config/*.cxx\n  lib/*.cpp             lib/*.cxx)\nlist(FILTER crails_app EXCLUDE REGEX \"app/client/.*\")\n\nadd_library(crails-app SHARED ${crails_app})\nadd_executable(server ${server_files})\n\nset(dependencies crails-app\n                 ${CRAILS_LINK_LIBRARIES}\n                 ${Boost_LIBRARIES}\n                 pthread dl crypto ssl)\n# Custom dependencies (do not modify this line)\n\ntarget_link_libraries(server ${dependencies})\n\nif (CRAILS_TESTS_FOUND)\n  file(GLOB_RECURSE tests_files  spec/*.cpp)\n  add_executable(tests ${tests_files})\n  target_link_libraries(tests crails-app ${CRAILS_TESTS_LIBRARIES} ${dependencies} ${tests_dependencies})\nendif()\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string crails_version;
  std::string project_name;
  std::string configuration_type;
  std::string cpp_version;
  std::list<std::string>& formats;
  std::list<std::string>& plugins;
};

std::string render_project_cmakelists_txt(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ProjectCmakelistsTxt(renderer, vars).render();
}