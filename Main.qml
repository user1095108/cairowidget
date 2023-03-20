import QtQml 2.15

import cairowidget 0.0

SampleItem {
  Timer {
    interval: 1000; running: true; repeat: true
    onTriggered: parent.update()
  }
}
