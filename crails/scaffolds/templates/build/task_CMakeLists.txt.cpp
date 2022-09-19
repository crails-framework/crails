#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ScaffoldsTaskCmakelistsTxt : public Crails::Template
{
public:
  ScaffoldsTaskCmakelistsTxt(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    task_name(Crails::cast<std::string>(vars, "task_name"))
  {}

  std::string render()
  {
ecpp_stream << "cmake_minimum_required(VERSION 3.0)\n\nfile(GLOB_RECURSE task_files\n     *.cpp *.cxx)\n\nadd_executable(" << ( task_name );
  ecpp_stream << " ${task_files})\n\ntarget_link_libraries(" << ( task_name );
  ecpp_stream << " ${dependencies})\n\nset_target_properties(" << ( task_name );
  ecpp_stream << " PROPERTIES OUTPUT_NAME \"task\")\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string task_name;
};

std::string render_scaffolds_task_cmakelists_txt(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldsTaskCmakelistsTxt(renderer, vars).render();
}