#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"
#include <crails/utils/string.hpp>

class OdbModule : public CommandIndex
{
public:
  static const std::vector<std::string> supported_backends;

  static bool check_backends_validity(const std::list<std::string>& backends);

  struct OdbInstaller : public Command
  {
    int  run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      static const std::string backend_desc = "set backends (" + Crails::join(OdbModule::supported_backends, ',') + ')';
      desc.add_options()
        ("backends,b", boost::program_options::value<std::string>(), backend_desc.c_str());
    }
  };

  struct OdbDisabler : public Command
  {
    int run() override;
  };

  struct OdbBackendManager : public Command
  {
    int  run() override;
    void options_description(boost::program_options::options_description& desc) const override
    {
      static const std::string backend_desc = "add backends (" + Crails::join(OdbModule::supported_backends) + ')';
      desc.add_options()
        ("add,a", boost::program_options::value<std::string>(), backend_desc.c_str())
        ("remove,r", boost::program_options::value<std::string>(), "remove backends");
    }
  };

  OdbModule();

  std::string_view description() const override { return "the odb module provides SQL-backed models"; }
};
