import QtQuick 1.1

Rectangle {
  width: 4096
  height: 4096
  id: mainBox
  objectName:"MainBox"
  color: "#ddee33"

  function scaleUpText (theFactor) {
    console.log ("scale " + mainText.scale + " by " + theFactor)
    mainText.scale = Math.min (mainText.maxScale, 
                       Math.max (mainText.minScale, mainText.scale * theFactor))
    console.log ("result " + mainText.scale)
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
    Rectangle {
      height:  200
      width: 400
      id: theButton
      objectName:"BigButton"
      color: Qt.darker (bottomHalf.color, 2.0)
      function handleLoopGesture () {
        mainTextBox.rotation += 15
        console.log (objectName + " loop gesture " + mainTextBox.rotation)
        return true;
      }
      anchors {
        left: listArea.right
        top:listArea.top
      }
      
      MouseArea {
        id:gestureTrap
        objectName:"GestureTrap"
        anchors.fill: parent
        onPressed: {
          console.log ("pressed button")
        }
        function handleLoopGesture () {
          mainTextBox.rotation += 15
          console.log (objectName + " loop gesture " + mainTextBox.rotation)
          return true;
        }
       
      }
      
    }
    
  }
  
}
