#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_ScaffoldsSpecTestCpp : public Crails::Template
{
public:
  render_ScaffoldsSpecTestCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    header(Crails::cast<std::string>(vars, "header")), 
    classname(Crails::cast<std::string>(vars, "classname"))
  {}

  void render()
  {
ecpp_stream << "" << ( classname );
  ecpp_stream << "::" << ( classname );
  ecpp_stream << "()\n{\n  before(std::bind(&" << ( classname );
  ecpp_stream << "::before_all, this));\n  after(std::bind(&" << ( classname );
  ecpp_stream << "::after_all, this));\n\n  describe(\"name of a method\", [this]()\n  {\n    it(\"fails\", [this]()\n    {\n      EXPECT(1, ==, 2);\n    });\n  });\n}\n\nvoid " << ( classname );
  ecpp_stream << "::before_all()\n{\n}\n\nvoid " << ( classname );
  ecpp_stream << "::after_all()\n{\n}\n";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  std::string header;
  std::string classname;
};

void render_scaffolds_spec_test_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsSpecTestCpp(renderer, target, vars).render();
}