#include "project_renderer.hpp"
#define declare_renderer(name) \
  std::string render_##name(const Crails::Renderer*, Crails::SharedVars&);
#define add_renderer(path, name) \
  templates.insert(pair<string, Generator>(path, render_##name))

using namespace std;

declare_renderer(project_cmakelists_txt)
declare_renderer(project_app_main_cpp)
declare_renderer(project_app_routes_cpp)
declare_renderer(project_app_controllers_application_hpp)
declare_renderer(project_app_views_exception_html)
declare_renderer(project_config_environment_cpp)
declare_renderer(project_config_databases_cpp)
declare_renderer(project_config_logger_cpp)
declare_renderer(project_config_renderers_cpp)
declare_renderer(project_config_request_pipe_cpp)
declare_renderer(project_config_salt_cpp)
declare_renderer(project_config_session_store_cpp)
declare_renderer(project_config_ssl_cpp)
declare_renderer(project_public_index_html)
declare_renderer(project_spec_main_cpp)
declare_renderer(renderer_cpp)
declare_renderer(scaffolds_controller_cpp)
declare_renderer(scaffolds_controller_hpp)
declare_renderer(scaffolds_odb_model_cpp)
declare_renderer(scaffolds_odb_model_hpp)
declare_renderer(scaffolds_view_index_html)
declare_renderer(scaffolds_view_show_html)
declare_renderer(scaffolds_view_new_html)
declare_renderer(scaffolds_view_edit_html)
declare_renderer(scaffolds_view_form_html)
declare_renderer(scaffolds_view_index_json)
declare_renderer(scaffolds_view_show_json)
declare_renderer(scaffolds_task_cmakelists_txt)
declare_renderer(scaffolds_task_main_cpp)
declare_renderer(odb_hpp)
declare_renderer(odb_cpp)
declare_renderer(odb_migrate_cpp)

ProjectRenderer::ProjectRenderer()
{
  add_renderer("CMakeLists.txt",                  project_cmakelists_txt);
  add_renderer("app/main.cpp",                    project_app_main_cpp);
  add_renderer("app/routes.cpp",                  project_app_routes_cpp);
  add_renderer("app/controllers/application.hpp", project_app_controllers_application_hpp);
  add_renderer("app/views/exception.html",        project_app_views_exception_html);
  add_renderer("config/environment.cpp",          project_config_environment_cpp);
  add_renderer("config/databases.cpp",            project_config_databases_cpp);
  add_renderer("config/logger.cpp",               project_config_logger_cpp);
  add_renderer("config/renderers.cpp",            project_config_renderers_cpp);
  add_renderer("config/request_pipe.cpp",         project_config_request_pipe_cpp);
  add_renderer("config/salt.cpp",                 project_config_salt_cpp);
  add_renderer("config/session_store.cpp",        project_config_session_store_cpp);
  add_renderer("config/ssl.cpp",                  project_config_ssl_cpp);
  add_renderer("public/index.html",               project_public_index_html);
  add_renderer("spec/main.cpp",                   project_spec_main_cpp);

  add_renderer("renderer.cpp", renderer_cpp);

  add_renderer("scaffolds/controller.cpp", scaffolds_controller_cpp);
  add_renderer("scaffolds/controller.hpp", scaffolds_controller_hpp);
  add_renderer("scaffolds/odb_model.cpp", scaffolds_odb_model_cpp);
  add_renderer("scaffolds/odb_model.hpp", scaffolds_odb_model_hpp);
  add_renderer("scaffolds/view_index.html", scaffolds_view_index_html);
  add_renderer("scaffolds/view_show.html", scaffolds_view_show_html);
  add_renderer("scaffolds/view_new.html", scaffolds_view_new_html);
  add_renderer("scaffolds/view_edit.html", scaffolds_view_edit_html);
  add_renderer("scaffolds/view_form.html", scaffolds_view_form_html);
  add_renderer("scaffolds/view_index.json", scaffolds_view_index_json);
  add_renderer("scaffolds/view_show.json", scaffolds_view_show_json);
  add_renderer("scaffolds/task/CMakeLists.txt", scaffolds_task_cmakelists_txt);
  add_renderer("scaffolds/task/main.cpp", scaffolds_task_main_cpp);

  add_renderer("config/odb.hpp", odb_hpp);
  add_renderer("config/odb.cpp", odb_cpp);
  add_renderer("tasks/odb_migrate/main.cpp", odb_migrate_cpp);
}
