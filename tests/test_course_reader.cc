//
// Created by Joseph Kuang on 5/4/21.
// Test File For Course Reader.
//

#include <course_reader.h>

#include <catch2/catch.hpp>
#include <fstream>

using golf::CourseReader;
using std::ifstream;
using std::vector;

TEST_CASE("Test Reading in Text Files") {
  SECTION("Test Loading Small Regular Text File") {
    CourseReader courseReader = CourseReader(3, 3);
    ifstream input_file("../../../../../../static/test/regular.txt");
    if (input_file.is_open()) {
      input_file >> courseReader;
      input_file.close();
    }
    vector<vector<char>> expected = { { '1', '1', '1' }, { '1', '0', '1' }, { '1', '1', '1' } };
    REQUIRE(expected == courseReader.GetPixelTypes());
  }

  SECTION("Test Empty Text File") {
    CourseReader courseReader = CourseReader(3, 3);
    ifstream input_file("../../../../../../static/test/empty.txt");
    if (input_file.is_open()) {
      input_file >> courseReader;
      input_file.close();
    }
    vector<vector<char>> expected = { { '0', '0', '0' }, { '0', '0', '0' }, { '0', '0', '0' } };
    REQUIRE(expected == courseReader.GetPixelTypes());
  }

  SECTION("Test Non-Existent Text File") {
    CourseReader courseReader = CourseReader(3, 3);
    ifstream input_file("../../../../../../static/test/blahblahblah.txt");
    if (input_file.is_open()) {
      input_file >> courseReader;
      input_file.close();
    }
    vector<vector<char>> expected = { { '0', '0', '0' }, { '0', '0', '0' }, { '0', '0', '0' } };
    REQUIRE(expected == courseReader.GetPixelTypes());
  }

  SECTION("Test Wrong Chars Text File") {
    CourseReader courseReader = CourseReader(3, 3);
    ifstream input_file("../../../../../../static/test/wrong_char.txt");
    if (input_file.is_open()) {
      input_file >> courseReader;
      input_file.close();
    }
    vector<vector<char>> expected = { { 'a', 'b', 'c' }, { 'd', 'e', 'f' }, { 'g', 'h', 'i' } };
    REQUIRE(expected == courseReader.GetPixelTypes());
  }

  SECTION("Test Wrong Data Type (Strings) Text File") {
    CourseReader courseReader = CourseReader(3, 3);
    ifstream input_file("../../../../../../static/test/strings.txt");
    if (input_file.is_open()) {
      input_file >> courseReader;
      input_file.close();
    }
    vector<vector<char>> expected = { { '0', '0', '0' }, { '0', '0', '0' }, { '0', '0', '0' } };
    REQUIRE(expected == courseReader.GetPixelTypes());
  }

  SECTION("Test Text File With Too Few Rows") {
    CourseReader courseReader = CourseReader(4, 3);
    ifstream input_file("../../../../../../static/test/regular.txt");
    if (input_file.is_open()) {
      input_file >> courseReader;
      input_file.close();
    }
    vector<vector<char>> expected = { { '1', '1', '1' }, { '1', '0', '1' }, { '1', '1', '1' }, { '0', '0', '0' } };
    REQUIRE(expected == courseReader.GetPixelTypes());
  }

  SECTION("Test Text File With Too Few Columns") {
    CourseReader courseReader = CourseReader(3, 4);
    ifstream input_file("../../../../../../static/test/regular.txt");
    if (input_file.is_open()) {
      input_file >> courseReader;
      input_file.close();
    }
    vector<vector<char>> expected = { { '1', '1', '1', '0' }, { '1', '0', '1', '0' }, { '1', '1', '1', '0' } };
    REQUIRE(expected == courseReader.GetPixelTypes());
  }

  SECTION("Test Text File With Too Many Rows") {
    CourseReader courseReader = CourseReader(2, 3);
    ifstream input_file("../../../../../../static/test/regular.txt");
    if (input_file.is_open()) {
      input_file >> courseReader;
      input_file.close();
    }
    vector<vector<char>> expected = { { '1', '1', '1' }, { '1', '0', '1' } };
    REQUIRE(expected == courseReader.GetPixelTypes());
  }

  SECTION("Test Text File With Too Many Columns") {
    CourseReader courseReader = CourseReader(3, 2);
    ifstream input_file("../../../../../../static/test/regular.txt");
    if (input_file.is_open()) {
      input_file >> courseReader;
      input_file.close();
    }
    vector<vector<char>> expected = { { '1', '1' }, { '1', '0' }, { '1', '1' } };
    REQUIRE(expected == courseReader.GetPixelTypes());
  }
}