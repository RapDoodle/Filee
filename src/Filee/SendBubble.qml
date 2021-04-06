import QtQuick 2.0
import QtQuick.Controls 2.12




Rectangle {
    id: sendBubble
    anchors{
        right: parent.right
        top: receiveBubble.bottom
        topMargin: 10
    }
    width: parent.width
    height: sendBubbleImage.height+10
    visible: true
    color: "#90ee90"

    BorderImage {
        id: sendBubbleImage
        anchors{
            right: parent.right

        }
        width: 100
        height: 30
        source: "pic/messageBubble3(2).png"
        antialiasing: true
    }
    ProgressBar {
        id: sendProgressBar
        width: parent.width*0.32
        height: 12
        enabled: true
        indeterminate: false
        font.pointSize: 9
        antialiasing: true

        ColorAnimation {
            from: "white"
            to: "#99d8ea"
            duration: 200
        }
        anchors{
            right: sendProgressPencentage.left
            top: sendBubbleImage.bottom
            topMargin: 5
            rightMargin: 2
        }
        value: 0.75
    }
    Text {
        id: sendProgressPencentage
        text: qsTr(sendProgressBar.value*100+"%")
        anchors{
            right: parent.right
            top: sendProgressBar.top
            //leftMargin: 2

        }
    }
    RoundButton{
        id:sendPause
        height:15
        width:height
        radius: height
        text: "\u23F8"
        //text: U+2713


        anchors{
            top: parent.top
            right: sendBubbleImage.left
            rightMargin: 5
        }
    }
    RoundButton{
        id:sendCancel
        text: "\u2715"
        height:15
        width:height
        radius: height
        anchors{
            top: sendPause.bottom
            topMargin: 1
            right: sendBubbleImage.left
            rightMargin: 5
        }

    }

}
