#include "project_renderer.hpp"
#define declare_renderer(name) \
  void render_##name(const Crails::Renderer&, Crails::RenderTarget&, Crails::SharedVars&);
#define add_renderer(path, name) \
  templates.insert(pair<string, Generator>(path, render_##name))

using namespace std;

declare_renderer(project_gitignore)
declare_renderer(project_cmakelists_txt)
declare_renderer(project_app_main_cpp)
declare_renderer(project_app_routes_cpp)
declare_renderer(project_app_controllers_application_hpp)
declare_renderer(project_app_views_exception_html)
declare_renderer(project_app_views_exception_json)
declare_renderer(project_config_router_hpp)
declare_renderer(project_config_server_cpp)
declare_renderer(project_config_server_hpp)
declare_renderer(project_config_databases_cpp)
declare_renderer(project_config_databases_hpp)
declare_renderer(project_config_renderers_hpp)
declare_renderer(project_config_renderers_cpp)
declare_renderer(project_config_request_pipe_cpp)
declare_renderer(project_config_session_store_hpp)
declare_renderer(project_config_session_store_cpp)
declare_renderer(project_config_ssl_cpp)
declare_renderer(project_config_i18n_hpp)
declare_renderer(project_config_i18n_cpp)
declare_renderer(project_public_index_html)
declare_renderer(project_spec_cmakelists_txt)
declare_renderer(project_spec_main_cpp)
declare_renderer(renderer_cpp)
declare_renderer(renderer_hpp)
declare_renderer(scaffolds_controller_cpp)
declare_renderer(scaffolds_controller_hpp)
declare_renderer(scaffolds_model_properties_hpp)
declare_renderer(scaffolds_metarecord_model_rb)
declare_renderer(scaffolds_odb_model_cpp)
declare_renderer(scaffolds_odb_model_hpp)
declare_renderer(scaffolds_mongodb_model_cpp)
declare_renderer(scaffolds_mongodb_model_hpp)
declare_renderer(scaffolds_view_index_html)
declare_renderer(scaffolds_view_show_html)
declare_renderer(scaffolds_view_new_html)
declare_renderer(scaffolds_view_edit_html)
declare_renderer(scaffolds_view_form_html)
declare_renderer(scaffolds_view_index_json)
declare_renderer(scaffolds_view_show_json)
declare_renderer(scaffolds_task_cmakelists_txt)
declare_renderer(scaffolds_task_buildfile)
declare_renderer(scaffolds_task_main_cpp)
declare_renderer(scaffolds_layout_bare_html)
declare_renderer(scaffolds_layout_bootstrap_html)
declare_renderer(scaffolds_layout_materialize_html)
declare_renderer(scaffolds_module_cmakelists_txt)
declare_renderer(scaffolds_module_module_hpp)
declare_renderer(scaffolds_module_routes_cpp)
declare_renderer(scaffolds_spec_test_hpp)
declare_renderer(scaffolds_spec_test_cpp)
declare_renderer(odb_hpp)
declare_renderer(odb_migrate_cpp)
declare_renderer(docker_dockerfile)
declare_renderer(docker_build_build2)
declare_renderer(docker_build_odb_compiler)
declare_renderer(docker_build_crails)
declare_renderer(docker_build_comet)
declare_renderer(docker_build_environment)
declare_renderer(mail_config_mailers_hpp)
declare_renderer(mail_config_mailers_cpp)
declare_renderer(sync_config_sync_cpp)
declare_renderer(signin_config_signin_cpp)
declare_renderer(sidekix_config_sidekix_cpp)
declare_renderer(sidekix_main_cpp)
declare_renderer(sidekix_ctpl_h)
declare_renderer(sidekix_sidetasks_cpp)
declare_renderer(sidekix_sidetasks_hpp)
declare_renderer(sidekix_sidetask_cpp)
declare_renderer(sentry_config_sentry_cpp)
declare_renderer(package_start_sh)
declare_renderer(package_stop_sh)

declare_renderer(build2_buildfile)
declare_renderer(build2_manifest)
declare_renderer(build2_repositories_manifest)
declare_renderer(build2_app_buildfile)
declare_renderer(build2_build_bootstrap_build)
declare_renderer(build2_build_export_build)
declare_renderer(build2_build_root_build)
declare_renderer(build2_exe_buildfile)
declare_renderer(build2_exe_bootstrap_build)
declare_renderer(build2_exe_root_build)
declare_renderer(build2_server_buildfile)
declare_renderer(build2_spec_bootstrap_build)
declare_renderer(build2_spec_root_build)
declare_renderer(build2_spec_buildfile)

ProjectRenderer::ProjectRenderer()
{
  add_renderer(".gitignore",                      project_gitignore);
  add_renderer("exe/server/main.cpp",             project_app_main_cpp);
  add_renderer("app/routes.cpp",                  project_app_routes_cpp);
  add_renderer("app/controllers/application.hpp", project_app_controllers_application_hpp);
  add_renderer("app/views/exception.html",        project_app_views_exception_html);
  add_renderer("app/views/exception.json",        project_app_views_exception_json);
  add_renderer("app/config/server.cpp",           project_config_server_cpp);
  add_renderer("app/config/router.hpp",           project_config_router_hpp);
  add_renderer("app/config/server.hpp",           project_config_server_hpp);
  add_renderer("app/config/databases.hpp",        project_config_databases_hpp);
  add_renderer("app/config/databases.cpp",        project_config_databases_cpp);
  add_renderer("app/config/renderers.hpp",        project_config_renderers_hpp);
  add_renderer("app/config/renderers.cpp",        project_config_renderers_cpp);
  add_renderer("app/config/request_pipe.cpp",     project_config_request_pipe_cpp);
  add_renderer("app/config/session_store.hpp",    project_config_session_store_hpp);
  add_renderer("app/config/session_store.cpp",    project_config_session_store_cpp);
  add_renderer("app/config/ssl.cpp",              project_config_ssl_cpp);
  add_renderer("app/config/i18n.hpp",             project_config_i18n_hpp);
  add_renderer("app/config/i18n.cpp",             project_config_i18n_cpp);
  add_renderer("public/index.html",               project_public_index_html);
  add_renderer("spec/driver/main.cpp",            project_spec_main_cpp);

  add_renderer("renderer.hpp", renderer_hpp);
  add_renderer("renderer.cpp", renderer_cpp);

  add_renderer("scaffolds/controller.cpp", scaffolds_controller_cpp);
  add_renderer("scaffolds/controller.hpp", scaffolds_controller_hpp);
  add_renderer("scaffolds/_model_properties.hpp", scaffolds_model_properties_hpp);
  add_renderer("scaffolds/metarecord_model.rb", scaffolds_metarecord_model_rb);
  add_renderer("scaffolds/odb_model.cpp", scaffolds_odb_model_cpp);
  add_renderer("scaffolds/odb_model.hpp", scaffolds_odb_model_hpp);
  add_renderer("scaffolds/mongodb_model.cpp", scaffolds_mongodb_model_cpp);
  add_renderer("scaffolds/mongodb_model.hpp", scaffolds_mongodb_model_hpp);
  add_renderer("scaffolds/view_index.html", scaffolds_view_index_html);
  add_renderer("scaffolds/view_show.html", scaffolds_view_show_html);
  add_renderer("scaffolds/view_new.html", scaffolds_view_new_html);
  add_renderer("scaffolds/view_edit.html", scaffolds_view_edit_html);
  add_renderer("scaffolds/view_form.html", scaffolds_view_form_html);
  add_renderer("scaffolds/view_index.json", scaffolds_view_index_json);
  add_renderer("scaffolds/view_show.json", scaffolds_view_show_json);
  add_renderer("scaffolds/task/CMakeLists.txt", scaffolds_task_cmakelists_txt);
  add_renderer("scaffolds/task/buildfile", scaffolds_task_buildfile);
  add_renderer("scaffolds/task/main.cpp", scaffolds_task_main_cpp);
  add_renderer("scaffolds/layouts/bare.html", scaffolds_layout_bare_html);
  add_renderer("scaffolds/layouts/bootstrap.html", scaffolds_layout_bootstrap_html);
  add_renderer("scaffolds/layouts/materialize.html", scaffolds_layout_materialize_html);
  add_renderer("scaffolds/modules/CMakeLists.txt", scaffolds_module_cmakelists_txt);
  add_renderer("scaffolds/modules/module.hpp", scaffolds_module_module_hpp);
  add_renderer("scaffolds/modules/routes.cpp", scaffolds_module_routes_cpp);
  add_renderer("scaffolds/spec/test.hpp", scaffolds_spec_test_hpp);
  add_renderer("scaffolds/spec/test.cpp", scaffolds_spec_test_cpp);

  add_renderer("package/start.sh",        package_start_sh);
  add_renderer("package/stop.sh",         package_stop_sh);

  add_renderer("app/config/odb.hpp", odb_hpp);
  add_renderer("exe/odb_migrate/main.cpp", odb_migrate_cpp);

  add_renderer("app/config/mailers.cpp", mail_config_mailers_cpp);
  add_renderer("app/config/mailers.hpp", mail_config_mailers_hpp);
  add_renderer("app/config/sync.cpp", sync_config_sync_cpp);
  add_renderer("app/config/signin.cpp", signin_config_signin_cpp);
  add_renderer("app/config/sentry.cpp", sentry_config_sentry_cpp);

  add_renderer("docker/Dockerfile",            docker_dockerfile);
  add_renderer("docker/build-build2.sh",       docker_build_build2);
  add_renderer("docker/build-odb-compiler.sh", docker_build_odb_compiler);
  add_renderer("docker/build-crails.sh",       docker_build_crails);
  add_renderer("docker/build-comet.sh",        docker_build_comet);
  add_renderer("docker/build-environment.sh",  docker_build_environment);

  add_renderer("app/config/sidekix.cpp",    sidekix_config_sidekix_cpp);
  add_renderer("exe/sidekix/main.cpp",      sidekix_main_cpp);
  add_renderer("exe/sidekix/ctpl.h",        sidekix_ctpl_h);
  add_renderer("exe/sidekix/sidetasks.hpp", sidekix_sidetasks_hpp);
  add_renderer("exe/sidekix/sidetasks.cpp", sidekix_sidetasks_cpp);
  add_renderer("exe/sidekix/sidetask.cpp",  sidekix_sidetask_cpp);

  add_renderer("cmake/CMakeLists.txt",      project_cmakelists_txt);
  add_renderer("cmake/spec/CMakeLists.txt", project_spec_cmakelists_txt);

  add_renderer("build2/buildfile", build2_buildfile);
  add_renderer("build2/manifest", build2_manifest);
  add_renderer("build2/repositories.manifest", build2_repositories_manifest);
  add_renderer("build2/app/buildfile", build2_app_buildfile);
  add_renderer("build2/build/bootstrap.build", build2_build_bootstrap_build);
  add_renderer("build2/build/export.build", build2_build_export_build);
  add_renderer("build2/build/root.build", build2_build_root_build);
  add_renderer("build2/exe/buildfile", build2_exe_buildfile);
  add_renderer("build2/exe/bootstrap.build", build2_exe_bootstrap_build);
  add_renderer("build2/exe/root.build", build2_exe_root_build);
  add_renderer("build2/server/buildfile", build2_server_buildfile);
  add_renderer("build2/spec/buildfile", build2_spec_buildfile);
  add_renderer("build2/spec/bootstrap.build", build2_spec_bootstrap_build);
  add_renderer("build2/spec/root.build", build2_spec_root_build);
}

const vector<string>& ProjectRenderer::get_mimetypes() const
{
  static const vector<string> meep{};

  return meep;
}
