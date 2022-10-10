#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ScaffoldsSpecTestHpp : public Crails::Template
{
public:
  ScaffoldsSpecTestHpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    classname(Crails::cast<std::string>(vars, "classname"))
  {}

  std::string render()
  {
ecpp_stream << "#pragma once\n#include <crails/tests/helper.hpp>\n\nclass " << ( classname );
  ecpp_stream << " : public Crails::Tests::Helper\n{\npublic:\n  " << ( classname );
  ecpp_stream << "();\n\nprivate:\n  void before_all();\n  void after_all();\n};\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string classname;
};

std::string render_scaffolds_spec_test_hpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldsSpecTestHpp(renderer, vars).render();
}