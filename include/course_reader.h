//
// Created by Joseph Kuang on 4/29/21.
// The Course Reader Class is Responsible for Taking in a Path to a Course
// and Creating the Map that will be used.
//
#pragma once

#include <istream>
#include <vector>

namespace golf {
using std::istream;
using std::vector;

class CourseReader {
 public:
  /**
   * Constructor that takes in the number of rows and columns of the course to
   * create the nested vector course_map_ representing the course
   * @param rows number of rows
   * @param cols number of columns
   */
  CourseReader(unsigned rows, unsigned cols);

  /**
   * Operator overload for >> that reads in a file and initializes course_map_
   * @param is the istream
   * @param courseReader the courseReader object
   * @return the istream
   */
  friend istream& operator>> (istream& is, CourseReader& courseReader);

  vector<vector<char>> GetPixelTypes() const;

 private:
  unsigned rows_;
  unsigned cols_;
  vector<vector<char>> pixel_types_;
};

} // namespace golf
