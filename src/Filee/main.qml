import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4

Window {
    id: root
    width: 405
    height: 720
    visible: true
    title: qsTr("Filee")

    property Component transferPanelView: TransferPanelView {}
    property Component test: Tset {}
    property var componentMap: {
        "TransferPanelView": transferPanelView
    }
    property var componentMap1: {
            "Test": test
    }

    StackView {
        id: stackView
        anchors.fill: parent

        Rectangle {
            // The home page starts here
            anchors.fill: parent
            color: "lightblue"

            Rectangle {
                    id: userInfo
                    width: root.width
                    height: root.height*0.27
                    color: "#ffff7f"

                    anchors {
                        left: root.left
                        top: root.top
                    }

                    Rectangle {
                        id: userPic
                        z: 1
                        width: userInfo.height*0.5
                        height: userInfo.height*0.5
                        color: "#55aaff"

                        anchors {
                            left: userInfo.left
                            leftMargin: userInfo.width*0.10
                            top: userInfo.top
                            topMargin: userInfo.height*0.25
                        }

                        Text {
                            id: userShortName
                            width: userPic.width*0.6
                            height: userPic.width*0.6
                            text: qsTr("ST")
                            font.pixelSize: 36
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            font.family: "Verdana"

                            anchors {
                                left: userPic.left
                                leftMargin: userPic.width*0.2
                                top: userPic.top
                                topMargin: userPic.height*0.2
                            }
                        }
                    }

                    Text {
                        id: userName
                        z: 1
                        width: userPic.width*1.8
                        height: userPic.height*0.5
                        text: qsTr("I am Scared Termite")
                        font.pixelSize: 18
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        lineHeightMode: Text.FixedHeight
                        minimumPixelSize: 12
                        font.family: "Verdana"

                        anchors {
                            left: userPic.right
                            leftMargin: userPic.width*0.3
                            top: userPic.top
                        }

                    }

                    Text {
                        id: userStatus
                        z: 1
                        width: userPic.width*1.8
                        height: userPic.height*0.5
                        text: qsTr("Not connected To WI-FI")
                        font.pixelSize: 16
                        horizontalAlignment: Text.AlignLeft
                        verticalAlignment: Text.AlignVCenter
                        minimumPixelSize: 12
                        font.family: "Verdana"

                        anchors {
                            left: userPic.right
                            leftMargin: userPic.width*0.3
                            top: userName.bottom
                        }

                    }

                }

                Rectangle {
                    id: menuButton
                    width: root.width
                    height: root.height*0.06
                    color: "#aaff7f"

                    anchors {
                        left: root.left
                        top: userInfo.bottom
                    }

                }

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
