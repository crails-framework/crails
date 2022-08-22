#pragma once
#include <boost/program_options.hpp>

class ScaffoldModel
{
public:
  virtual void options_description(boost::program_options::options_description& desc) const = 0;
  virtual int create(boost::program_options::variables_map&) = 0;

protected:
  static const char* properties_help() { return "properties, such as: -p std::string/name 'unsigned int/age'"; }
  static std::map<std::string, std::string> properties_option(const boost::program_options::variables_map&);
};
