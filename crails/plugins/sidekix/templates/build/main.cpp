#include <sstream>
#include "crails/shared_vars.hpp"
#include "crails/template.hpp"

class SidekixMainCpp : public Crails::Template
{
public:
  SidekixMainCpp(const Crails::Renderer* renderer, Crails::SharedVars& vars) :
    Crails::Template(renderer, vars)
  {}

  std::string render()
  {
ecpp_stream << "#include <csignal>\n#include <functional>\n#include <chrono>\n#include <atomic>\n#include <crails/params.hpp>\n#include <crails/utils/timer.hpp>\n#include <crails/logger.hpp>\n#include <crails/sidekix.hpp>\n#include <crails/log_files.hpp>\n#include <fstream>\n#include \"ctpl.h\"\n#include \"sidetasks.hpp\"\n\n#ifndef THREAD_POOL_SIZE\n# define THREAD_POOL_SIZE 2\n#endif\n\nusing namespace std;\nusing namespace Crails;\n\natomic_bool should_continue(true);\n\nstatic void shutdown(int) { should_continue = false; }\n\nstatic void run_task(const std::string& type, std::function<void(Params&)> runner, std::shared_ptr<Params> params, int id)\n{\n  try\n  {\n    Utils::Timer timer;\n\n    logger << Logger::Info\n      << \"# Running task of type \" << type << \" in \" << id << Logger::endl;\n    runner(*params);\n    logger << Logger::Info\n      << \"# Done (duration: \" << timer.GetElapsedSeconds() << \"s)\"\n      << Logger::endl;\n  }\n  catch (const std::exception& e)\n  {\n    logger << Logger::Error <<\n      \"[!] Catched exception: \" << e.what() << Logger::endl;\n  }\n}\n\nstatic void run_tasks(ctpl::thread_pool& pool, vector<string> tasks)\n{\n  auto* database = Sidekix::SideTaskDatabase::instance();\n\n  for (auto task_id : tasks)\n  {\n    auto data = std::make_shared<Crails::Params>();\n\n    if (database->take(task_id, *data))\n    {\n      string task_type = (*data)[\"sidekix\"][\"type\"];\n      auto runner = sidetasks.find(task_type);\n\n      if (runner == sidetasks.end())\n        logger << Logger::Error << \"/!\\\\ Scheduled task of unknown type `\" << task_type << '`' << Logger::endl;\n      else\n        pool.push(std::bind(&run_task, task_type, runner->second, data, std::placeholders::_1));\n    }\n  }\n}\n\nvoid initialize_pid_file(const std::string& filepath)\n{\n  std::ofstream output(filepath.c_str());\n\n  if (output.is_open())\n  {\n    pid_t pid = getpid();\n\n    logger << \">> PID \" << pid << \" (stored in \" << filepath << ')' << Logger::endl;\n    output << pid;\n    output.close();\n  }\n  else\n    logger << \"!! Failed to open PID file \" << filepath << Logger::endl;\n}\n\nint main(int argc, const char** argv)\n{\n  ctpl::thread_pool taskpool(THREAD_POOL_SIZE);\n  Sidekix::SideTaskDatabase* database;\n  SingletonInstantiator<LogFiles, int, const char**> log_files(argc, argv);\n\n  {\n    using namespace boost;\n    program_options::variables_map vm;\n    program_options::options_description desc(\"Options\");\n\n    desc.add_options()\n      (\"log,l\", program_options::value<std::string>(), \"log output file\")\n      (\"error,e\", program_options::value<std::string>(), \"error log output file\")\n      (\"pidfile\", program_options::value<std::string>(), \"pid file\");\n    program_options::store(program_options::parse_command_line(argc, argv, desc), vm);\n    program_options::notify(vm);\n    if (vm.count(\"pidfile\"))\n      initialize_pid_file(vm[\"pidfile\"].as<std::string>());\n  }\n\n  signal(SIGINT, &shutdown);\n  database = Sidekix::SideTaskDatabase::instance();\n  do {\n    run_tasks(taskpool, database->pending_tasks());\n    sleep(5);\n  } while (should_continue == true);\n}\n";
    return ecpp_stream.str();
  }
private:
  std::stringstream ecpp_stream;
};

std::string render_sidekix_main_cpp(const Crails::Renderer* renderer, Crails::SharedVars& vars)
{
  return SidekixMainCpp(renderer, vars).render();
}