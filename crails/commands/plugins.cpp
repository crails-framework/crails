#include "plugins.hpp"
#include "../plugins/odb/module.hpp"
#include "../plugins/mongodb/module.hpp"
#include "../plugins/comet/plugin.hpp"
#include "../plugins/docker/plugin.hpp"
#include "../plugins/metarecord/plugin.hpp"
#include "../plugins/mail/plugin.hpp"
#include "../plugins/signin/plugin.hpp"
#include "../plugins/sync/plugin.hpp"
#include "../plugins/sidekix/plugin.hpp"
#include "../plugins/redis/plugin.hpp"
#include "../plugins/sentry/plugin.hpp"
#include "../plugins/i18n/plugin.hpp"
#include "../plugins/others.hpp"

using namespace std;

PluginManager::PluginManager()
{
  add_command("extra", []() { return make_shared<OtherPlugins>(); });
  add_command("comet", []() { return make_shared<CometPlugin>(); });
  add_command("docker", []() { return make_shared<DockerPlugin>(); });
  add_command("i18n", []() { return make_shared<I18nPlugin>(); });
  add_command("mail", []() { return make_shared<MailPlugin>(); });
  add_command("metarecord", []() { return make_shared<MetarecordPlugin>(); });
  add_command("mongodb", []() { return make_shared<MongodbModule>(); });
  add_command("odb", []() { return make_shared<OdbModule>(); });
  add_command("redis", []() { return make_shared<RedisPlugin>(); });
  add_command("sentry", []() { return make_shared<SentryPlugin>(); });
  add_command("sidekix", []() { return make_shared<SidekixPlugin>(); });
  add_command("signin", []() { return make_shared<SigninPlugin>(); });
  add_command("sync", []() { return make_shared<SyncPlugin>(); });
}
