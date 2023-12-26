#pragma once
#include "../../command_index.hpp"
#include "../../command.hpp"

class MetarecordPlugin : public CommandIndex
{
public:
  std::string_view description() const override { return "generates model code from a Ruby-based DSL"; }

  struct MetarecordInstaller : public ::Command
  {
    std::string_view description() const override { return "install the metarecord plugin"; }
    int run() override;
    void options_description(boost::program_options::options_description&) const override;
  };

  struct MetarecordGenerators : public ::Command
  {
    std::string_view description() const override { return "list, add or remove metarecord generators"; }
    int run() override;
    void options_description(boost::program_options::options_description&) const override;
  };

  MetarecordPlugin();

  static bool build(const ProjectConfiguration&, bool verbose);
  static bool has_view_generator(const ProjectConfiguration&);
};
