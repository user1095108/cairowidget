import QtQuick
import QtQuick.Controls

import cairowidget

SampleItem {
  Timer {
    interval: 1000; running: true; repeat: true
    onTriggered: parent.update()
  }
}
