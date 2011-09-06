import QtQuick 1.1
import moui.geuzen.utils.static 1.0

Rectangle {
  width: 4096
  height: 4096
  id: mainBox
  objectName:"MainBox"
  color: "#ddee33"
  rotation: 0
  x: isPortrait ? (isInverted ? (width - height) * 0.5 : (height - width) * 0.5) : 0
  property bool isPortrait: false
  property bool isInverted: false
  property real rowHeight: 32
  property real mainWidth: isPortrait ? height : width
  property real mainHeight: isPortrait ? width : height
  property real orientMargin: (isPortrait ? Math.abs (width - height) * 0.5 : 0)
  
  GeuzenOrientation {
    id: orientationWatcher
    onRotationChange: {
      mainBox.isPortrait = portrait
      mainBox.rotation = rotation
      mainBox.isInverted = inverted
      console.log ("orientation port " + mainBox.isPortrait)
      console.log ("main box x " + mainBox.x + " y " + mainBox.y)
      console.log ("rect size: " + mainBox.width + ", " + mainBox.height)
      console.log ("my size  : " + mainWidth + ", " + mainHeight)
    }
  }
  
  function scaleUpText (theFactor) {
    console.log ("scale " + mainText.scale + " by " + theFactor)
    mainText.scale = Math.min (mainText.maxScale, 
                       Math.max (mainText.minScale, mainText.scale * theFactor))
    console.log ("result " + mainText.scale)
  }
  
  function rotateText (theAngle) {
    mainTextBox.rotation += theAngle
    if (mainTextBox.rotation < 0) {
      mainTextBox.rotation += 360
    } else if (mainTextBox.rotation > 360) {
      mainTextBox.rotation -= 360
    }
  }


  PinchArea {
    anchors.fill: bottomHalf
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
  
  
  Rectangle {
    id: topHalf
    objectName: "TopHalf"
    width: parent.width
    height:  0.5*parent.height
    anchors {
      top: parent.top
      horizontalCenter: parent.horizontalCenter
    }
    color: "transparent"
    Rectangle {
      id:quitButton
      width: 80
      height: 80
      z:10
      anchors { top: topHalf.top; right: topHalf.right; rightMargin: mainBox.orientMargin }
      color: "red"
      Text {
        anchors.centerIn: parent
        text: "Quit"
      }
      MouseArea {
        anchors.fill: parent
        onClicked: {
          Qt.quit()
        }
      }
    }
    Rectangle {
      id:mainTextBox
      width: 400
      height:240
      rotation: 0
      color:"transparent"
      anchors { verticalCenter: topHalf.verticalCenter; horizontalCenter: topHalf.horizontalCenter }
      Text {
        id: mainText
        scale: 1
        property real maxScale: 5
        property real minScale: 0.3
        anchors.centerIn:  parent
        text: "Pantomime"
      }
    }
  }

  
  Rectangle {  
    id:bottomHalf
    width: parent.width
    height: 0.5*parent.height
    objectName:"BottomHalf"
    color:Qt.lighter(mainBox.color)
    anchors {
      top: topHalf.bottom
      horizontalCenter: parent.horizontalCenter
    }
    
    VisualItemModel {
      id: itemModel
      Rectangle { height: 50; width: 80; color: "red" }
      Rectangle { height: 50; width: 80; color: "green" }
      Rectangle { height: 50; width: 80; color: "blue" }
      Rectangle { height: 50; width: 80; color: "#00dddd" }
      Rectangle { height: 50; width: 80; color: "#dddd00" }
      Rectangle { height: 50; width: 80; color: "red" }
      Rectangle { height: 50; width: 80; color: "green" }
    }
    ListView {
      height: 200
      width: 80
      model: itemModel
      clip: true
      id:listArea
      anchors {
        top: parent.top
        horizontalCenter: parent.horizontalCenter
      }
    }
    GeuzenLoopArea {
      id: turnRight
      anchors.fill: rightButton
      onLooped: {
        console.log ("loop detected right")
        mainBox.rotateText (15)
      }
    }   
    Rectangle {
      height:  200
      width: bottomHalf.width * 0.4
      id: rightButton
      color: Qt.darker (bottomHalf.color, 2.0)
      anchors {
        left: listArea.right
        top:listArea.top
      }
      
    }
    Rectangle {
      height:  200
      width: bottomHalf.width * 0.4
      id: leftButton
      color: Qt.lighter (bottomHalf.color, 2.0)
      anchors {
        right: listArea.left
        top:listArea.top
      }
      
    }
    GeuzenLoopArea {
      id: turnLeft
      anchors.fill: leftButton
      onLooped: {
        console.log ("loop detected left")
        mainBox.rotateText (-15)
      }
    }   
  }
  Component.onCompleted: {
    console.log ("load completed, calling start routines")
    orientationWatcher.start ()
  }
}
