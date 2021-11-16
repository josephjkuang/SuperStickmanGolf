//
// Created by Joseph Kuang on 4/20/21.
// Main File for Project to Run Cinder Application
//
#include "golf_simulation_app.h"

using golf::GolfSimulationApp;

void prepareSettings(GolfSimulationApp::Settings* settings) {
  settings->setResizable(false);
}

// This line is a macro that expands into an "int main()" function.
CINDER_APP(GolfSimulationApp, ci::app::RendererGl, prepareSettings);