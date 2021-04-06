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
