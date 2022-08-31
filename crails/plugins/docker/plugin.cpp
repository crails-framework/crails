#include "plugin.hpp"
#include <crails/utils/split.hpp>
#include "../../file_renderer.hpp"
#include "../../version.hpp"

using namespace std;

DockerPlugin::DockerPlugin()
{
  add_command("install", []() { return make_shared<DockerInstaller>(); });
  add_command("shell",   []() { return make_shared<DockerShell>(); });
  add_command("package", []() { return make_shared<DockerPackage>(); });
}

void DockerPlugin::refresh_environment(const ProjectConfiguration& configuration)
{
  FileRenderer renderer;
  list<string> packages = configuration.plugins();
  list<string> sql_backends = Crails::split(configuration.variable("odb-backends"), ',');

  std::remove(packages.begin(), packages.end(), "libcrails");
  std::remove(packages.begin(), packages.end(), "libcrails-odb");
  std::remove(packages.begin(), packages.end(), "comet");
  renderer.should_overwrite = true;
  renderer.vars["with_odb"]     = configuration.has_plugin("libcrails-odb");
  renderer.vars["with_comet"]   = configuration.has_plugin("comet");
  renderer.vars["packages"]     = &packages;
  renderer.vars["sql_backends"] = &sql_backends;
  renderer.generate_file("docker/build-environment.sh", "docker/base/build-environment.sh");
}

int DockerPlugin::DockerInstaller::run()
{
  FileRenderer renderer;
  bool with_odb   = configuration.has_plugin("libcrails-odb");
  bool with_comet = configuration.has_plugin("comet");
  string build2_fingerprint = "70:64:FE:E4:E0:F3:60:F1:B4:51:E1:FA:12:5C:E0:B3:DB:DF:96:33:39:B9:2E:E5:C2:68:63:4C:A6:47:39:43";

  if (options.count("image"))
    renderer.vars["image"] = options["image"].as<string>();
  renderer.vars["crails_version"] = configuration.version();
  renderer.vars["with_odb"] = with_odb;
  renderer.vars["with_comet"] = with_comet;
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

int DockerPlugin::DockerPackage::run()
{
  // TODO
  return 0;
}
