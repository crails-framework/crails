#include "builder.hpp"
#include <filesystem>
#include <boost/process.hpp>
#include <iostream>
#include <regex>
#include <crails/render_file.hpp>
#include "../file_renderer.hpp"
#include "../file_collector.hpp"
#include <crails/utils/string.hpp>
#include <crails/utils/split.hpp>
#include <crails/cli/conventions.hpp>
#include <crails/cli/process.hpp>
#include <crails/logger.hpp>

using namespace std;

// html:
// crails templates -r html -i app/views -t Crails::HtmlTemplate -z crails/html_template.hpp
// json:
// crails templates -r json -i app/views -t Crails::JsonTemplate -z crails/json_template.hpp
// rss:
// crails templates -r rss -i app/views -t Crails::RssTemplate -z crails/rss_template.hpp

static string ecpp_binary(const ProjectConfiguration& configuration)
{
  string path = configuration.crails_bin_path();

  Crails::logger << "ecpp_binary lookup: crails_bin_path=" << path << Crails::Logger::endl;
  if (path.length() == 0)
  {
    string which_query = Crails::which("ecpp");

    if (which_query.length() == 0)
      throw std::runtime_error("Could not find ecpp binary");
    return which_query;
  }
  return path + "/ecpp";
}

void TemplateBuilder::options_description(boost::program_options::options_description& desc) const
{
  desc.add_options()
    ("verbose,v", "enable verbose mode")
    ("classname,n",       boost::program_options::value<string>(), "name of the Renderer class that will be generated")
    ("template-type,t",   boost::program_options::value<string>(), "name of the template class (defaults to Crails::Template)")
    ("template-header,z", boost::program_options::value<string>(), "path to the header defining the template class")
    ("stream-property,s", boost::program_options::value<string>(), "use a stream property provided by the template class")
    ("render-mode,m",     boost::program_options::value<string>(), "use raw or markup mode")
    ("pattern,p",         boost::program_options::value<string>(), "pattern matching input files")
    ("input,i",           boost::program_options::value<string>(), "input folders, separated by commas")
    ("renderer,r",        boost::program_options::value<string>(), "renderer type (ex: html)");
}

int TemplateBuilder::run()
{
  if (validate_options() && generate_templates())
    return generate_renderer_header() && generate_renderer_ctor() ? 0 : -1;
  return -1;
}

bool TemplateBuilder::validate_options()
{
  if (!options.count("renderer"))
  {
    cerr << "No renderer specified." << endl;
    return false;
  }
  if (!options.count("input"))
  {
    cerr << "No input directory specified." << endl;
    return false;
  }
  if (!options.count("classname"))
  {
    cerr << "No classname specified:" << endl;
    return false;
  }
  if (options.count("pattern"))
    pattern = options["pattern"].as<string>();
  renderer = options["renderer"].as<string>();
  renderer_name = options["classname"].as<string>();
  renderer_filename = Crails::naming_convention.filenames(renderer_name);
  function_prefix = Crails::underscore(this->renderer_name) + "_render";
  return true;
}

void TemplateBuilder::collect_files()
{
  auto inputs = Crails::split(options["input"].as<string>(), ',');

  for (const string& input : inputs)
  {
    if (filesystem::is_directory(input))
    {
      FileCollector collector(input, pattern);

      collector.collect_files([this, collector](const filesystem::path& path)
      {
        string filepath      = filesystem::canonical(path).string();
        string relative_path = collector.relative_path_for(path);
        string alias         = collector.name_for(path);
        string classname;

        for (unsigned int i = 0 ; i < alias.length() ; ++i)
        {
          if (alias[i] == '/' || alias[i] == '.') classname += '_';
          else classname += alias[i];
        }
        classname = Crails::camelize(classname);
        targets.emplace(filepath, Target{alias, classname, Crails::underscore(classname)});
      });
    }
  }
  all_targets = targets;
}

string TemplateBuilder::command_for_target(const pair<string, Target>& target) const
{
  stringstream command;

  command << ecpp_binary(configuration) << " -n " << target.second.classname << " -i " << target.first;
  if (options.count("render-mode"))
    command << " -m " << options["render-mode"].as<string>();
  if (options.count("template-type"))
    command << " -t " << options["template-type"].as<string>();
  if (options.count("template-header"))
    command << " -z " << options["template-header"].as<string>();
  command << " -p " << function_prefix;
  if (options.count("stream-property"))
    command << " --stream-property " << options["stream-property"].as<string>();
  if (options.count("verbose"))
    cout << "[TEMPLATE] " << command.str() << endl;
  return command.str();
}

bool TemplateBuilder::run_ecpp(const pair<string, Target>& target, Crails::RenderFile& output) const
{
  boost::process::ipstream pipe_stream;
  boost::process::child    ecpp(
    command_for_target(target),
    boost::process::std_out > pipe_stream
  );
  string       line;
  stringstream generated_template;

  while (pipe_stream && std::getline(pipe_stream, line))
    generated_template << line << '\n';
  ecpp.wait();
  if (ecpp.exit_code() != 0)
    return false;
  output.set_body(generated_template.str().c_str(), generated_template.str().length());
  return true;
}

void TemplateBuilder::prune_up_to_date_template(Targets::iterator it, filesystem::path template_path)
{
  filesystem::path directory(output_directory + '/' + renderer);
  filesystem::path existing_template_path(it->first);

  if (filesystem::last_write_time(existing_template_path) < filesystem::last_write_time(template_path))
  {
    if (options.count("verbose"))
      cout << "[TEMPLATE] " << it->second.alias << " already up to date." << endl;
    targets.erase(it);
  }
}

void TemplateBuilder::clear_dropped_templates()
{
  filesystem::path directory(output_directory + '/' + renderer);
  filesystem::recursive_directory_iterator dir(directory);
  list<filesystem::path> marked_for_deletion;

  for (auto& entry : dir)
  {
    string filename = entry.path().filename().string();
    bool   exists = false;
    auto   it = targets.begin();

    for (; it != targets.end() ; ++it)
    { if ((it->second.function + ".cpp") == filename) break ; }
    for (auto it = targets.begin() ; it != targets.end() && !exists ; ++it)
      exists = (it->second.function + ".cpp") == filename;
    if (it == targets.end())
      marked_for_deletion.push_back(entry.path());
    else
      prune_up_to_date_template(it, entry.path());
  }
  for (const auto& path : marked_for_deletion)
  {
    cout << "[TEMPLATE] Clearing " << path.string() << endl;
    filesystem::remove(path);
  }
}

bool TemplateBuilder::generate_templates()
{
  string renderer_output_directory = output_directory + '/' + renderer;

  collect_files();
  if (filesystem::is_directory(renderer_output_directory))
    clear_dropped_templates();
  for (auto it = targets.begin() ; it != targets.end() ; ++it)
  {
    string             generated_template;
    auto               target = pair{it->first, it->second};
    Crails::RenderFile render_target;

    cout << "[TEMPLATE] Generating template " << target.second.alias << endl;
    filesystem::create_directories(renderer_output_directory);
    if (!filesystem::is_directory(renderer_output_directory))
    {
      cerr << "[TEMPLATE] Could not create directory " << renderer_output_directory << endl;
      return false;
    }
    render_target.open(renderer_output_directory + '/' + target.second.function + ".cpp");
    if (!run_ecpp(target, render_target))
    {
      cerr << "[TEMPLATE] Could not generate " << target.second.alias << endl;
      return false;
    }
  }
  return true;
}

bool TemplateBuilder::generate_renderer_header()
{
  FileRenderer renderer;

  renderer.should_overwrite = true;
  renderer.vars["renderer_name"] = renderer_name;
  renderer.vars["renderer_type"] = this->renderer;
  renderer.generate_file(
    "renderer.hpp",
    output_directory + '/' + renderer_filename + ".hpp"
  );
  return true;
}

bool TemplateBuilder::generate_renderer_ctor()
{
  FileRenderer renderer;
  map<string, string>  target_var;

  for (auto it = all_targets.begin() ; it != all_targets.end() ; ++it)
    target_var.emplace(it->second.alias, Crails::underscore(it->second.classname));
  renderer.should_overwrite = true;
  renderer.vars["renderer_name"] = renderer_name;
  renderer.vars["renderer_filename"] = renderer_filename;
  renderer.vars["function_prefix"] = function_prefix;
  renderer.vars["targets"] = &target_var;
  renderer.generate_file(
    "renderer.cpp",
    output_directory + '/' + renderer_filename + ".cpp"
  );
  return true;
}
