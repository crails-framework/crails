#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ScaffoldsTaskBuildfile : public Crails::Template
{
public:
  render_ScaffoldsTaskBuildfile(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    project_name(Crails::cast< std::string >(vars, "project_name")), 
    task_name(Crails::cast< std::string >(vars, "task_name"))
  {}

  void render()
  {
ecpp_stream << "import libs += lib" << ( project_name );
  ecpp_stream << "%lib{" << ( project_name );
  ecpp_stream << "}\n\nexe{task}: {hxx ixx txx cxx}{**} $libs\n\ncxx.poptions =+ \"-I$out_root\" \"-I$src_root\"\ncxx.poptions =+ \"-Wall\" \"-pedantic\"\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string project_name;
  std::string task_name;
};

void render_scaffolds_task_buildfile(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsTaskBuildfile(renderer, target, vars).render();
}