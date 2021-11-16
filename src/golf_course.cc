//
// Created by Joseph Kuang on 4/20/21.
//

#include "golf_course.h"
#define PI 3.14159265

namespace golf {
using ci::gl::color;
using ci::gl::drawSolidCircle;
using glm::length;

GolfCourse::GolfCourse(const Color& color, float radius,
                       const vec2& position, const vec2& velocity) {
  original_position_ = position;
  original_velocity_ = velocity;
  ball_ = Ball(color, radius, position, velocity);

  moves_ = 0;
  power_ups_ = kInitialPowerUps;

  gravity_present_ = true;
  on_sticky_ = false;
  on_sand_ = false;
  game_won_ = false;
  cheat_mode_ = false;
}

void GolfCourse::SetCourseMap(const vector<vector<char>>& course_map) {
  course_map_ = course_map;
}

void GolfCourse::Display() const {
  color(Color(kOutlineColor));
  drawSolidCircle(ball_.GetPosition(), ball_.GetRadius() + 1);
  color(Color(ball_.GetColor()));
  drawSolidCircle(ball_.GetPosition(), ball_.GetRadius());
}

void GolfCourse::AdvanceOneFrame() {
  if (gravity_present_) {
    ball_.SetVelocity(ball_.GetVelocity() - kGravity);
    ExamineBallTrajectory();
    ball_.SetPosition(ball_.GetPosition() + ball_.GetVelocity());
  } else if (!on_sticky_){
    if (TurnOnGravity()) {
      // Recalling this method with gravity turned on now.
      AdvanceOneFrame();
    } else {
      ExamineBallTrajectory();
      UseGrassSpeed();
    }
  }
}

void GolfCourse::HitBall(unsigned power, unsigned angle) {
  if (CanHitBall()) {
    moves_++;
    on_sticky_ = false;
    gravity_present_ = true;
    last_position_ = ball_.GetPosition();

    if (angle == kHorizontalAngle1 || angle == kHorizontalAngle2) {
      // Want to keep gravity off for hitting horizontally.
      gravity_present_ = false;
    }

    ball_.SetVelocity(vec2(power * cos(PI * angle / kRadianAdjustment),
                           -(power * sin(PI * angle / kRadianAdjustment))));

    // Making sure you can't hit the ball farther down the sand.
    if (on_sand_ && ball_.GetVelocity().y > 0) {
      ball_.SetVelocity(vec2(ball_.GetVelocity().x, 0));
      gravity_present_ = false;
    } else {
      on_sand_ = false;
    }

    ExamineBallTrajectory();
    ball_.SetPosition(ball_.GetPosition() + ball_.GetVelocity());
  } else {
    std::cout << "Sorry, you cannot move until your ball is at rest. Your speed: "
              << length(ball_.GetVelocity()) << std::endl;
  }
}

bool GolfCourse::CanHitBall() const{
  return (length(ball_.GetVelocity()) < kNotMovingSpeed && !gravity_present_ && !game_won_)
         ||
         cheat_mode_;
}

void GolfCourse::ResetGame() {
  ball_.SetPosition(original_position_);
  ball_.SetVelocity(original_velocity_);

  gravity_present_ = true;
  on_sticky_ = false;
  on_sand_ = false;
  game_won_ = false;
  cheat_mode_ = false;

  moves_ = 0;
  power_ups_ = kInitialPowerUps;
}

void GolfCourse::SwitchMode() {
  if (!game_won_) {
    cheat_mode_ = !cheat_mode_;
  }
}

void GolfCourse::ExamineBallTrajectory() {
  vec2 expected_position = ball_.GetPosition() + ball_.GetVelocity();
  if (!FindWindowCollision(expected_position)) {
    // Standardizing movement inside for loop to be left to right
    float x_start = ball_.GetPosition().x + 1;
    float x_end = expected_position.x + ball_.GetRadius();
    if (expected_position.x < ball_.GetPosition().x) {
      x_start = expected_position.x - ball_.GetRadius();
      x_end = ball_.GetPosition().x - 1;
    }

    // Standardizing movement inside for loop to be top to bottom
    float y_start = ball_.GetPosition().y + 1;
    float y_end = expected_position.y + ball_.GetRadius();
    if (expected_position.y < ball_.GetPosition().y) {
      y_start = expected_position.y - ball_.GetRadius();
      y_end = ball_.GetPosition().y - 1;
    }

    if (ball_.GetPosition().x - expected_position.x == 0) {
      FindCollisionIncrementingY(x_start, x_end, y_start, y_end);
    } else if (ball_.GetPosition().y - expected_position.y == 0) {
      FindCollisionIncrementingX(x_start, x_end, y_start, y_end);
    } else {
      if (!FindCollisionIncrementingX(x_start, x_end, y_start, y_end)) {
        FindCollisionIncrementingY(x_start, x_end, y_start, y_end);
      }
    }
  }
}

bool GolfCourse::FindWindowCollision(const vec2& position) {
  bool collision_present = false;

  // Collisions with the Right and Left Walls
  if (position.x + ball_.GetRadius() + 1 >= kWindowWidth) {
    ball_.SetVelocity(ball_.GetVelocity() * vec2(-kCollisionDampener, 1));
    ball_.SetPosition(vec2(kWindowWidth - ball_.GetRadius(), ball_.GetPosition().y));
    collision_present = true;
  } else if (position.x - ball_.GetRadius() - 1 <= 0) {
    ball_.SetVelocity(ball_.GetVelocity() * vec2(-kCollisionDampener, 1));
    ball_.SetPosition(vec2(ball_.GetRadius(), ball_.GetPosition().y));
    collision_present = true;
  }

  // Collisions with the Bottom Wall
  if (position.y + ball_.GetRadius() >= kWindowHeight && ball_.GetVelocity().y >= 0) {
    ball_.SetVelocity(ball_.GetVelocity() * vec2(1, -kCollisionDampener));
    if (abs(ball_.GetVelocity().y) <= kLandBallSpeed) {
      LandBallOnGround(kWindowHeight - ball_.GetRadius());
    }
    collision_present = true;
  } else if (position.y - ball_.GetRadius() <= 0) {
    // No collision will occur but implementation requires a collision to seemingly occur.
    collision_present = true;
  }
  return collision_present;
}

bool GolfCourse::FindCollisionIncrementingX(unsigned x_start, unsigned x_end, unsigned y_start, unsigned y_end) {
  float slope = static_cast<float>(y_end - y_start) / (x_end - x_start);
  unsigned y_current;

  for (size_t x_current = x_start; x_current <= x_end; x_current++) {
    y_current = slope * (x_end - x_current) + y_start;
    if (FindCollision(x_current, y_current)) {
      return true;
    }
  }
  return false;
}

bool GolfCourse::FindCollisionIncrementingY(unsigned x_start, unsigned x_end, unsigned y_start, unsigned y_end) {
  float slope = static_cast<float>(x_end - x_start) / (y_end - y_start);
  unsigned x_current;

  for (size_t y_current = y_start; y_current <= y_end; y_current++) {
    x_current = slope * (y_end - y_current) + x_start;
    if (FindCollision(x_current, y_current)) {
      return true;
    }
  }
  return false;
}

bool GolfCourse::FindCollision(unsigned x_current, unsigned y_current) {
  if (course_map_[x_current][y_current] == kWall) {
    HitWall(x_current, y_current);
    return true;
  } else if (course_map_[x_current][y_current] == kWater) {
    HitWater();
    return true;
  } else if (course_map_[x_current][y_current] == kSand) {
    HitSand(x_current, y_current);
    return true;
  } else if (course_map_[x_current][y_current] == kSticky) {
    HitSticky(x_current, y_current);
    return true;
  } else if (course_map_[x_current][y_current] == kHole) {
    if (abs(ball_.GetVelocity().x) < 1) {
      game_won_ = true;
      return true;
    }
  }
  return false;
}

void GolfCourse::HitWall(unsigned x_pos, unsigned y_pos) {
  // Checking if there is a vertical line boundary
  if (course_map_[x_pos + 1][y_pos] == kWall ||
      course_map_[x_pos - 1][y_pos] == kWall) {
    ball_.SetVelocity(ball_.GetVelocity() * vec2(1, -kCollisionDampener));

    if (abs(ball_.GetVelocity().y) <= kLandBallSpeed) {
      LandBallOnGround(y_pos - ball_.GetRadius());
    }
  }
  // Checking for horizontal line boundary
  if (course_map_[x_pos][y_pos + 1] == kWall ||
      course_map_[x_pos][y_pos - 1] == kWall) {
    ball_.SetVelocity(ball_.GetVelocity() * vec2(-kCollisionDampener, 1));
  }
}

void GolfCourse::HitWater() {
  ball_.SetPosition(last_position_);
  ball_.SetVelocity(vec2(0, 0));
  gravity_present_ = false;
  moves_++;
}

void GolfCourse::HitSand(unsigned x_pos, unsigned y_pos) {
  ball_.SetPosition(vec2(x_pos, y_pos));
  ball_.SetVelocity(vec2(0, 0));
  on_sand_ = true;
  gravity_present_ = false;
}

void GolfCourse::HitSticky(unsigned x_pos, unsigned y_pos) {
  ball_.SetPosition(vec2(x_pos, y_pos));
  ball_.SetVelocity(vec2(0, 0));
  on_sticky_ = true;
  gravity_present_ = false;
}

void GolfCourse::LandBallOnGround(unsigned y_pos) {
  ball_.SetVelocity(vec2(ball_.GetVelocity().x, 0));
  ball_.SetPosition(vec2(ball_.GetPosition().x, y_pos));
  gravity_present_ = false;
}

bool GolfCourse::TurnOnGravity() {
  if (course_map_[ball_.GetPosition().x][ball_.GetPosition().y + ball_.GetRadius()] == kEmpty
      &&
      course_map_[ball_.GetPosition().x][ball_.GetPosition().y] == kEmpty) {
    gravity_present_ = true;
    return true;
  }
  return false;
}

void GolfCourse::UseGrassSpeed() {
  ball_.SetPosition(ball_.GetPosition() + kGrassDampener * ball_.GetVelocity());
  ball_.SetVelocity(vec2 (ball_.GetVelocity().x * kLossOfEnergy, 0));

  if (abs(ball_.GetVelocity().x) < kNotMovingPutSpeed) {
    ball_.SetVelocity(vec2(0,0));
  }
}

void GolfCourse::UseMulligan() {
  if (moves_ != 0 && CanHitBall() && power_ups_ > 0) {
    ball_.SetPosition(last_position_);
    moves_--;
    power_ups_--;
  }
}

void GolfCourse::UseAirBrakes() {
  if (length(ball_.GetVelocity()) > kNotMovingSpeed && power_ups_ > 0) {
    ball_.SetVelocity(vec2(0, 0));
    power_ups_--;
  }
}

void GolfCourse::UseExtraBounce() {
  if (length(ball_.GetVelocity()) > kNotMovingSpeed && power_ups_ > 0) {
    ball_.SetVelocity(ball_.GetVelocity() + vec2(0, kExtraBounceValue));
    power_ups_--;
  }
}

Ball GolfCourse::GetBall() const {
  return ball_;
}

bool GolfCourse::GetGameWon() const {
  return game_won_;
}

unsigned GolfCourse::GetMoves() const {
  return moves_;
}

unsigned GolfCourse::GetPowerUps() const {
  return power_ups_;
}

} // namespace golf
