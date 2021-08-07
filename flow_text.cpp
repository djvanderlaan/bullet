#include "flow_text.h"
#include <locale>
#include <stdexcept>

std::vector<std::string> flow_text(const std::string txt, const int ncolumns) {
  if (ncolumns < 1) 
    throw std::runtime_error("Number of columns needs to be larger than 0.");
  std::vector<std::string> result;
  auto last_white = txt.end();
  auto last_position = txt.begin();
  int  column = 0;
  for (auto c = txt.begin(); c != txt.end(); ++c, ++column) {
    if (std::isspace(*c)) {
      last_white = c;
    }
    if (column >= ncolumns) {
      if (last_white != txt.end()) {
        result.push_back({last_position, last_white});
        last_position = last_white+1;
        column = c - last_position;
        last_white = txt.end();
      } else {
        result.push_back({last_position, c});
        last_position = c;
        column = c - last_position;
        last_white = txt.end();
      }
    }
  }
  if (last_position != txt.end()) result.push_back({last_position, txt.end()});
  return result;
}

