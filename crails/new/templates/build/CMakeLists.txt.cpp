#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <algorithm>
#include <list>

class render_ProjectCmakelistsTxt : public Crails::Template
{
public:
  render_ProjectCmakelistsTxt(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    crails_version(Crails::cast<std::string>(vars, "crails_version")), 
    project_name(Crails::cast<std::string>(vars, "project_name")), 
    configuration_type(Crails::cast<std::string>(vars, "configuration_type",  "full")), 
    cpp_version(Crails::cast<std::string>(vars, "cpp_version",  "c++17")), 
    formats(reinterpret_cast<std::list<std::string>&>(*Crails::cast<std::list<std::string>*>(vars, "formats"))), 
    plugins(reinterpret_cast<std::list<std::string>&>(*Crails::cast<std::list<std::string>*>(vars, "plugins")))
  {}

  void render()
  {
ecpp_stream << "cmake_minimum_required(VERSION 3.5)\n\nproject(" << ( project_name );
  ecpp_stream << ")\n\nset(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -std=" << ( cpp_version );
  ecpp_stream << " -Wall -Wno-unknown-pragmas -Wno-deprecated-declarations -pedantic\")\nSET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)\n\nfind_package(PkgConfig)\npkg_check_modules(CRAILS REQUIRED";
 for (auto plugin : plugins){
  ecpp_stream << " " << ( plugin );
  ecpp_stream << ">=" << ( crails_version );
  ecpp_stream << "";
 };
  ecpp_stream << ")\npkg_check_modules(CRAILS_TESTS QUIET libcrails-tests>=" << ( crails_version);
  ecpp_stream << ")\npkg_check_modules(CRAILS_TESTS_SELENIUM QUIET libcrails-selenium>=" << ( crails_version );
  ecpp_stream << ")\n\ninclude_directories(/usr/local/include ${CRAILS_INCLUDE_DIRS} ./app)\nlist(APPEND CMAKE_INSTALL_RPATH \"$ORIGIN\")\nlist(APPEND CMAKE_INSTALL_RPATH ${CRAILS_LIBRARY_DIRS})\n\nfile(GLOB server_files exe/server/*.cpp)\nfile(GLOB_RECURSE crails_app app/*.cpp app/*.cxx)\n\nadd_library(crails-app SHARED ${crails_app})\nadd_executable(server ${server_files})\n\nset(dependencies crails-app\n                 ${CRAILS_LINK_LIBRARIES}\n                 pthread dl crypto ssl)\n# Custom dependencies (do not modify this line)\n\ntarget_link_libraries(server ${dependencies})\n\nif (CRAILS_TESTS_FOUND)\n  add_subdirectory(spec/driver)\nendif()\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
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

void render_project_cmakelists_txt(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectCmakelistsTxt(renderer, target, vars).render();
}