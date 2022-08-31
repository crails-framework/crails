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
  -n ProjectConfigRenderersCpp \
  -i $PROJECT_TPL/config/renderers.cpp.ecpp \
  >  $PROJECT_TPL/build/renderers.cpp.cpp
$ECPP \
  -n ProjectConfigRequestPipeCpp \
  -i $PROJECT_TPL/config/request_pipe.cpp.ecpp \
  >  $PROJECT_TPL/build/request_pipe.cpp.cpp
$ECPP \
  -n ProjectConfigSaltCpp \
  -i $PROJECT_TPL/config/salt.cpp.ecpp \
  >  $PROJECT_TPL/build/salt.cpp.cpp
$ECPP \
  -n ProjectConfigSessionStoreCpp \
  -i $PROJECT_TPL/config/session_store.cpp.ecpp \
  >  $PROJECT_TPL/build/session_store.cpp
$ECPP \
  -n ProjectConfigSslCpp \
  -i $PROJECT_TPL/config/ssl.cpp.ecpp \
  >  $PROJECT_TPL/build/ssl.cpp
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

##
## Odb Module
##
$ECPP \
  -n OdbMigrateCpp \
  -i crails/plugins/odb/migrate.cpp.ecpp \
  >  $PROJECT_TPL/build/odb_migrate.cpp.cpp

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
