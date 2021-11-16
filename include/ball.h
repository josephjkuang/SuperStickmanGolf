//
// Created by Joseph Kuang on 4/20/21.
// Class for the golf ball that is used inside the golf course.
//
#pragma once

#include "cinder/gl/gl.h"

namespace golf {
using ci::Color;
using glm::vec2;

class Ball {
 public:
  /**
   * Initialization Constructor
   * @param color color of ball
   * @param mass mass of ball
   * @param radius radius of ball
   * @param position position of ball
   * @param velocity velocity of ball
   */
  Ball(const Color& color, float radius, const vec2& position, const vec2& velocity);

  // Default Constructor for Declarations
  Ball() = default;

  void SetPosition(const vec2& position);

  void SetVelocity(const vec2& velocity);

  Color GetColor() const;

  float GetRadius() const;

  vec2 GetPosition() const;

  vec2 GetVelocity() const;

 private:
  Color color_;
  float radius_;
  vec2 position_;
  vec2 velocity_;
};

} // namespace golf

