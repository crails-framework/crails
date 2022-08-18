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
