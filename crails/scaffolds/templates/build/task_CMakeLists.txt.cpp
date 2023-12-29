#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ScaffoldsTaskCmakelistsTxt : public Crails::Template
{
public:
  render_ScaffoldsTaskCmakelistsTxt(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    task_name(Crails::cast<std::string>(vars, "task_name"))
  {}

  void render()
  {
ecpp_stream << "cmake_minimum_required(VERSION 3.0)\n\nfile(GLOB_RECURSE task_files\n     *.cpp *.cxx)\n\nadd_executable(" << ( task_name );
  ecpp_stream << " ${task_files})\n\ntarget_link_libraries(" << ( task_name );
  ecpp_stream << " ${dependencies})\n\nset_target_properties(" << ( task_name );
  ecpp_stream << " PROPERTIES OUTPUT_NAME \"task\")\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string task_name;
};

void render_scaffolds_task_cmakelists_txt(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsTaskCmakelistsTxt(renderer, target, vars).render();
}