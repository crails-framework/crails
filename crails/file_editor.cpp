#include "file_editor.hpp"
#include <crails/read_file.hpp>
#include <crails/render_file.hpp>
#include <regex>
#include <iostream>

CrailsFileEditor::CrailsFileEditor(const std::string& path) : path(path)
{
}

bool CrailsFileEditor::load_file()
{
  return Crails::read_file(path, contents);
}

bool CrailsFileEditor::use_symbol(const std::string& symbol, bool whole_line)
{
  std::string pattern_str = "^\\s*" + prefix_pattern + "\\s*" + symbol;
  if (whole_line) pattern_str += "\\s*$";
  std::regex pattern(pattern_str.c_str(), std::regex_constants::multiline | std::regex_constants::icase);
  auto match = std::sregex_iterator(contents.begin(), contents.end(), pattern);
  std::cout << "pattern= " << ("^\\s*" + prefix_pattern + "\\s*" + symbol + "\\s*$") << std::endl;

  if (match != std::sregex_iterator())
  {
    position = match->position() + match->length() + 1;
    return true;
  }
  else
    std::cout << "[FILE] Cannot find insert pattern `" << symbol << "` in file " << path << std::endl;
  return false;
}

void CrailsFileEditor::insert(const std::string& text)
{
  contents.insert(position, text);
}

void CrailsFileEditor::save_file()
{
  Crails::RenderFile render_target;

  render_target.open(path);
  render_target.set_body(contents.c_str(), contents.length());
  std::cout << "[EDITED] File `" << path << "`  was updated." << std::endl;
}
