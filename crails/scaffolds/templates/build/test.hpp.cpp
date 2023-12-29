#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ScaffoldsSpecTestHpp : public Crails::Template
{
public:
  render_ScaffoldsSpecTestHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast<std::string>(vars, "classname"))
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include <crails/tests/helper.hpp>\n\nclass " << ( classname );
  ecpp_stream << " : public Crails::Tests::Helper\n{\npublic:\n  " << ( classname );
  ecpp_stream << "();\n\nprivate:\n  void before_all();\n  void after_all();\n};\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
};

void render_scaffolds_spec_test_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsSpecTestHpp(renderer, target, vars).render();
}