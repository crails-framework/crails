#include "plugin.hpp"
#include <crails/utils/split.hpp>
#include <string_view>
#include "../../file_renderer.hpp"
#include "../../version.hpp"

using namespace std;

static bool has_imagemagick(const ProjectConfiguration& configuration)
{
  return configuration.has_plugin("libcrails-image") || configuration.has_plugin("libcrails-captcha");
}

const vector<string_view> packages_blacklist = {
  "libcrails", "libcrails-odb", "libcrails-cms", "comet", "metarecord"
};

DockerPlugin::DockerPlugin()
{
  add_command("install", []() { return make_shared<DockerInstaller>(); });
  add_command("shell",   []() { return make_shared<DockerShell>(); });
  add_command("build",   []() { return make_shared<DockerBuild>(); });
  add_command("run",     []() { return make_shared<DockerRun>(); });
  add_command("package", []() { return make_shared<DockerPackage>(); });
  add_command("deploy",  []() { return make_shared<DockerDeploy>(); });
  add_command("new",     []() { return make_shared<DockerNew>(); });
  add_command("refresh", []() { return make_shared<DockerRefreshEnvironment>(); });
}

void DockerPlugin::refresh_environment(const ProjectConfiguration& configuration)
{
  FileRenderer renderer;
  list<string> packages = configuration.plugins();
  list<string> sql_backends = Crails::split(configuration.variable("odb-backends"), ',');

  for (string_view package : packages_blacklist)
    packages.erase(remove(packages.begin(), packages.end(), package), packages.end());
  renderer.should_overwrite = true;
  renderer.vars["with_odb"]         = configuration.has_plugin("libcrails-odb");
  renderer.vars["with_comet"]       = configuration.has_plugin("comet");
  renderer.vars["with_imagemagick"] = has_imagemagick(configuration);
  renderer.vars["packages"]         = &packages;
  renderer.vars["sql_backends"]     = &sql_backends;
  renderer.generate_file("docker/build-environment.sh", "docker/base/build-environment.sh");
}

int DockerPlugin::DockerInstaller::run()
{
  FileRenderer renderer;
  bool with_ssh         = configuration.has_plugin("libcrails-ssh");
  bool with_odb         = configuration.has_plugin("libcrails-odb");
  bool with_comet       = configuration.has_plugin("comet");
  bool with_imagemagick = has_imagemagick(configuration);
  bool with_metarecord  = configuration.has_plugin("metarecord");
  string build2_fingerprint = "70:64:FE:E4:E0:F3:60:F1:B4:51:E1:FA:12:5C:E0:B3:DB:DF:96:33:39:B9:2E:E5:C2:68:63:4C:A6:47:39:43";

  if (options.count("image"))
    renderer.vars["image"] = options["image"].as<string>();
  renderer.vars["crails_version"] = configuration.version();
  renderer.vars["with_odb"] = with_odb;
  renderer.vars["with_comet"] = with_comet;
  renderer.vars["with_imagemagick"] = with_imagemagick;
  renderer.vars["with_metarecord"] = with_metarecord;
  renderer.vars["with_ssh"] = with_ssh;
  renderer.vars["build2_fingerprint"] = build2_fingerprint;
  renderer.generate_file("docker/Dockerfile", "docker/base/Dockerfile");
  renderer.generate_file("docker/build-build2.sh", "docker/base/build-build2.sh");
  renderer.generate_file("docker/build-crails.sh", "docker/base/build-crails.sh");
  if (with_comet)
    renderer.generate_file("docker/build-comet.sh", "docker/base/build-comet.sh");
  if (with_odb)
    renderer.generate_file("docker/build-odb-compiler.sh", "docker/base/build-odb-compiler.sh");
  DockerPlugin::refresh_environment(configuration);
  return 0;
}

int DockerPlugin::DockerNew::run()
{
  FileRenderer renderer;
  string name;

  if (!options.count("name"))
  {
    cerr << "missing required --name parameter" << endl;
    return -1;
  }
  name = options["name"].as<string>();
  if (options.count("image"))
    renderer.vars["image"] = options["image"].as<string>();
  renderer.vars["script_path"] = string("base/");
  renderer.vars["with_odb"] = configuration.has_plugin("libcrails-odb");
  renderer.vars["with_comet"] = configuration.has_plugin("comet");
  renderer.vars["with_imagemagick"] = has_imagemagick(configuration);
  renderer.vars["with_metarecord"] = configuration.has_plugin("metarecord");
  renderer.generate_file("docker/Dockerfile", "docker/" + name + "/Dockerfile");
  return 0;
}

