#include "plugins.hpp"
#include "../plugins/odb/module.hpp"
#include "../plugins/comet/plugin.hpp"
#include "../plugins/docker/plugin.hpp"
#include "../plugins/metarecord/plugin.hpp"
#include "../plugins/mail/plugin.hpp"

using namespace std;

PluginManager::PluginManager()
{
  add_command("odb", []() { return make_shared<OdbModule>(); });
  add_command("comet", []() { return make_shared<CometPlugin>(); });
  add_command("docker", []() { return make_shared<DockerPlugin>(); });
  add_command("mail", []() { return make_shared<MailPlugin>(); });
  add_command("metarecord", []() { return make_shared<MetarecordPlugin>(); });
}
