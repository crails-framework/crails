#!/bin/bash -ex
ECPP=ecpp
PROJECT_TPL=crails/new/templates
RENDERERS_TPL=crails/templates
SCAFFOLDS_TPL=crails/scaffolds/templates
##
## AppTemplate
##
mkdir -p $PROJECT_TPL/build
$ECPP \
  -n ProjectGitignore \
  -i $PROJECT_TPL/.gitignore.ecpp \
  >  $PROJECT_TPL/build/gitignore.cpp
$ECPP \
  -n ProjectCmakelistsTxt \
  -i $PROJECT_TPL/CMakeLists.txt.ecpp \
  >  $PROJECT_TPL/build/CMakeLists.txt.cpp
$ECPP \
  -n ProjectAppMainCpp \
  -i $PROJECT_TPL/app/main.cpp.ecpp \
  >  $PROJECT_TPL/build/main.cpp.cpp
$ECPP \
  -n ProjectAppRoutesCpp \
  -i $PROJECT_TPL/app/routes.cpp.ecpp \
  >  $PROJECT_TPL/build/routes.cpp.cpp
$ECPP \
  -n ProjectAppViewsExceptionHtml \
  -i $PROJECT_TPL/app/views/exception.html.ecpp \
  >  $PROJECT_TPL/build/exception.html.cpp
$ECPP \
  -n ProjectAppControllersApplicationHpp \
  -i $PROJECT_TPL/app/controllers/application.hpp.ecpp \
  >  $PROJECT_TPL/build/application.hpp.cpp
$ECPP \
  -n ProjectConfigServerCpp \
  -i $PROJECT_TPL/config/server.cpp.ecpp \
  >  $PROJECT_TPL/build/server.cpp.cpp
$ECPP \
  -n ProjectConfigServerHpp \
  -i $PROJECT_TPL/config/server.hpp.ecpp \
  >  $PROJECT_TPL/build/server.hpp.cpp
$ECPP \
  -n ProjectConfigEnvironmentCpp \
  -i $PROJECT_TPL/config/environment.cpp.ecpp \
  >  $PROJECT_TPL/build/environment.cpp.cpp
$ECPP \
  -n ProjectConfigDatabasesCpp \
  -i $PROJECT_TPL/config/databases.cpp.ecpp \
  >  $PROJECT_TPL/build/databases.cpp.cpp
$ECPP \
  -n ProjectConfigLoggerCpp \
  -i $PROJECT_TPL/config/logger.cpp.ecpp \
  >  $PROJECT_TPL/build/logger.cpp.cpp
$ECPP \
  -n ProjectConfigRenderersHpp \
  -i $PROJECT_TPL/config/renderers.hpp.ecpp \
  >  $PROJECT_TPL/build/renderers.hpp.cpp
$ECPP \
  -n ProjectConfigRenderersCpp \
  -i $PROJECT_TPL/config/renderers.cpp.ecpp \
  >  $PROJECT_TPL/build/renderers.cpp.cpp
$ECPP \
  -n ProjectConfigRequestPipeCpp \
  -i $PROJECT_TPL/config/request_pipe.cpp.ecpp \
  >  $PROJECT_TPL/build/request_pipe.cpp.cpp
$ECPP \
  -n ProjectConfigSessionStoreHpp \
  -i $PROJECT_TPL/config/session_store.hpp.ecpp \
  >  $PROJECT_TPL/build/session_store.hpp.cpp
$ECPP \
  -n ProjectConfigCookiesCpp \
  -i $PROJECT_TPL/config/cookies.cpp.ecpp \
  >  $PROJECT_TPL/build/cookies.cpp.cpp
$ECPP \
  -n ProjectConfigSslCpp \
  -i $PROJECT_TPL/config/ssl.cpp.ecpp \
  >  $PROJECT_TPL/build/ssl.cpp
$ECPP \
  -n ProjectConfigI18nHpp \
  -i $PROJECT_TPL/config/i18n.hpp.ecpp \
  >  $PROJECT_TPL/build/i18n.hpp.cpp
$ECPP \
  -n ProjectConfigI18nCpp \
  -i $PROJECT_TPL/config/i18n.cpp.ecpp \
  >  $PROJECT_TPL/build/i18n.cpp
$ECPP \
  -n ProjectPublicIndexHtml \
  -i $PROJECT_TPL/public/index.html.ecpp \
  >  $PROJECT_TPL/build/index.html.cpp
$ECPP \
  -n ProjectSpecMainCpp \
  -i $PROJECT_TPL/spec/main.cpp.ecpp \
  >  $PROJECT_TPL/build/spec.cpp

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
  >  $PROJECT_TPL/build/odb_migrate.cpp.cpp

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
