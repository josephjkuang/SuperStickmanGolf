//
// Created by Joseph Kuang on 4/20/21.
// The Golf Course class is responsible for storing the position of the golf
// ball and course obstacles. This class is also responsible for determining
// how the golf ball moves_ inside the course.
//
#pragma once

#include "cinder/gl/gl.h"
#include "ball.h"

namespace golf {
using ci::Color;
using glm::vec2;
using std::vector;

class GolfCourse {
 public:
  /*
   * Initialization Constructor that creates the ball object, sets the boundaries
   * of the course, and sets up the beginning state of the course.
   */
  GolfCourse(const Color& color, float radius, const vec2& position,
             const vec2& velocity);

  void SetCourseMap(const vector<vector<char>>& course_map);

  /**
   * Method that is responsible for drawing all the ball
   */
  void Display() const;

  /**
   * Method that is responsible for advancing the position of the golf ball.
   * It follows different rules depending on if the gravity is on or if the ball
   * is attached to a sticky zone.
   */
  void AdvanceOneFrame();

  /**
   * Method that updates the balls velocity after it has been hit.
   * It will turn gravity back on, except when the ball is shot horizontally.
   * @param power the power of the hit
   * @param direction the direction that the ball is hit
   */
  void HitBall(unsigned power, unsigned angle);

  /**
   * Method that determines if the ball is at rest so it can be hit.
   * One Can Hit the ball if it is moving slow enough, gravity is off, and the
   * game has not already been won.
   * If cheat_mode_ is on, though, one can hit the ball whenever.
   * @return a boolean on if you can hit the ball.
   */
  bool CanHitBall() const;

  /**
   * Method that is called when the user decides to restart the course.
   */
  void ResetGame();

  /**
   * Method that switches the between only hitting the ball at rest and hitting
   * the ball whenever.
   */
  void SwitchMode();

  /**
   * Power-up that reverses the ball back to the place that it was just shot.
   */
  void UseMulligan();

  /**
   * Power-up that stops the ball in the middle of the air.
   */
  void UseAirBrakes();

  /**
   * Power-up that gives you an extra bounce to move a little farther
   */
  void UseExtraBounce();

  Ball GetBall() const;

  bool GetGameWon() const;

  unsigned GetMoves() const;

  unsigned GetPowerUps() const;

 private:
  static const unsigned kWindowWidth = 1400;
  static const unsigned kWindowHeight = 700;

  static constexpr float kCollisionDampener = 0.5;
  static constexpr float kGrassDampener = 0.7;
  static constexpr float kLossOfEnergy = 0.98;
  static constexpr float kLandBallSpeed = 1;
  static constexpr float kNotMovingSpeed = 0.1;
  static constexpr float kNotMovingPutSpeed = 0.5;

  static const unsigned kHorizontalAngle1 = 0;
  static const unsigned kHorizontalAngle2 = 180;
  static const unsigned kRadianAdjustment = 180;

  static const unsigned kInitialPowerUps = 3;
  static const int kExtraBounceValue = -5;

  static const char kEmpty = '0';
  static const char kWall = '1';
  static const char kWater = '2';
  static const char kSand = '3';
  static const char kSticky = '4';
  static const char kHole = '5';


  const Color kOutlineColor = Color("black");

  const vec2 kGravity = vec2(0, -0.1);
  vector<vector<char>> course_map_;

  Ball ball_;
  unsigned moves_;
  unsigned power_ups_;

  bool cheat_mode_;
  bool game_won_;
  bool gravity_present_;
  bool on_sticky_;
  bool on_sand_;

  vec2 original_position_;
  vec2 original_velocity_;
  vec2 last_position_;

  /**
   * Method responsible for seeing if a collision occurs between the current
   * position, and the expected position after adding the velocity to the position.
   */
  void ExamineBallTrajectory();

  /**
   * Helper Method for Updating the velocity of the ball when it collides
   * with the edge of the screen.
   * @param position the expected position of the golf ball.
   * @return a boolean if a window collision occurred.
   */
  bool FindWindowCollision(const vec2& position);

  /**
   * Helper Method for ExamineBallTrajectory that loops through the positions between
   * the ball's current position and expected position by incrementing x by one.
   * Need both an x and a y increment method because sometimes the ball will
   * then phase through the barriers.
   * @param x_start the x start position
   * @param x_end the x end position
   * @param y_start the y start position
   * @param y_end the y end position
   */
  bool FindCollisionIncrementingX(unsigned x_start, unsigned x_end, unsigned y_start, unsigned y_end);

  /**
   * Helper Method for ExamineBallTrajectory that loops through the positions between
   * the ball's current position and expected position by incrementing y by one.
   * This method technically doesn't need to return anything but just wanted to keep naming consistent.
   * @param x_start the x start position
   * @param x_end the x end position
   * @param y_start the y start position
   * @param y_end the y end position
   */
  bool FindCollisionIncrementingY(unsigned x_start, unsigned x_end, unsigned y_start, unsigned y_end);

  /**
   * Helper method that checks for the different types of collisions
   * @param x_current the current x-position.
   * @param y_current the current y-position.
   * @return a boolean if a collision occurs.
   */
  bool FindCollision(unsigned x_current, unsigned y_current);

  /**
   * Helper method to flip the direction of the ball with a slower speed.
   * @param x_pos the expected x position
   * @param y_pos the expected y position
   */
  void HitWall(unsigned x_pos, unsigned y_pos);

  /**
   * Helper method that will reset the position of the ball if it hits the water.
   */
  void HitWater();

  /**
   * Method that will stop the ball in the sand if it hits the sand.
   * @param x_pos the current x-position of the ball.
   * @param y_pos the current y-position of the ball.
   */
  void HitSand(unsigned x_pos, unsigned y_pos);

  /**
   * Method that will stop the ball if it hits a sticky zone.
   * @param x_pos the current x-position of the ball.
   * @param y_pos the current y-position of the ball.
   */
  void HitSticky(unsigned x_pos, unsigned y_pos);

  /**
   * Helper method so the ball doesn't fall through the ground.
   * @param y_pos the boundary that the ball will be resting on.
   */
  void LandBallOnGround(unsigned x_pos);

  /**
   * Method that checks to see if the ball has fallen off the surface, and if
   * gravity should be turned back on.
   * @return a boolean whether gravity should be turned back on.
   */
  bool TurnOnGravity();

  /**
   * Method that will slow down the ball for putting on the grass.
   */
  void UseGrassSpeed();
};

} // namespace golf
