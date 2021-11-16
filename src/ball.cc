//
// Created by Joseph Kuang on 4/20/21.
//
#include "ball.h"

namespace golf {

Ball::Ball(const Color& color, float radius, const vec2& position, const vec2& velocity) {
  color_ = color;
  radius_ = radius;
  position_ = position;
  velocity_ = velocity;
}

void Ball::SetPosition(const vec2& position) {
  position_ = position;
}

void Ball::SetVelocity(const vec2& velocity) {
  velocity_ = velocity;
}

Color Ball::GetColor() const {
  return color_;
}

float Ball::GetRadius() const {
  return radius_;
}

vec2 Ball::GetPosition() const {
  return position_;
}

vec2 Ball::GetVelocity() const {
  return velocity_;
}

} // namespace golf