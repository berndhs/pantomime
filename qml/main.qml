import QtQuick 1.1

Rectangle {
  width: 4096
  height: 4096
  id: mainBox
  color: "#ddee33"
  Text {
    id: mainText
    scale: 1
    property real maxScale: 5
    property real minScale: 0.3
    anchors.centerIn:  parent
    text: "Pantomime"
  }
  function scaleUpText (theFactor) {
    console.log ("scale " + mainText.scale + " by " + theFactor)
    mainText.scale = Math.min (mainText.maxScale, 
                       Math.max (mainText.minScale, mainText.scale * theFactor))
    console.log ("result " + mainText.scale)
  }

  PinchArea {
    anchors.fill: parent
    id: pinchControl
    onPinchStarted: {
      console.log ("started pinch ")
    }
    onPinchUpdated: {
      console.log ("updated " + pinch.scale)
      mainBox.scaleUpText (pinch.scale)
    }
    onPinchFinished: {
      console.log ("finished pinch ")
      console.log ("finale size: " + mainText.scale + " w " + mainText.width)
    }
  }

  MouseArea {
    anchors.fill: parent
    onDoubleClicked: {
      Qt.quit()
    }
  }
}
