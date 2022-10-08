#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class ScaffoldsSpecTestCpp : public Crails::Template
{
public:
  ScaffoldsSpecTestCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars), 
    header(Crails::cast<std::string>(vars, "header")), 
    classname(Crails::cast<std::string>(vars, "classname"))
  {}

  std::string render()
  {
ecpp_stream << "" << ( classname );
  ecpp_stream << "::" << ( classname );
  ecpp_stream << "()\n{\n  before(std::bind(&" << ( classname );
  ecpp_stream << "::before_all, this));\n  after(std::bind(&" << ( classname );
  ecpp_stream << "::after_all, this));\n\n  describe(\"name of a method\", [this]()\n  {\n    it(\"fails\", [this]()\n    {\n      EXPECT(1, ==, 2);\n    });\n  });\n}\n\nvoid " << ( classname );
  ecpp_stream << "::before_all()\n{\n}\n\nvoid " << ( classname );
  ecpp_stream << "::after_all()\n{\n}\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
  std::string header;
  std::string classname;
};

std::string render_scaffolds_spec_test_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return ScaffoldsSpecTestCpp(renderer, vars).render();
}