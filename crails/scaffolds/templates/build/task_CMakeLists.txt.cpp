#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ScaffoldsTaskCmakelistsTxt : public Crails::Template
{
public:
  render_ScaffoldsTaskCmakelistsTxt(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    task_name(Crails::cast< std::string >(vars, "task_name"))
  {}

  void render()
  {
    ecpp_stream.reserve(712);
    // BEGIN TEMPLATE BODY
ecpp_stream << "cmake_minimum_required(VERSION 3.5)\n\nfile(GLOB_RECURSE task_files\n     *.cpp *.cxx)\n\nadd_executable(" << ( task_name );
  ecpp_stream << " ${task_files})\n\ntarget_link_libraries(" << ( task_name );
  ecpp_stream << " ${dependencies})\n\nset_target_properties(" << ( task_name );
  ecpp_stream << " PROPERTIES OUTPUT_NAME \"task\")\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
  std::string task_name;
};

void render_scaffolds_task_cmakelists_txt(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsTaskCmakelistsTxt(renderer, target, vars).render();
}