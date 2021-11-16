//
// Created by Joseph Kuang on 4/29/21.
//

#include "course_reader.h"

#include <iostream>
#include <sstream>

namespace golf {
CourseReader::CourseReader(unsigned int rows, unsigned int cols) {
  rows_ = rows;
  cols_ = cols;

  std::vector<char> inner(cols_, '0');
  pixel_types_ = std::vector<std::vector<char>>(rows_, inner);
}

istream& operator>> (istream& is, golf::CourseReader& courseReader) {
  std::string line;
  unsigned row = 0;

  while (std::getline(is, line) && row < courseReader.rows_) {
    std::stringstream line_stream(line);
    char value_str;
    unsigned column = 0;

    while (line_stream >> value_str) {
      courseReader.pixel_types_[row][column] = value_str;
      column++;
    }
    row++;
  }
  return is;
}

vector<vector<char>> CourseReader::GetPixelTypes() const {
  return pixel_types_;
}

} // namespace golf
