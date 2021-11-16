//
// Created by Joseph Kuang on 4/20/21.
// GolfSimulationApp is responsible for providing the visual for the game
//
#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "course_reader.h"
#include "golf_course.h"

namespace golf {
using ci::Color;
using ci::Font;
using ci::gl::Texture2dRef;

using std::array;
using std::string;

class GolfSimulationApp : public ci::app::App {
  public:
   /**
    * Default Constructor that sets the window size, loads the images, and sets the boundaries.
    */
    GolfSimulationApp();

    void draw() override;
    void update() override;
    void keyDown(ci::app::KeyEvent event) override;

  private:
    static const unsigned kWindowWidth = 1400;
    static const unsigned kWindowHeight = 700;

    static const unsigned kParTextHeight = 15;
    static const unsigned kParTextWidth = 15;
    static const unsigned kMovesHeight = 55;
    static const unsigned kPowerUpsHeight = 95;
    static const unsigned kPowerHeight = 135;

    static const unsigned kArrowWidth = 50;
    static const unsigned kArrowHeight = 10;
    static const int kArrowPlacement = -10;

    static const unsigned kRadianAdjustment = 180;
    static const unsigned kArrowAngleChange = 5;
    static const unsigned kMaxAngle = 360;

    // Decided to have these constants for quick adjustments to angle.
    static const unsigned kEastAngle = 0;
    static const unsigned kNorthAngle = 90;
    static const unsigned kWestAngle = 180;
    static const unsigned kSouthAngle = 270;
    static const unsigned kNorthEastAngle = 45;
    static const unsigned kNorthWestAngle = 135;
    static const unsigned kSouthWestAngle = 225;
    static const unsigned kSouthEastAngle = 315;

    const Font kFont = Font("Arial-Black", 25);
    const Color kFontColor = Color::hex(0x212bb8);

    // Storing course paths in an array in case I want to add future courses in the future.
    const array<const string, 1> kBackgroundPath = {"../../../../../../static/img/background.png"};
    const array<const string, 1> kCoursePaths = {"../../../../../../static/courses/course.txt"};

    const string kArrowPath = "../../../../../../static/img/arrow.png";
    const string kStickmanPath = "../../../../../../static/img/stickman.png";

    const vec2 kStickmanDimensions = vec2(20, -60);
    const vec2 kStickmanTranslation = vec2(14, 4);

    Texture2dRef background_;
    Texture2dRef arrow_;
    Texture2dRef stickman_;

    unsigned current_power_ = 10;
    unsigned current_angle_ = 45;

    GolfCourse golfCourse_ = GolfCourse(Color("white"), 2, vec2(100, 100), vec2(0, 0));

    /**
     * Helper Method For Draw that draws the arrow and stickman.
     */
    void DrawImages() const;

    /**
     * Helper Method that Draws the String Directions in Top Left of Screen
     */
    void DrawStringDirections() const;
};

} // namespace golf
