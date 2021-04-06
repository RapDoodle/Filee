import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

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

            id:backbutton
            anchors.left: parent.left
            height: parent.height
            width: height
            color: "#FFAFFC"

            Text {
                text: "back"
                anchors.fill: parent
                font.family:  "Arial"
                font.styleName: "bold"
                font.pixelSize: 20
                minimumPixelSize: 8
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent;
                onClicked: {
                    if(stackView.depth > 0)
                        stackView.clear();
                }
            }
        }

        Rectangle {

            id:photo
            anchors.left: backbutton.right
            height: parent.height
            width: height
            color: "#ffffff"


            MouseArea {
                anchors.fill: parent;
            }
        }

        Rectangle {

            id:username
            anchors.left: photo.right
            height: parent.height
            width:view.width
            color: "#2e2f30"

            Text {
                text: "Foolish"
                anchors.fill: parent
                color: "white"
                font.family:  "Arial"
                font.styleName: "bold"
                font.pixelSize: 20
                minimumPixelSize: 8
                horizontalAlignment: Text.AlignLeft
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent;
            }
        }

        Rectangle {
            anchors.right: parent.right
            height: parent.height
            width: height
            color: "#FFAFFC"

            Text {
                text: "set"
                anchors.fill: parent
                font.family:  "Arial"
                font.styleName: "bold"
                font.pixelSize: 20
                minimumPixelSize: 8
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            MouseArea {
                anchors.fill: parent;
            }
        }
    }

    Rectangle {

        id:upload

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
            font.family:  "Arial"
            font.styleName: "bold"
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

    Rectangle {

        id: send

        anchors {
            left: parent.left
            bottom:upload.top
        }
        width: view.width
        height: view.height * 0.075
        color: "#fed500"

        Row{//the text input box

            spacing: 10
            anchors.centerIn: parent

            Rectangle{
                width: send.width
                height: send.height
                color:  "lightgrey"
                border.color: "grey"
                TextInput{
                    id:input
                    anchors.fill: parent
                    anchors.margins: 2
                    font.pointSize: 14

                    onEditingFinished:{
                        console.log(text)
                    }

                }

            }
        }

        Rectangle{
            id:sendbutton

            anchors.right: send.right
            height: send.height
            width: height
            color: "#4f4e4e"

            Text {
                text: "send"
                anchors.fill: parent
                font.family:  "Arial"
                font.styleName: "bold"
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
     }

    ReceiveBubble{
        id:receivebubble
    }
    SendBubble{
        id:sendbubble
        anchors.top: receivebubble.bottom
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.66;height:480;width:640}D{i:15}
}
##^##*/
