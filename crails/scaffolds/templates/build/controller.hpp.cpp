#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
using namespace std;

class render_ScaffoldsControllerHpp : public Crails::Template
{
public:
  render_ScaffoldsControllerHpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast<string>(vars, "classname")), 
    parent_class(Crails::cast<string>(vars, "parent_class",  "ApplicationController")), 
    parent_header(Crails::cast<string>(vars, "parent_header",  "app/controllers/application.hpp")), 
    model_class(Crails::cast<string>(vars, "model_class",  "")), 
    model_header(Crails::cast<string>(vars, "model_header",  "")), 
    resource_scaffold(Crails::cast<bool>(vars, "resource_scaffold",  false)), 
    crud_scaffold(Crails::cast<bool>(vars, "crud_scaffold",  false))
  {}

  void render()
  {
ecpp_stream << "#pragma once\n#include \"" << ( parent_header );
  ecpp_stream << "\"";
 if (model_class.length() > 0){
  ecpp_stream << "\n#include \"" << ( model_header );
  ecpp_stream << "\"\n#include <crails/odb/controller.hpp>\n\nclass " << ( model_class );
  ecpp_stream << ";";
 };
  ecpp_stream << "\n\nclass " << ( classname );
  ecpp_stream << " : public " << ( parent_class );
  ecpp_stream << "\n{\npublic:\n  " << ( classname );
  ecpp_stream << "(Crails::Context&);\n\n  void initialize() override;\n  void finalize() override;";
 if (resource_scaffold || crud_scaffold){
  ecpp_stream << "\n  void index();\n  void show();\n  void create();\n  void update();\n  void destroy();";
 if (resource_scaffold){
  ecpp_stream << "\n  void new_();\n  void edit();";
 };
  ecpp_stream << "";
 };
  ecpp_stream << "\nprotected:";
 if (model_class.length() > 0){
  ecpp_stream << "\n  std::shared_ptr<" << ( model_class );
  ecpp_stream << "> find_model(Crails::Odb::id_type id);\n  void require_model(Crails::Odb::id_type id);\n  void find_list();\n\n  std::shared_ptr<" << ( model_class );
  ecpp_stream << "> model;\n  std::vector<" << ( model_class );
  ecpp_stream << "> model_list;";
 };
  ecpp_stream << "\n};\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
  string classname;
  string parent_class;
  string parent_header;
  string model_class;
  string model_header;
  bool resource_scaffold;
  bool crud_scaffold;
};

void render_scaffolds_controller_hpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsControllerHpp(renderer, target, vars).render();
}