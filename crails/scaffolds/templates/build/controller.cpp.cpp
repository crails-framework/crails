#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"
#include <crails/utils/string.hpp>
using namespace std;

class render_ScaffoldsControllerCpp : public Crails::Template
{
public:
  render_ScaffoldsControllerCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars), 
    classname(Crails::cast<string>(vars, "classname")), 
    router_path(Crails::cast<string>(vars, "router_path")), 
    path(Crails::cast<string>(vars, "path")), 
    view_path(Crails::cast<string>(vars, "view_path")), 
    parent_class(Crails::cast<string>(vars, "parent_class",  "ApplicationController")), 
    model_class(Crails::cast<string>(vars, "model_class",  "")), 
    model_header(Crails::cast<string>(vars, "model_header",  "")), 
    id_type(Crails::cast<string>(vars, "id_type",  "")), 
    database_backend(Crails::cast<string>(vars, "database_backend",  "")), 
    resource_scaffold(Crails::cast<bool>(vars, "resource_scaffold",  false)), 
    crud_scaffold(Crails::cast<bool>(vars, "crud_scaffold",  false))
  {}

  void render()
  {
ecpp_stream << "#include \"" << ( path );
  ecpp_stream << ".hpp\"\n#include <crails/params.hpp>\n#include <crails/http_response.hpp>";
 if (model_header.length() > 0){
  ecpp_stream << "\n#include <crails/odb/to_vector.hpp>\n#include <boost/lexical_cast.hpp>";
   if (database_backend == "odb"){
  ecpp_stream << "\n#include \"autogen/odb/application-odb.hxx\"";
 };
  ecpp_stream << "\n#include \"" << ( model_header );
  ecpp_stream << "\"";
 };
  ecpp_stream << "\n\nusing namespace std;\n" << ( classname + "::" + classname );
  ecpp_stream << "(Crails::Context& context) : " << ( parent_class );
  ecpp_stream << "(context)\n{\n}\n\nvoid " << ( classname );
  ecpp_stream << "::initialize()\n{\n  " << ( parent_class );
  ecpp_stream << "::initialize();";
 if (model_class.length() > 0){
  ecpp_stream << "\n  string action_name = get_action_name();\n\n  if (response.sent()) return;\n  if (action_name == \"show\" || action_name == \"edit\" || action_name == \"update\" || action_name == \"destroy\")\n    require_model(params[\"id\"]);";
 };
  ecpp_stream << "\n}\n\nvoid " << ( classname );
  ecpp_stream << "::finalize()\n{\n  " << ( parent_class );
  ecpp_stream << "::finalize();\n}\n";
 if (model_class.length() > 0){
  ecpp_stream << "\nshared_ptr<" << ( model_class );
  ecpp_stream << "> " << ( classname );
  ecpp_stream << "::find_model(" << ( id_type );
  ecpp_stream << " id)\n{";
 if (database_backend.length() > 0){
  ecpp_stream << "\n  database.find_one(model, id);\n  vars[\"model\"] = model.get();";
 };
  ecpp_stream << "  return model;\n}\n\nvoid " << ( classname );
  ecpp_stream << "::require_model(" << ( id_type );
  ecpp_stream << " id)\n{\n  if (!model && !find_model(id))\n    respond_with(Crails::HttpStatus::not_found);\n}\n\nvoid " << ( classname );
  ecpp_stream << "::find_list()\n{";
 if (database_backend == "odb"){
  ecpp_stream << "\n  odb::result<" << ( model_class );
  ecpp_stream << "> results;\n\n  database.find(results);\n  model_list = Crails::Odb::to_vector<" << ( model_class );
  ecpp_stream << ">(results);";
 }else if (database_backend == "mongodb"){
  ecpp_stream << "\n  Crails::MongoDB::Result<" << ( model_class );
  ecpp_stream << "> results;\n\n  database.find(results);\n  model_list = results.to_vector();";
 };
  ecpp_stream << "  vars[\"models\"] = &model_list;\n}";
 };
  ecpp_stream << "\n";
 if (resource_scaffold || crud_scaffold){
  ecpp_stream << "\nvoid " << ( classname );
  ecpp_stream << "::index()\n{";
 if (model_class.length() > 0){
  ecpp_stream << "\n  find_list();";
 };
  ecpp_stream << "\n  render(\"" << ( view_path );
  ecpp_stream << "/index\");\n}\n\nvoid " << ( classname );
  ecpp_stream << "::show()\n{\n  render(\"" << ( view_path );
  ecpp_stream << "/show\");\n}\n";
 if (resource_scaffold){
  ecpp_stream << "\nvoid " << ( classname );
  ecpp_stream << "::new_()\n{\n  render(\"" << ( view_path );
  ecpp_stream << "/new\");\n}";
 };
  ecpp_stream << "\nvoid " << ( classname );
  ecpp_stream << "::create()\n{";
 if (model_class.length() > 0){
  ecpp_stream << "\n  " << ( model_class );
  ecpp_stream << " model;\n  model.edit(params[" << ( model_class );
  ecpp_stream << "::scope]);";
 if (database_backend.length() > 0){
  ecpp_stream << "\n  database.save(model);";
 };
  ecpp_stream << "  redirect_to(\"" << ( router_path );
  ecpp_stream << "/\" + boost::lexical_cast<std::string>(model.get_id()));";
 };
  ecpp_stream << "\n}\n";
 if (resource_scaffold){
  ecpp_stream << "\nvoid " << ( classname );
  ecpp_stream << "::edit()\n{\n  render(\"" << ( view_path );
  ecpp_stream << "/edit\");\n}";
 };
  ecpp_stream << "\nvoid " << ( classname );
  ecpp_stream << "::update()\n{";
 if (model_class.length() > 0){
  ecpp_stream << "\n  model->edit(params[" << ( model_class );
  ecpp_stream << "::scope]);";
 if (database_backend.length() > 0){
  ecpp_stream << "\n  database.save(*model);";
 };
  ecpp_stream << "\n  redirect_to(\"" << ( router_path );
  ecpp_stream << "/\" + boost::lexical_cast<std::string>(model->get_id()));";
 };
  ecpp_stream << "\n}\n\nvoid " << ( classname );
  ecpp_stream << "::destroy()\n{";
 if (model_class.length() > 0 && database_backend.length() > 0){
  ecpp_stream << "\n  database.destroy(*model);";
 };
  ecpp_stream << "\n  redirect_to(\"" << ( path );
  ecpp_stream << "\");\n}";
 };
  ecpp_stream << "";
    std::string _out_buffer = ecpp_stream.str();
    _out_buffer = this->apply_post_render_filters(_out_buffer);
    this->target.set_body(_out_buffer);
  }
private:
  std::stringstream ecpp_stream;
  string classname;
  string router_path;
  string path;
  string view_path;
  string parent_class;
  string model_class;
  string model_header;
  string id_type;
  string database_backend;
  bool resource_scaffold;
  bool crud_scaffold;
};

void render_scaffolds_controller_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_ScaffoldsControllerCpp(renderer, target, vars).render();
}