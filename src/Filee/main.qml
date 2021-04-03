import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4

Window {
    id: root
    width: 320
    height: 568
    visible: true
    title: qsTr("Filee")

    property Component transferPanelView: TransferPanelView {}

    property var componentMap: {
        "TransferPanelView": transferPanelView
    }

    StackView {
        id: stackView
        anchors.fill: parent

        Rectangle {
            // The home page starts here
            anchors.fill: parent
            color: "lightblue"

            Text {
                text: "Click me to go to the next page";
                font.pointSize: 14;
                font.bold: true;
                color: "#2D6A4F";
                anchors.centerIn: parent;

                MouseArea {
                    anchors.fill: parent;
                    onClicked: {
                        if (stackView.depth == 0) {
                            // Only push the control view if we haven't pushed it
                            stackView.push(transferPanelView);
                            stackView.currentItem.forceActiveFocus();
                        }
                    }
                }
            }

            // The home page ends here
        }
    }

}
