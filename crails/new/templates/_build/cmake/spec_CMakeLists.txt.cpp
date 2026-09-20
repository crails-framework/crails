#include <crails/template_stream.hpp>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ProjectSpecCmakelistsTxt : public Crails::Template
{
public:
  render_ProjectSpecCmakelistsTxt(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
    ecpp_stream.reserve(794);
    // BEGIN TEMPLATE BODY
ecpp_stream << "cmake_minimum_required(VERSION 3.5)\n\nfile(GLOB_RECURSE tests_files *.cpp *.cxx)\n\nadd_executable(tests ${tests_files})\n\nif (CRAILS_TESTS_SELENIUM_FOUND)\n  set(CMAKE_CXX_FLAGS \"${CMAKE_CXX_FLAGS} -DTESTS_WITH_SELENIUM\")\nendif()\n\ntarget_link_libraries(tests crails-app ${CRAILS_TESTS_LIBRARIES} ${CRAILS_TESTS_SELENIUM_LIBRARIES} ${dependencies} ${tests_dependencies})\n";
    // END TEMPLATE BODY
    std::string _out_buffer = std::move(ecpp_stream).extract();
    this->target.set_body(this->apply_post_render_filters(std::move(_out_buffer)));
  }
private:
  Crails::TemplateStream ecpp_stream;
};

void render_project_spec_cmakelists_txt(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ProjectSpecCmakelistsTxt(renderer, target, vars).render();
}