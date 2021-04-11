import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls 1.4


ApplicationWindow {
    id: root
    width: 405
    height: 720
    visible: true
    title: qsTr("Filee")
    minimumHeight: 720
    minimumWidth: 405


//    property Component transferPanelView: TransferPanelView {}
//    property var componentMap: {
//        "TransferPanelView": transferPanelView
//    }
    Component.onCompleted: {
        _guiController.exec()
        usertable.model = _onlineDevicesList
    }

    StackView {
        id: stackView
        anchors.fill: parent

        Rectangle {
            // The home page starts here
            anchors.fill: parent
            color: "#a5a5a5"

            Rectangle {
                id: userInfo
                width: root.width
                height: root.height*0.25
                color: "#525252"

                anchors {
                    left: root.left
                    top: root.top
                }

                Rectangle {
                    id: userPic
                    z: 1
                    width: userInfo.height*0.6
                    height: userInfo.height*0.6
                    color: "#cea392"

                    anchors {
                        left: userInfo.left
                        leftMargin: userInfo.width*0.10
                        top: userInfo.top
                        topMargin: userInfo.height*0.2
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
                            leftMargin: userPic.width*0.16
                            top: userPic.top
                            topMargin: userPic.height*0.2
                        }
                    }
                }

                Text {
                    id: userName
                    z: 1
                    width: userPic.width*1.8
                    height: userPic.height*0.4
                    text: qsTr("I am Scared Termite")
                    font.pixelSize: 18
                    color: "#f4eeeb"
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
                    height: userPic.height*0.3
                    text: qsTr("Not connected To WI-FI")
                    font.pixelSize: 16
                    color: "#d6845a"
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

                Text {
                    id: ipAddress
                    z: 1
                    width: userPic.width*1.8
                    height: userPic.height*0.3
                    text: qsTr("IP Address: 127.0.0.1")
                    font.pixelSize: 16
                    color: "#f4eeeb"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    minimumPixelSize: 12
                    font.family: "Verdana"

                    anchors {
                        left: userPic.right
                        leftMargin: userPic.width*0.3
                        top: userStatus.bottom
                    }

                }

            }

            Rectangle {
                id: menuButton
                width: root.width
                height: root.height*0.02
                color: "#282828"

                anchors {
                    left: root.left
                    top: userInfo.bottom
                }

            }

            TableView {
                id: usertable
                frameVisible: true
                width: parent.width
                height: parent.height

                anchors {
                    top: menuButton.bottom
                }
                backgroundVisible: false

                //anchors.fill: parent

                model: root._onlineDevicesList

                Component {
                    id: itemDelegateText
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        color: styleData.selected ? "#cb8862" : "#c5aba8"
                        elide: styleData.elideMode
                        text: styleData.value
                        font.family: "Verdana"
                        font.pointSize: 10
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                    }
                }

                TableViewColumn {
                    role: "nickname"
                    title:qsTr("Nickname")
                    width: 200
                    delegate: itemDelegateText
                }

                TableViewColumn {
                    role: "ip"
                    title:qsTr("Address")
                    width:100
                    delegate: itemDelegateText
                }

                TableViewColumn {
                    role: "status"
                    title:qsTr("Status")
                    width:100
                    delegate: itemDelegateText
                }

                headerDelegate: Rectangle {
                     height: 30
                     color: "#323232"
                     border.color: "#626262"
                     Text {
                        id: headerName
                        text: styleData.value
                        color: "lightgrey"
                        font.family: "Verdana"
                        font.pointSize: 10
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        anchors.fill: parent
                       }
                 }

                 rowDelegate: Rectangle {
                    id:rowRectangle

                    property color rowColor: styleData.selected?"#f1eeeb":(styleData.alternate ? "#525252":"#323232")
                    color:rowColor
                    height: 30

                  }
            }


            //swipe view starts here
            SwipeControl {
                id: swipeControl
                //height: parent.bottom-usertable.bottom
                //width: parent.width
                anchors{
                    //top: usertable.bottom
                    bottom: parent.bottom
                    //bottomMargin: 1
                }
                currentIndex: tabBar.currentIndex
            }
//                SwipeView{
//                    id:swipeView
//                    width: parent.width
//                    height: parent.height*0.3
//                    anchors{
//                        top: menuButton.bottom
//                    }
//                    currentIndex: tabBar.currentIndex
//                    SendPage{
//                        width: parent.width
//                    }
//                    ReceivePage{
//                        width: parent.width
//                    }
//                }
//                TabBar{
//                    id: tabBar
//                    width: parent.width
//                    anchors{
//                        top: swipeControl.bottom
//                    }
//                    currentIndex: swipeControl.currentIndex


//                    TabButton {
//                        text: qsTr("Page 1")
//                    }
//                    TabButton {
//                        text: qsTr("Page 2")
//                    }
//                }
            //swipe view ends here

        // The home page ends here
        }
    }
}
