import QtQuick 2.0
import QtQuick.Controls 2.12



Rectangle {
    id: receiveBubble
    anchors{
        left: parent.left
        top: navigationBar.bottom
        topMargin: 10
    }
    width: parent.width
    height: receiveBubbleImage.height+10
    color: "#90ee90"

    BorderImage {
        id: receiveBubbleImage
        anchors{
            left: parent.left

        }


        width: 100
        height: 30
        source: "pic/messageBubble3(1).png"
        antialiasing: true
    }
    ProgressBar {
        id: receiveProgressBar
        width: parent.width*0.32
        height: 12
        enabled: true
        indeterminate: false
        font.pointSize: 9
        antialiasing: true

        ColorAnimation {
            from: "white"
            to: "#efe5b0"
            duration: 200
        }
        anchors{
            top: receiveBubbleImage.bottom
            topMargin: 5
            left: parent.left
        }
        value: 0.3
    }
    Text {
        id: receiveProgressPencentage
        text: qsTr(receiveProgressBar.value*100+"%")
        anchors{
            left: receiveProgressBar.right
            top: receiveProgressBar.top
            leftMargin: 2

        }
    }
    RoundButton{
        id:receivePause
        height:15
        width:height
        radius: height
        text: "\u003D"
        //text: U+2713
        font.pointSize: 15
        rotation: 90
        anchors{
            top: parent.top
            left: receiveBubbleImage.right
            leftMargin: 5
        }
    }
    RoundButton{
        id:receiveCancel

        height:15
        width:height
        radius: height
        text: "\u00D7"
        anchors{
            top: receivePause.bottom
            topMargin: 1
            left: receiveBubbleImage.right
            leftMargin: 5
        }

    }
}


