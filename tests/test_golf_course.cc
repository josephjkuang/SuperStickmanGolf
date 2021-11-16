//
// Created by Joseph Kuang on 4/22/21.
// Test File For Golf Course Class
//

#include <catch2/catch.hpp>
#include <course_reader.h>
#include <golf_course.h>

using ci::Color;
using glm::vec2;
using golf::CourseReader;
using golf::GolfCourse;
using std::ifstream;

TEST_CASE("Test Advance One Frame Simple Movement") {
  GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                      vec2(100, 100), vec2(0, 0));
  CourseReader courseReader = CourseReader(1400, 700);
  ifstream input_file("../../../../../../static/courses/course.txt");

  if (input_file.is_open()) {
    input_file >> courseReader;
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    input_file.close();
  }

  SECTION("Velocity Updated From Gravity") {
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(0, 0.1) == golf_course.GetBall().GetVelocity());
  }

  SECTION("Position Updated From Gravity") {
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(100, 100.1) == golf_course.GetBall().GetPosition());
  }

  SECTION("Position Updated From Gravity and Initial Velocity") {
    GolfCourse golf_course2 = GolfCourse(Color("white"), 5,
                                        vec2(100, 100), vec2(2));
    golf_course2.SetCourseMap(courseReader.GetPixelTypes());
    golf_course2.AdvanceOneFrame();
    REQUIRE(vec2(102, 102.1) == golf_course2.GetBall().GetPosition());
  }
}

TEST_CASE("Test Collisions With Edge of Window") {
  SECTION("Collision when Ball Will Travel Past Right Wall") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                         vec2(1395, 100), vec2(6, 0));
    golf_course.AdvanceOneFrame();

    REQUIRE(vec2(-3, 0.1) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(1392, 100.1) == golf_course.GetBall().GetPosition());
  }

  SECTION("Collision when Outer Radius of Ball Will Hit The Right Wall") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1392, 100), vec2(6, 0));
    golf_course.AdvanceOneFrame();

    REQUIRE(vec2(-3, 0.1) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(1392, 100.1) == golf_course.GetBall().GetPosition());
  }

  SECTION("Collision when Ball Will Travel Past Left Wall") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(5, 100), vec2(-6, 0));
    golf_course.AdvanceOneFrame();

    REQUIRE(vec2(3, 0.1) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(8, 100.1) == golf_course.GetBall().GetPosition());
  }

  SECTION("Collision when Outer Radius of Ball Will Hit The Left Wall") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(8, 100), vec2(-6, 0));
    golf_course.AdvanceOneFrame();

    REQUIRE(vec2(3, 0.1) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(8, 100.1) == golf_course.GetBall().GetPosition());
  }

  SECTION("Collision when Ball Will Travel Past Bottom Wall") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(100, 1395), vec2(0, 6));
    golf_course.AdvanceOneFrame();

    REQUIRE(vec2(0, -3.05) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(100, 1391.95) == golf_course.GetBall().GetPosition());
  }

  SECTION("Collision when Outer Radius of Ball Will Hit The Bottom Wall") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(100, 1392), vec2(0, 6));
    golf_course.AdvanceOneFrame();

    REQUIRE(vec2(0, -3.05) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(100, 1388.95) == golf_course.GetBall().GetPosition());
  }

  SECTION("Collision when Ball Hits Corner") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(5, 1395), vec2(-6, 6));
    golf_course.AdvanceOneFrame();

    REQUIRE(vec2(3, -3.05) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(8, 1391.95) == golf_course.GetBall().GetPosition());
  }

  SECTION("No Collision when Ball Will Approach Vertical Window") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(100, 2), vec2(0, -6));
    golf_course.AdvanceOneFrame();

    REQUIRE(vec2(0, -5.9) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(100, -3.9) == golf_course.GetBall().GetPosition());
  }
}

TEST_CASE("Test Collision With Walls") {
  CourseReader courseReader = CourseReader(1400, 700);
  ifstream input_file("../../../../../../static/courses/course.txt");

  if (input_file.is_open()) {
    input_file >> courseReader;
    input_file.close();
  }

  SECTION("Test Horizontal Collision with Greater Horizontal Speed") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(654, 100), vec2(5, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(-2.5, 1.1) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(651.5, 101.1) == golf_course.GetBall().GetPosition());
  }

  SECTION("Test Horizontal Collision with Greater Vertical Speed") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(654, 100), vec2(5, 10));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(-2.5, 10.1) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(651.5, 110.1) == golf_course.GetBall().GetPosition());
  }

  SECTION("Test Horizontal Collision with 0 Vertical Speed") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(654, 100), vec2(5, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(-2.5, 0.1) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(651.5, 100.1) == golf_course.GetBall().GetPosition());
  }

  SECTION("Test Vertical Collision with Greater Horizontal Speed") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(700, 388), vec2(8, -5));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(8, 2.45) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(708, 390.45) == golf_course.GetBall().GetPosition());
  }

  SECTION("Test Vertical Collision with Greater Vertical Speed") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(700, 388), vec2(4, -5));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(4, 2.45) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(704, 390.45) == golf_course.GetBall().GetPosition());
  }

  SECTION("Test Vertical Collision with 0 Horizontal Speed") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(700, 388), vec2(0, -5));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(0, 2.45) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(700, 390.45) == golf_course.GetBall().GetPosition());
  }

  SECTION("Test Collision at Corner") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(654, 388), vec2(5, -5));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(-2.5, 2.45) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(651.5, 390.45) == golf_course.GetBall().GetPosition());
  }
}

TEST_CASE("Test Collision With Special Barriers") {
  CourseReader courseReader = CourseReader(1400, 700);
  ifstream input_file("../../../../../../static/courses/course.txt");

  if (input_file.is_open()) {
    input_file >> courseReader;
    input_file.close();
  }

  // Only Need to Test These Collisions From One Direction Because They Use The
  // Same Code as Walls Which Already Tested it.
  SECTION("Test Collision with Sticky Zone") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(646, 100), vec2(5, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(0, 0) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(650, 104) == golf_course.GetBall().GetPosition());
  }

  SECTION("Test Collision with Sand Zone") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(700, 285), vec2(2, 5));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(0, 0) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(705, 288) == golf_course.GetBall().GetPosition());
  }

  SECTION("Test Collision with Water") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(300, 598), vec2(0, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.SwitchMode();
    golf_course.HitBall(10, 270);
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(0, 0) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(300, 598) == golf_course.GetBall().GetPosition());
  }
}

TEST_CASE("Test Turn off Gravity") {
  CourseReader courseReader = CourseReader(1400, 700);
  ifstream input_file("../../../../../../static/courses/course.txt");

  if (input_file.is_open()) {
    input_file >> courseReader;
    input_file.close();
  }

  SECTION("Test Landing Ball on Horizontal Surface with Minimum Speed") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(3, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(3, 0) == golf_course.GetBall().GetVelocity());
    REQUIRE(vec2(1303, 182) == golf_course.GetBall().GetPosition());
  }

  SECTION("Test Gravity is Turned off When Ball Is Resting On Horizontal Surface") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(0, 0) == golf_course.GetBall().GetVelocity());
  }

  SECTION("Test Gravity is Turned off When Ball Hits Sticky") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(646, 100), vec2(5, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(0, 0) == golf_course.GetBall().GetVelocity());
  }

  SECTION("Test Gravity is Turned off When Ball Hits Sand") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(700, 285), vec2(2, 5));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(0, 0) == golf_course.GetBall().GetVelocity());
  }

  SECTION("Test Friction for Putting") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 1));
    // Getting the ball to land and turn off gravity
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    golf_course.HitBall(5, 0);
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(4.9, 0) == golf_course.GetBall().GetVelocity());
  }

  SECTION("Test Collisions Still Occur When Gravity is Off") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1393, 186), vec2(0, 1));
    // Getting the ball to land and turn off gravity
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(0, 0) == golf_course.GetBall().GetVelocity());
    golf_course.HitBall(10, 0);
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(-4.9, 0) == golf_course.GetBall().GetVelocity());
  }

  SECTION("Test Friction for Putting Eventually Sets Velocity to be Exactly 0") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1393, 186), vec2(0, 1));
    // Getting the ball to land and turn off gravity
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    golf_course.HitBall(1, 180);
    for (size_t i = 0; i < 40; i++) {
      golf_course.AdvanceOneFrame();
    }
    REQUIRE(vec2(0, 0) == golf_course.GetBall().GetVelocity());
  }
}

TEST_CASE("Test Reset Game") {
  CourseReader courseReader = CourseReader(1400, 700);
  ifstream input_file("../../../../../../static/courses/course.txt");

  if (input_file.is_open()) {
    input_file >> courseReader;
    input_file.close();
  }

  GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                       vec2(640, 100), vec2(5, 0));
  golf_course.SetCourseMap(courseReader.GetPixelTypes());
  golf_course.AdvanceOneFrame();
  golf_course.AdvanceOneFrame(); // Ball Hits Sticky Zone, Gravity is Turned Off, and Velocity is Zero
  golf_course.ResetGame();

  SECTION("Test Position Reset") {
    REQUIRE(vec2(640, 100) == golf_course.GetBall().GetPosition());
  }

  SECTION("Test Velocity Reset") {
    REQUIRE(vec2(5, 0) == golf_course.GetBall().GetVelocity());
  }

  SECTION("Test Gravity Turned Back On") {
    golf_course.AdvanceOneFrame();
    REQUIRE(vec2(5, 0.1) == golf_course.GetBall().GetVelocity());
  }

  SECTION("Test Number of Moves Resets") {
    GolfCourse golf_course2 = GolfCourse(Color("white"), 2,
                                        vec2(1300, 600), vec2(0, 1));
    golf_course2.SetCourseMap(courseReader.GetPixelTypes());
    golf_course2.SwitchMode();
    golf_course2.HitBall(1, 1);
    golf_course2.ResetGame();
    REQUIRE(0 == golf_course2.GetMoves());
  }

  SECTION("Test Reset Win Game") {
    GolfCourse golf_course2 = GolfCourse(Color("white"), 2,
                                        vec2(1220, 185), vec2(0, 0));
    golf_course2.SetCourseMap(courseReader.GetPixelTypes());
    golf_course2.AdvanceOneFrame();
    golf_course2.ResetGame();
    REQUIRE(!golf_course2.GetGameWon());
  }

  SECTION("Test Number of Power-Ups Resets") {
    GolfCourse golf_course2 = GolfCourse(Color("white"), 2,
                                         vec2(1220, 185), vec2(0, 0));
    golf_course2.SetCourseMap(courseReader.GetPixelTypes());
    golf_course2.SwitchMode();
    golf_course2.HitBall(1, 1);
    golf_course2.UseMulligan();
    golf_course2.ResetGame();
    REQUIRE(3 == golf_course2.GetPowerUps());
  }
}

TEST_CASE("Test Winning Game") {
  CourseReader courseReader = CourseReader(1400, 700);
  ifstream input_file("../../../../../../static/courses/course.txt");

  if (input_file.is_open()) {
    input_file >> courseReader;
    input_file.close();
  }

  SECTION("Test Win Game When Ball At Rest") {
    GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                         vec2(1220, 185), vec2(0, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(golf_course.GetGameWon());
  }

  SECTION("Test No Win Game When in Wrong Position") {
    GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                        vec2(100, 185), vec2(0, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(!golf_course.GetGameWon());
  }

  SECTION("Test No Win Game When Moving Too Fast") {
    GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                        vec2(1213, 185), vec2(5, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(!golf_course.GetGameWon());
  }

  SECTION("Test Win Game When Ball Moving Slow Enough") {
    GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                        vec2(1213, 185), vec2(0.5, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(golf_course.GetGameWon());
  }

  SECTION("Test Can't Switch To Cheat Mode After Winning Game") {
    GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                        vec2(1220, 185), vec2(0, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    golf_course.SwitchMode();
    REQUIRE(!golf_course.CanHitBall());
  }
}

TEST_CASE("Test Hitting Ball") {
  CourseReader courseReader = CourseReader(1400, 700);
  ifstream input_file("../../../../../../static/courses/course.txt");

  if (input_file.is_open()) {
    input_file >> courseReader;
    input_file.close();
  }

  SECTION("Test Can Hit Ball") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame(); // Getting the ball to land in sand and turn off gravity
    REQUIRE(golf_course.CanHitBall());
  }

  SECTION("Test Can Hit Ball in Cheat Mode") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(100, 186), vec2(0, 10));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.SwitchMode();
    REQUIRE(golf_course.CanHitBall());
  }

  SECTION("Test Can't Move When Gravity is On") {
    GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                        vec2(100, 185), vec2(0, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    REQUIRE(!golf_course.CanHitBall());
  }

  SECTION("Test Can't Move When Ball is Moving Too Fast") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame(); // Getting the ball to land in sand and turn off gravity
    golf_course.HitBall(5, 0); // Hitting the Ball At Rest
    REQUIRE(!golf_course.CanHitBall()); // Trying to Hit The Ball Again
  }

  SECTION("Test Can Hit Ball When Moving Slow Enough") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    golf_course.HitBall(1, 0);

    // Getting Ball to be not zero but less than minimum speed 0.1 to hit again
    for (size_t i = 0; i < 35; i++) {
      golf_course.AdvanceOneFrame();
    }
    REQUIRE(golf_course.CanHitBall());
  }

  SECTION("Test Can't Hit Ball When Game is Won") {
    GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                        vec2(1220, 185), vec2(0, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();
    REQUIRE(!golf_course.CanHitBall());
  }

  SECTION("Test Move Counter") {
    GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                        vec2(100, 185), vec2(0, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.SwitchMode();
    golf_course.HitBall(0, 0);
    golf_course.HitBall(0, 0);
    REQUIRE(2 == golf_course.GetMoves());
  }

  SECTION("Test Hit Ball Horizontally Normally") {
    GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                        vec2(100, 100), vec2(0, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.SwitchMode();
    golf_course.HitBall(5, 0);
    REQUIRE(vec2(5, 0) == golf_course.GetBall().GetVelocity());
  }

  SECTION("Test Hit Ball Vertically Normally") {
    GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                        vec2(100, 100), vec2(0, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.SwitchMode();
    golf_course.HitBall(5, 90);

    // Speed doesn't get to exactly zero because PI isn't an exact value
    REQUIRE(0 == Approx(golf_course.GetBall().GetVelocity().x).margin(0.001));
    REQUIRE(-5 == golf_course.GetBall().GetVelocity().y);
  }

  SECTION("Test Hit Ball Diagonally Normally") {
    GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                        vec2(100, 100), vec2(0, 0));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.SwitchMode();
    golf_course.HitBall(5, 30);

    REQUIRE(4.330 == Approx(golf_course.GetBall().GetVelocity().x).margin(0.001));
    REQUIRE(-2.5 == golf_course.GetBall().GetVelocity().y);
  }

  // Testing Hitting The Ball Into Water Was Already Tested in: Test Collision With Special Barriers

  SECTION("Test Gravity isn't Turned Backed on After Horizontal Hit") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 600), vec2(0, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame(); // Gravity is Turned off Here When Hits Sand

    golf_course.HitBall(5, 0);
    golf_course.AdvanceOneFrame();

    REQUIRE(0 == golf_course.GetBall().GetVelocity().y);
  }

  SECTION("Test Gravity is Turned Backed on After Hit") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 600), vec2(0, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();

    golf_course.HitBall(5, 90);
    golf_course.AdvanceOneFrame();

    REQUIRE(-4.9 == Approx(golf_course.GetBall().GetVelocity().y).margin(0.001));
  }

  SECTION("Test You Cannot Hit The Ball Farther Down The Sand") {
    GolfCourse golf_course = GolfCourse(Color("white"), 2,
                                        vec2(1300, 600), vec2(0, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame();

    golf_course.HitBall(5, 270);
    golf_course.AdvanceOneFrame();

    REQUIRE(0 == golf_course.GetBall().GetVelocity().y);
  }
}

TEST_CASE("Test Power-Ups") {
  CourseReader courseReader = CourseReader(1400, 700);
  ifstream input_file("../../../../../../static/courses/course.txt");

  if (input_file.is_open()) {
    input_file >> courseReader;
    input_file.close();
  }

  SECTION("Test Initial Number of Power-Ups") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 1));
    REQUIRE(3 == golf_course.GetPowerUps());
  }

  SECTION("Test Number of Power-Ups Left After Using One") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.UseAirBrakes();
    REQUIRE(2 == golf_course.GetPowerUps());
  }

  SECTION("Test Number of Moves After Using a Mulligan") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.AdvanceOneFrame(); // Getting the ball to land in sand and turn off gravity
    golf_course.HitBall(0, 0);
    golf_course.UseMulligan();
    REQUIRE(0 == golf_course.GetMoves());
  }

  SECTION("Test Using Air Brakes") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(10, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.UseAirBrakes();
    REQUIRE(vec2(0, 0) == golf_course.GetBall().GetVelocity());
  }

  SECTION("Test Using Extra Bonce") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(10, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());
    golf_course.UseExtraBounce();
    REQUIRE(vec2(10, -4) == golf_course.GetBall().GetVelocity());
  }

  SECTION("Test Using Power Up When Out of PowerUps") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 1));
    golf_course.SetCourseMap(courseReader.GetPixelTypes());

    golf_course.AdvanceOneFrame(); // Getting the ball to land in sand and turn off gravity

    // Wasting Power-Ups
    golf_course.HitBall(0, 0);
    golf_course.UseMulligan();
    golf_course.HitBall(0, 0);
    golf_course.UseMulligan();
    golf_course.HitBall(0, 0);
    golf_course.UseMulligan();

    // Power-Ups have run out now.
    golf_course.HitBall(0, 0);
    golf_course.UseMulligan();

    REQUIRE(1 == golf_course.GetMoves());
  }

  SECTION("Test Using Mulligan When No Moves Have Been Made") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 0));
    golf_course.UseMulligan();
    REQUIRE(3 == golf_course.GetPowerUps());
  }

  SECTION("Test Using Mulligan When Ball is Moving") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 10));
    golf_course.UseMulligan();
    REQUIRE(3 == golf_course.GetPowerUps());
  }

  SECTION("Test Using Air Brakes When Ball is not moving") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 0));
    golf_course.UseAirBrakes();
    REQUIRE(3 == golf_course.GetPowerUps());
  }

  SECTION("Test Using Extra Bounce When Ball is not moving") {
    GolfCourse golf_course = GolfCourse(Color("white"), 5,
                                        vec2(1300, 186), vec2(0, 0));
    golf_course.UseExtraBounce();
    REQUIRE(3 == golf_course.GetPowerUps());
  }
}