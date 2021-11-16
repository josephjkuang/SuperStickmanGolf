//
// Created by Joseph Kuang on 4/20/21.
//

#include "golf_simulation_app.h"
#define PI 3.14159265

namespace golf {
using ci::app::KeyEvent;
using ci::ImageSourceRef;
using ci::loadImage;
using ci::Rectf;
using ci::gl::drawString;
using ci::gl::drawStringCentered;
using ci::gl::rotate;
using ci::gl::Texture2d;
using ci::gl::translate;
using std::to_string;

GolfSimulationApp::GolfSimulationApp() {
  ci::app::setWindowSize(kWindowWidth, kWindowHeight);

  // Loading in Images
  ImageSourceRef background_img = loadImage(kBackgroundPath[0]);
  background_ = Texture2d::create(background_img);
  ImageSourceRef arrow_img = loadImage(kArrowPath);
  arrow_ = Texture2d::create(arrow_img);
  ImageSourceRef stickman_img = loadImage(kStickmanPath);
  stickman_ = Texture2d::create(stickman_img);

  // Loading in Course Map from a File
  CourseReader courseReader = CourseReader(kWindowWidth, kWindowHeight);
  std::ifstream input_file(kCoursePaths[0]);

  if (input_file.is_open()) {
    input_file >> courseReader;
    golfCourse_.SetCourseMap(courseReader.GetPixelTypes());
    input_file.close();
  } else {
    std::cerr << "Could not open input file" << std::endl;
  }
}

void GolfSimulationApp::draw() {
  ci::gl::draw(background_, Rectf(vec2(0, 0), vec2(kWindowWidth, kWindowHeight)));

  if (golfCourse_.GetGameWon()) {
    drawStringCentered("Congratulations, You Won The Game in "
                           + to_string(golfCourse_.GetMoves())
                           + " moves! Click r to try again",
                       vec2(kWindowWidth / 2, kParTextHeight), kFontColor, kFont);
  } else {
    DrawImages();
    golfCourse_.Display();
    DrawStringDirections();
  }
}

void GolfSimulationApp::update() {
  golfCourse_.AdvanceOneFrame();
}

void GolfSimulationApp::keyDown(KeyEvent event) {
  switch (event.getCode()) {
    case KeyEvent::KEY_RETURN:
      golfCourse_.HitBall(current_power_, current_angle_);
      break;

    case KeyEvent::KEY_1:
      current_power_ = 1;
      break;

    case KeyEvent::KEY_2:
      current_power_ = 2;
      break;

    case KeyEvent::KEY_3:
      current_power_ = 3;
      break;

    case KeyEvent::KEY_4:
      current_power_ = 4;
      break;

    case KeyEvent::KEY_5:
      current_power_ = 5;
      break;

    case KeyEvent::KEY_6:
      current_power_ = 6;
      break;

    case KeyEvent::KEY_7:
      current_power_ = 7;
      break;

    case KeyEvent::KEY_8:
      current_power_ = 8;
      break;

    case KeyEvent::KEY_9:
      current_power_ = 9;
      break;

    case KeyEvent::KEY_0:
      current_power_ = 10;
      break;

    case KeyEvent::KEY_a:
      current_angle_ = kNorthAngle;
      break;

    case KeyEvent::KEY_b:
      current_angle_ = kEastAngle;
      break;

    case KeyEvent::KEY_c:
      current_angle_ = kSouthAngle;
      break;

    case KeyEvent::KEY_d:
      current_angle_ = kWestAngle;
      break;

    case KeyEvent::KEY_e:
      current_angle_ = kNorthEastAngle;
      break;

    case KeyEvent::KEY_f:
      current_angle_ = kSouthEastAngle;
      break;

    case KeyEvent::KEY_g:
      current_angle_ = kSouthWestAngle;
      break;

    case KeyEvent::KEY_h:
      current_angle_ = kNorthWestAngle;
      break;

    case KeyEvent::KEY_LEFT:
      if (current_angle_ == kMaxAngle) {
        current_angle_ = 0;
      }
      current_angle_ = current_angle_ + kArrowAngleChange;
      break;

    case KeyEvent::KEY_RIGHT:
      if (current_angle_ == 0) {
        current_angle_ = kMaxAngle;
      }
      current_angle_ = current_angle_ - kArrowAngleChange;
      break;

    case KeyEvent::KEY_r:
      golfCourse_.ResetGame();
      break;

    case KeyEvent::KEY_PERIOD:
      golfCourse_.SwitchMode();
      break;

    case KeyEvent::KEY_BACKSPACE:
      golfCourse_.UseMulligan();
      break;

    case KeyEvent::KEY_UP:
      golfCourse_.UseExtraBounce();
      break;

    case KeyEvent::KEY_DOWN:
      golfCourse_.UseAirBrakes();
      break;
  }
}

void GolfSimulationApp::DrawImages() const {
  if (golfCourse_.CanHitBall()) {
    ci::gl::draw(stickman_, Rectf(golfCourse_.GetBall().GetPosition() + kStickmanDimensions,
                                  golfCourse_.GetBall().GetPosition() - kStickmanTranslation));

    // Putting the reference frame at the ball.
    translate(golfCourse_.GetBall().GetPosition().x, golfCourse_.GetBall().GetPosition().y);
    rotate(-PI * current_angle_ / kRadianAdjustment);
    ci::gl::draw(arrow_, Rectf(-vec2(kArrowPlacement, kArrowHeight),
                               vec2(kArrowWidth, kArrowHeight)));

    // Putting the reference frame back to normal.
    rotate(PI * current_angle_ / kRadianAdjustment);
    translate(-golfCourse_.GetBall().GetPosition().x, -golfCourse_.GetBall().GetPosition().y);
  }
}

void GolfSimulationApp::DrawStringDirections() const {
  drawString("PAR: 5", vec2(kParTextWidth, kParTextHeight), kFontColor, kFont);
  drawString("Moves: " + to_string(golfCourse_.GetMoves()),
             vec2(kParTextWidth, kMovesHeight), kFontColor, kFont);
  drawString("Power: " + to_string(current_power_),
             vec2(kParTextWidth, kPowerHeight), kFontColor, kFont);
  drawString("Power-Ups: " + to_string(golfCourse_.GetPowerUps()),
             vec2(kParTextWidth, kPowerUpsHeight), kFontColor, kFont);
}

} // namespace golf
