#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include "../../../../project_configuration.hpp"

class render_Build2BuildRootBuild : public Crails::Template
{
public:
  render_Build2BuildRootBuild(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    cpp_version(Crails::cast<std::string>(vars, "cpp_version",  "latest")), 
    configuration(reinterpret_cast<ProjectConfiguration&>(*Crails::cast<ProjectConfiguration*>(vars, "configuration")))
  {}

  void render()
  {
ecpp_stream << "# Uncomment to suppress warnings coming from external libraries\n#cxx.internal.scope = current\n\ncxx.std = " << ( cpp_version );
  ecpp_stream << "\n\nusing cxx\n\nhxx{*}: extension = " << ( configuration.source_extension(HeaderExt) );
  ecpp_stream << "\nixx{*}: extension = " << ( configuration.source_extension(InlineExt) );
  ecpp_stream << "\ntxx{*}: extension = " << ( configuration.source_extension(TemplateExt) );
  ecpp_stream << "\ncxx{*}: extension = " << ( configuration.source_extension(SourceExt) );
  ecpp_stream << "\n\n# The test target for cross-testing (running tests under Wine, etc).\ntest.target = $cxx.target\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string cpp_version;
  ProjectConfiguration& configuration;
};

void render_build2_build_root_build(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_Build2BuildRootBuild(renderer, target, vars).render();
}