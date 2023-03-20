import QtQuick
import QtQuick.Controls

import cairowidget

ApplicationWindow {
  width: 700
  height: 700

  visible: true

  SampleItem {
    anchors.fill: parent

    Timer {
      interval: 1000; running: true; repeat: true
      onTriggered: parent.update()
    }
  }
}
