#!/bin/bash -ex
ECPP=ecpp
PROJECT_TPL=crails/new/templates
PROJECT_TPL_BUILD=$PROJECT_TPL/_build
RENDERERS_TPL=crails/templates
SCAFFOLDS_TPL=crails/scaffolds/templates
mkdir -p $PROJECT_TPL_BUILD

##
## AppTemplate
##
$ECPP \
  -n ProjectGitignore \
  -i $PROJECT_TPL/.gitignore.ecpp \
  >  $PROJECT_TPL_BUILD/gitignore.cpp
$ECPP \
  -n ProjectAppMainCpp \
  -i $PROJECT_TPL/exe/server/main.cpp.ecpp \
  >  $PROJECT_TPL_BUILD/main.cpp.cpp
$ECPP \
  -n ProjectAppRoutesCpp \
  -i $PROJECT_TPL/app/routes.cpp.ecpp \
  >  $PROJECT_TPL_BUILD/routes.cpp.cpp
$ECPP \
  -n ProjectConfigRouterHpp \
  -i $PROJECT_TPL/app/config/router.hpp.ecpp \
  >  $PROJECT_TPL_BUILD/router.hpp.cpp
$ECPP \
  -n ProjectAppViewsExceptionHtml \
  -i $PROJECT_TPL/app/views/exception.html.ecpp \
  >  $PROJECT_TPL_BUILD/exception.html.cpp
$ECPP \
  -n ProjectAppViewsExceptionJson \
  -i $PROJECT_TPL/app/views/exception.json.ecpp \
  >  $PROJECT_TPL_BUILD/exception.json.cpp
$ECPP \
  -n ProjectAppControllersApplicationHpp \
  -i $PROJECT_TPL/app/controllers/application.hpp.ecpp \
  >  $PROJECT_TPL_BUILD/application.hpp.cpp
$ECPP \
  -n ProjectConfigServerCpp \
  -i $PROJECT_TPL/app/config/server.cpp.ecpp \
  >  $PROJECT_TPL_BUILD/server.cpp.cpp
$ECPP \
  -n ProjectConfigServerHpp \
  -i $PROJECT_TPL/app/config/server.hpp.ecpp \
  >  $PROJECT_TPL_BUILD/server.hpp.cpp
$ECPP \
  -n ProjectConfigDatabasesHpp \
  -i $PROJECT_TPL/app/config/databases.hpp.ecpp \
  >  $PROJECT_TPL_BUILD/databases.hpp.cpp
$ECPP \
  -n ProjectConfigDatabasesCpp \
  -i $PROJECT_TPL/app/config/databases.cpp.ecpp \
  >  $PROJECT_TPL_BUILD/databases.cpp.cpp
$ECPP \
  -n ProjectConfigRenderersHpp \
  -i $PROJECT_TPL/app/config/renderers.hpp.ecpp \
  >  $PROJECT_TPL_BUILD/renderers.hpp.cpp
$ECPP \
  -n ProjectConfigRenderersCpp \
  -i $PROJECT_TPL/app/config/renderers.cpp.ecpp \
  >  $PROJECT_TPL_BUILD/renderers.cpp.cpp
$ECPP \
  -n ProjectConfigRequestPipeCpp \
  -i $PROJECT_TPL/app/config/request_pipe.cpp.ecpp \
  >  $PROJECT_TPL_BUILD/request_pipe.cpp.cpp
$ECPP \
  -n ProjectConfigSessionStoreHpp \
  -i $PROJECT_TPL/app/config/session_store.hpp.ecpp \
  >  $PROJECT_TPL_BUILD/session_store.hpp.cpp
$ECPP \
  -n ProjectConfigSessionStoreCpp \
  -i $PROJECT_TPL/app/config/session_store.cpp.ecpp \
  >  $PROJECT_TPL_BUILD/session_store.cpp.cpp
$ECPP \
  -n ProjectConfigSslCpp \
  -i $PROJECT_TPL/app/config/ssl.cpp.ecpp \
  >  $PROJECT_TPL_BUILD/ssl.cpp
$ECPP \
  -n ProjectConfigI18nHpp \
  -i $PROJECT_TPL/app/config/i18n.hpp.ecpp \
  >  $PROJECT_TPL_BUILD/i18n.hpp.cpp
$ECPP \
  -n ProjectConfigI18nCpp \
  -i $PROJECT_TPL/app/config/i18n.cpp.ecpp \
  >  $PROJECT_TPL_BUILD/i18n.cpp
$ECPP \
  -n ProjectPublicIndexHtml \
  -i $PROJECT_TPL/public/index.html.ecpp \
  >  $PROJECT_TPL_BUILD/index.html.cpp
$ECPP \
  -n ProjectSpecMainCpp \
  -i $PROJECT_TPL/spec/driver/main.cpp.ecpp \
  >  $PROJECT_TPL_BUILD/spec.cpp

## AppTemplate/CMake
PROJECT_CMAKE_TPL=crails/new/templates/cmake
PROJECT_CMAKE_BUILD=$PROJECT_TPL_BUILD/cmake
mkdir -p $PROJECT_CMAKE_BUILD
$ECPP \
  -n ProjectCmakelistsTxt \
  -i $PROJECT_CMAKE_TPL/CMakeLists.txt.ecpp \
  >  $PROJECT_CMAKE_BUILD/CMakeLists.txt.cpp
$ECPP \
  -n ProjectSpecCmakelistsTxt \
  -i $PROJECT_CMAKE_TPL/spec/CMakeLists.txt.ecpp \
  >  $PROJECT_CMAKE_BUILD/spec_CMakeLists.txt.cpp

## AppTemplate/Build2
PROJECT_BUILD2_TPL=crails/new/templates/build2
PROJECT_BUILD2_BUILD=$PROJECT_TPL_BUILD/build2
mkdir -p $PROJECT_BUILD2_BUILD
$ECPP \
  -n Build2Buildfile \
  -i $PROJECT_BUILD2_TPL/buildfile.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_buildfile.cpp
$ECPP \
  -n Build2Manifest \
  -i $PROJECT_BUILD2_TPL/manifest.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_manifest.cpp
$ECPP \
  -n Build2RepositoriesManifest \
  -i $PROJECT_BUILD2_TPL/repositories.manifest.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_repositories.cpp
$ECPP \
  -n Build2AppBuildfile \
  -i $PROJECT_BUILD2_TPL/app/buildfile.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_app_buildfile.cpp
$ECPP \
  -n Build2BuildBootstrapBuild \
  -i $PROJECT_BUILD2_TPL/build/bootstrap.build.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_bootstrap_build.cpp
$ECPP \
  -n Build2BuildExportBuild \
  -i $PROJECT_BUILD2_TPL/build/export.build.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_export_build.cpp
$ECPP \
  -n Build2BuildRootBuild \
  -i $PROJECT_BUILD2_TPL/build/root.build.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_root_build.cpp
$ECPP \
  -n Build2ExeBuildfile \
  -i $PROJECT_BUILD2_TPL/exe/buildfile.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_exe_buildfile.cpp
$ECPP \
  -n Build2ExeBootstrapBuild \
  -i $PROJECT_BUILD2_TPL/exe/build/bootstrap.build.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_exe_bootstrap_build.cpp
$ECPP \
  -n Build2ExeRootBuild \
  -i $PROJECT_BUILD2_TPL/exe/build/root.build.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_exe_root_build.cpp
$ECPP \
  -n Build2ServerBuildfile \
  -i $PROJECT_BUILD2_TPL/exe/server/buildfile.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_server_buildfile.cpp
$ECPP \
  -n Build2SpecBootstrapBuild \
  -i $PROJECT_BUILD2_TPL/spec/build/bootstrap.build.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_spec_bootstrap_build.cpp
$ECPP \
  -n Build2SpecRootBuild \
  -i $PROJECT_BUILD2_TPL/spec/build/root.build.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_spec_root_build.cpp
$ECPP \
  -n Build2SpecBuildfile \
  -i $PROJECT_BUILD2_TPL/spec/driver/buildfile.ecpp \
  > $PROJECT_BUILD2_BUILD/build2_spec_buildfile.cpp

##
## Renderers
##
mkdir -p $RENDERERS_TPL/build
$ECPP \
  -n RendererHpp \
  -i $RENDERERS_TPL/renderer.hpp.ecpp \
  >  $RENDERERS_TPL/build/renderer.hpp.cpp
$ECPP \
  -n RendererCpp \
  -i $RENDERERS_TPL/renderer.cpp.ecpp \
  >  $RENDERERS_TPL/build/renderer.cpp

##
## Scaffolds
##
mkdir -p $SCAFFOLDS_TPL/build
$ECPP \
  -n ScaffoldsControllerCpp \
  -i $SCAFFOLDS_TPL/controller.cpp.ecpp \
  >  $SCAFFOLDS_TPL/build/controller.cpp.cpp
$ECPP \
  -n ScaffoldsControllerHpp \
  -i $SCAFFOLDS_TPL/controller.hpp.ecpp \
  >  $SCAFFOLDS_TPL/build/controller.hpp.cpp
$ECPP \
  -n ScaffoldsModelPropertiesHpp \
  -i $SCAFFOLDS_TPL/_model_properties.hpp.ecpp \
  >  $SCAFFOLDS_TPL/build/_model_properties.hpp.cpp
$ECPP \
  -n ScaffoldsMetarecordModelRb \
  -i $SCAFFOLDS_TPL/metarecord_model.rb.ecpp \
  >  $SCAFFOLDS_TPL/build/metarecord_model.rb.cpp
$ECPP \
  -n ScaffoldsOdbModelCpp \
  -i $SCAFFOLDS_TPL/odb_model.cpp.ecpp \
  >  $SCAFFOLDS_TPL/build/odb_model.cpp.cpp
$ECPP \
  -n ScaffoldsOdbModelHpp \
  -i $SCAFFOLDS_TPL/odb_model.hpp.ecpp \
  >  $SCAFFOLDS_TPL/build/odb_model.hpp.cpp
$ECPP \
  -n ScaffoldsMongodbModelCpp \
  -i $SCAFFOLDS_TPL/mongodb_model.cpp.ecpp \
  >  $SCAFFOLDS_TPL/build/mongodb_model.cpp.cpp
$ECPP \
  -n ScaffoldsMongodbModelHpp \
  -i $SCAFFOLDS_TPL/mongodb_model.hpp.ecpp \
  >  $SCAFFOLDS_TPL/build/mongodb_model.hpp.cpp
$ECPP \
  -n ScaffoldsViewIndexHtml \
  -i $SCAFFOLDS_TPL/view_index.html.ecpp \
  >  $SCAFFOLDS_TPL/build/view_index.html.cpp
$ECPP \
  -n ScaffoldsViewShowHtml \
  -i $SCAFFOLDS_TPL/view_show.html.ecpp \
  >  $SCAFFOLDS_TPL/build/view_show.html.cpp
$ECPP \
  -n ScaffoldsViewNewHtml \
  -i $SCAFFOLDS_TPL/view_new.html.ecpp \
  >  $SCAFFOLDS_TPL/build/view_new.html.cpp
$ECPP \
  -n ScaffoldsViewEditHtml \
  -i $SCAFFOLDS_TPL/view_edit.html.ecpp \
  >  $SCAFFOLDS_TPL/build/view_edit.html.cpp
$ECPP \
  -n ScaffoldsViewFormHtml \
  -i $SCAFFOLDS_TPL/view_form.html.ecpp \
  >  $SCAFFOLDS_TPL/build/view_form.html.cpp
$ECPP \
  -n ScaffoldsViewIndexJson \
  -i $SCAFFOLDS_TPL/view_index.json.ecpp \
  >  $SCAFFOLDS_TPL/build/view_index.json.cpp
$ECPP \
  -n ScaffoldsViewShowJson \
  -i $SCAFFOLDS_TPL/view_show.json.ecpp \
  >  $SCAFFOLDS_TPL/build/view_show.json.cpp
$ECPP \
  -n ScaffoldsTaskCmakelistsTxt \
  -i $SCAFFOLDS_TPL/task_CMakeLists.txt.ecpp \
  >  $SCAFFOLDS_TPL/build/task_CMakeLists.txt.cpp
$ECPP \
  -n ScaffoldsTaskBuildfile \
  -i $SCAFFOLDS_TPL/task_buildfile.ecpp \
  >  $SCAFFOLDS_TPL/build/task_buildfile.txt.cpp
$ECPP \
  -n ScaffoldsTaskMainCpp \
  -i $SCAFFOLDS_TPL/task_main.cpp.ecpp \
  >  $SCAFFOLDS_TPL/build/task_main.cpp.cpp
$ECPP \
  -n ScaffoldsSpecTestHpp \
  -i $SCAFFOLDS_TPL/test.hpp.ecpp \
  >  $SCAFFOLDS_TPL/build/test.hpp.cpp
$ECPP \
  -n ScaffoldsSpecTestCpp \
  -i $SCAFFOLDS_TPL/test.cpp.ecpp \
  >  $SCAFFOLDS_TPL/build/test.cpp.cpp

##
## Odb Module
##
$ECPP \
  -n OdbMigrateCpp \
  -i crails/plugins/odb/migrate.cpp.ecpp \
  >  $PROJECT_TPL_BUILD/odb_migrate.cpp.cpp

##
## Sentry plugin
##
$ECPP \
  -n SentryConfigSentryCpp \
  -i crails/plugins/sentry/templates/sentry.cpp.ecpp \
  >  crails/plugins/sentry/templates/build/sentry.cpp.cpp

##
## Docker plugin
##
DOCKER_TPL=crails/plugins/docker/templates
$ECPP \
  -n DockerDockerfile \
  -i $DOCKER_TPL/Dockerfile.ecpp \
  >  $DOCKER_TPL/build/Dockerfile.cpp
$ECPP \
  -n DockerBuildBuild2 \
  -i $DOCKER_TPL/build-build2.sh.ecpp \
  >  $DOCKER_TPL/build/build-build2.sh.cpp
$ECPP \
  -n DockerBuildOdbCompiler \
  -i $DOCKER_TPL/build-odb-compiler.sh.ecpp \
  >  $DOCKER_TPL/build/build-odb-compiler.sh.cpp
$ECPP \
  -n DockerBuildCrails \
  -i $DOCKER_TPL/build-crails.sh.ecpp \
  >  $DOCKER_TPL/build/build-crails.cpp
$ECPP \
  -n DockerBuildComet \
  -i $DOCKER_TPL/build-comet.sh.ecpp \
  >  $DOCKER_TPL/build/build-comet.cpp
$ECPP \
  -n DockerBuildEnvironment \
  -i $DOCKER_TPL/build-environment.sh.ecpp \
  >  $DOCKER_TPL/build/build-environment.cpp

##
## Sidekix plugin
##
SIDEKIX_TPL=crails/plugins/sidekix/templates
$ECPP \
  -n SidekixConfigSidekixCpp \
  -i $SIDEKIX_TPL/config.cpp.ecpp \
  >  $SIDEKIX_TPL/build/config.cpp
$ECPP \
  -n SidekixMainCpp \
  -i $SIDEKIX_TPL/main.cpp.ecpp \
  >  $SIDEKIX_TPL/build/main.cpp
$ECPP \
  -n SidekixCtplH \
  -i $SIDEKIX_TPL/ctpl.h.ecpp \
  >  $SIDEKIX_TPL/build/ctpl.cpp
$ECPP \
  -n SidekixSidetasksCpp \
  -i $SIDEKIX_TPL/sidetasks.cpp.ecpp \
  >  $SIDEKIX_TPL/build/sidetasks.cpp
$ECPP \
  -n SidekixSidetasksHpp \
  -i $SIDEKIX_TPL/sidetasks.hpp.ecpp \
  >  $SIDEKIX_TPL/build/sidetasks.hpp.cpp
$ECPP \
  -n SidekixSidetaskCpp \
  -i $SIDEKIX_TPL/sidetask.cpp.ecpp \
  >  $SIDEKIX_TPL/build/sidetask.cpp

##
## Package
##
PACKAGE_TPL=crails/package/templates
$ECPP \
  -n PackageStartSh \
  -i $PACKAGE_TPL/start.sh \
  >  $PACKAGE_TPL/build/start.sh.cpp
$ECPP \
  -n PackageStopSh \
  -i $PACKAGE_TPL/stop.sh \
  >  $PACKAGE_TPL/build/stop.sh.cpp
