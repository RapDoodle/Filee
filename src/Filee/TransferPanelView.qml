import QtQuick 2.0
import QtQuick.Controls 2.12

Rectangle {
    id: view
    color: "lightgreen"
    anchors.fill: parent

    Keys.onReleased: {
        if (event.key === Qt.Key_Back) {
            stackView.pop();
            event.accepted = true;
        }
    }

    Rectangle {
        id:navigationBar
        width: view.width
        height: root.height * 0.075
        color: "#A2D2FF"

        Rectangle {
            anchors.left: parent.left
            height: parent.height
            width: height
            color: "#FFAFFC"

            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    if(stackView.depth > 0)
                        stackView.clear();
                }
            }
        }
    }

    Rectangle {
        anchors {
            left: parent.left
            bottom: parent.bottom
        }
        width: view.width
        height: view.height * 0.075
        color: "#4CC9F0"

        Text {
            text: "Upload file"
            anchors.fill: parent
            font.pixelSize: 20
            minimumPixelSize: 8
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        MouseArea {
            anchors.fill: parent;
            onClicked: {
                console.log("Clicked")
            }
        }
    }
//////////////////////////////////////////////////////////////

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
            text: "\u23F8"
            //text: U+2713


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
            text: "\u2715"
            anchors{
                top: receivePause.bottom
                topMargin: 1
                left: receiveBubbleImage.right
                leftMargin: 5
            }

        }
    }

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

}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.66;height:480;width:640}D{i:15}
}
##^##*/
