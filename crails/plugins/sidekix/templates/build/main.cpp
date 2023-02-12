#include <sstream>
#include "crails/render_target.hpp"
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class render_SidekixMainCpp : public Crails::Template
{
public:
  render_SidekixMainCpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars) :
    Crails::Template(renderer, target, vars)
  {}

  void render()
  {
ecpp_stream << "#include <csignal>\n#include <functional>\n#include <chrono>\n#include <atomic>\n#include <crails/params.hpp>\n#include <crails/utils/timer.hpp>\n#include <crails/logger.hpp>\n#include <crails/sidekix.hpp>\n#include <crails/log_files.hpp>\n#include <crails/server.hpp>\n#include <crails/session_store/no_session_store.hpp>\n#include <boost/program_options.hpp>\n#include <fstream>\n#include <thread>\n#ifdef _WIN32\n# include <Windows.h>\n#else\n# include <unistd.h>\n#endif\n#include \"ctpl.h\"\n#include \"sidetasks.hpp\"\n\n#ifndef THREAD_POOL_SIZE\n# define THREAD_POOL_SIZE 2\n#endif\n\nusing namespace std;\nusing namespace Crails;\n\nstatic unsigned int update_interval = 1000000;\nstatic atomic_bool should_continue(true);\n\nstatic void shutdown(int)\n{\n  Server::get_io_context().stop();\n  should_continue = false;\n}\n\nstatic void run_task(const std::string& type, std::function<void(Params&)> runner, std::shared_ptr<Params> params, int id)\n{\n  try\n  {\n    Utils::Timer timer;\n\n    logger << Logger::Info\n      << \"# Running task of type \" << type << \" in \" << id << Logger::endl;\n    runner(*params);\n    logger << Logger::Info\n      << \"# Done (duration: \" << timer.GetElapsedSeconds() << \"s)\"\n      << Logger::endl;\n  }\n  catch (const std::exception& e)\n  {\n    logger << Logger::Error <<\n      \"[!] Catched exception: \" << e.what() << Logger::endl;\n  }\n}\n\nstatic void run_tasks(ctpl::thread_pool& pool, vector<string> tasks)\n{\n  auto data = std::make_shared<Crails::Params>();\n  auto* database = Sidekix::SideTaskDatabase::instance();\n\n  while (database->pop(*data))\n  {\n    string task_type = (*data)[Sidekix::task_type_param];\n    auto runner = sidetasks.find(task_type);\n\n    if (runner == sidetasks.end())\n      logger << Logger::Error << \"/!\\\\ Scheduled task of unknown type `\" << task_type << '`' << Logger::endl;\n    else\n      pool.push(std::bind(&run_task, task_type, runner->second, data, std::placeholders::_1));\n    data = std::make_shared<Crails::Params>();\n  }\n}\n\nstatic void tasks_main()\n{\n  ctpl::thread_pool taskpool(THREAD_POOL_SIZE);\n  Sidekix::SideTaskDatabase* database;\n\n  database = Sidekix::SideTaskDatabase::instance();\n  do {\n    run_tasks(taskpool, database->pending_tasks());\n    usleep(update_interval);\n  } while (should_continue == true);\n}\n\nstatic void initialize_pid_file(const std::string& filepath)\n{\n  std::ofstream output(filepath.c_str());\n\n  if (output.is_open())\n  {\n    pid_t pid = getpid();\n\n    logger << Logger::Info << \">> PID \" << pid << \" (stored in \" << filepath << ')' << Logger::endl;\n    output << pid;\n    output.close();\n  }\n  else\n    logger << Logger::Error << \"!! Failed to open PID file \" << filepath << Logger::endl;\n}\n\nint main(int argc, const char** argv)\n{\n  SingletonInstantiator<LogFiles> log_files(argc, argv);\n  SingletonInstantiator<NoSessionStore::Factory> empty_session_store;\n\n  {\n    using namespace boost;\n    program_options::variables_map vm;\n    program_options::options_description desc(\"Options\");\n\n    desc.add_options()\n      (\"log,l\", program_options::value<std::string>(), \"log output file\")\n      (\"error,e\", program_options::value<std::string>(), \"error log output file\")\n      (\"pidfile\", program_options::value<std::string>(), \"pid file\")\n      (\"interval\", program_options::value<unsigned short>(), \"task pulling interval in microseconds\");\n    program_options::store(program_options::parse_command_line(argc, argv, desc), vm);\n    program_options::notify(vm);\n    if (vm.count(\"pidfile\"))\n      initialize_pid_file(vm[\"pidfile\"].as<std::string>());\n    if (vm.count(\"interval\"))\n      update_interval = vm[\"interval\"].as<unsigned int>();\n  }\n\n  signal(SIGINT, &shutdown);\n  signal(SIGTERM, &shutdown);\n  std::thread task_thread(&tasks_main);\n  Server::get_io_context().run();\n  task_thread.join();\n  return 0;\n}\n";
    this->target.set_body(ecpp_stream.str());
  }
private:
  std::stringstream ecpp_stream;
};

void render_sidekix_main_cpp(const Crails::Renderer& renderer, Crails::RenderTarget& target, Crails::SharedVars& vars)
{
  render_SidekixMainCpp(renderer, target, vars).render();
}