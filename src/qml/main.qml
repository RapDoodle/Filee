import QtQuick 2.12
import QtQuick.Window 2.12

import QtQuick.Controls 1.4
import QtQuick.Controls 2.5


ApplicationWindow {
    id: root
    width: 460
    height: 720
    visible: true
    title: qsTr("Filee")
    minimumHeight: 720
    minimumWidth: 405

    property bool sendModify: true
    property bool receiveModify: false
    property double sendProgress: 0.0
    property double receiveProgress: 0.0
    property int swipePageNum: 0
    property bool speedModify: false
    property bool pauseAndCancelVisibility: false
    property color checkedColor: "#525252"//"#cea392"//""#0ACF97"

    // Interfaces

    // Hide the send button
    function sendHide(){
        //sendVisibility.visible = false
        sendModify = false
    }
    // Show the send button
    function sendShow() {
        sendModify = true
    }

    // Hide the two buttons in the receiver page
    function receiveHide() {
        receiveModify = false
    }

    // Show the two buttons in the receiver page
    function receiveShow() {
        receiveModify = true
    }

    // Sender's progress update (between 0 to 10000)
    function sendProgressModify(progress) {
        sendProgress = 0.0001*progress
    }

    // Receiver's progress update (between 0 to 10000)
    function receiveProgressModify(progress) {
        receiveProgress =  0.0001*progress
    }

    // Switch to sender's page
    function swipePageNumTo1() {
        swipePageNum = 0
    }

    // Switch to receiver's page
    function swipePageNumTo2() {
        swipePageNum = 1
    }

    //change here-------------------------------------------
    function pauseAndCancelShow(){
        pauseAndCancelVisibility = true
    }
    function pauseAndCancelHide(){
        pauseAndCancelVisibility = false
    }
    //------------------------------------------------------


    Component.onCompleted: {
        _guiController.exec()
        usertable.model = _onlineDevicesList
    }

    // For sender
    Connections {
        target: _guiController
        onSenderBegin: {
            sendProgressModify(0)
            sendHide()
            speedModify = true

        }
        onSenderEnded: {
            sendShow()
            speedModify = false
        }
        onSenderStatusUpdate: sendProgressModify(status)
    }

    // For receiver
    Connections {
        target: _guiController
        onReceiverBegin: {
            receiveHide();
            swipePageNumTo2();
            speedModify = true
        }
        onReceiverEnded: {
            receiveShow();
            speedModify = false
        }
        onReceiverTerminated: {
            sendProgressModify(0);
            swipePageNumTo1();
            speedModify = false
        }
        onReceiverStatusUpdate: receiveProgressModify(status)
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
                height: root.height*0.30
                color: "#525252"

                anchors {
                    left: root.left
                    top: root.top
                }

                Rectangle {
                    id: speedField
                    width: parent.width
                    height: 0.20*parent.height
                    color: "#282828"
                    anchors{
                        top: userInfo.top
                    }
                    Text {
                        id: speedText
                        anchors {
                            centerIn:  speedField

                            //verticalCenter: speedField.verticalCenter
                        }
                        color: "white"
                        font {
                            pointSize: 15
                            family: "Verdana"
                        }
                        Connections {
                            target: _guiController
                            onRateUpdate: speedText.text = qsTr("Speed: ") + rate
                        }
                        visible: speedModify
                    }
                }

                Rectangle {
                    id: userPic
                    z: 1
                    width: userInfo.height*0.50
                    height: userInfo.height*0.50
                    radius: 10
                    color: "#cea392"

                    anchors {
                        left: userInfo.left
                        leftMargin: userInfo.width*0.08
                        top: userInfo.top
                        topMargin: userInfo.height*0.36
                    }

                    Text {
                        id: userShortName
                        width: userPic.width*0.6
                        height: userPic.width*0.6
                        text: qsTr(_nickname.substring(0, 2).toUpperCase())
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
                    height: userPic.height*0.4
                    text: qsTr("I am " + _nickname)
                    font.pixelSize: 22
                    color: "#f4eeeb"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    lineHeightMode: Text.FixedHeight
                    minimumPixelSize: 12
                    font.family: "Verdana"

                    anchors {
                        left: userPic.right
                        leftMargin: userPic.width*0.2
                        bottom: userStatus.top
                    }

                }

                Text {
                    id: userStatus
                    z: 1
                    width: userPic.width*1.8
                    height: userPic.height*0.4
                    text: qsTr("Service is running")
                    font.pixelSize: 18
                    color: "#01b636"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    minimumPixelSize: 12
                    font.family: "Verdana"

                    anchors {
                        left: userPic.right
                        leftMargin: userPic.width*0.2
                        bottom: ipAddress.top
                    }

                }

                Text {
                    id: ipAddress
                    z: 1
                    width: userPic.width*1.8
                    height: userPic.height*0.3
                    text: "No valid interface."
                    font.pixelSize: 18
                    color: "#f4eeeb"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    minimumPixelSize: 12
                    font.family: "Verdana"

                    property int ipIndex: 0
                    Component.onCompleted: {
                        ipAddress.showIp()
                    }

                    function showIp() {
                        if (_guiController.qmlLocalIps.length > 0) {
                            if (_guiController.qmlLocalIps.length === 1) {
                                ipAddress.text = qsTr(_guiController.qmlLocalIps[0]);
                            } else {
                                ipAddress.text = qsTr(_guiController.qmlLocalIps[ipIndex] +
                                        " (" +  (_guiController.qmlLocalIps.length - 1) + " more)");
                            }
                        } else {
                            ipAddress.text = qsTr("No valid interface.");
                        }
                    }

                    anchors {
                        left: userPic.right
                        leftMargin: userPic.width*0.2
                        bottom: userPic.bottom
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            ipAddress.ipIndex++
                            ipAddress.ipIndex %= _guiController.qmlLocalIps.length
                            ipAddress.showIp()
                        }
                    }
                }
            }

            Rectangle {
                id: menuButton
                width: root.width
                height: root.height*0.05*1.5
                color: "#282828"

                anchors {
                    left: root.left
                    top: userInfo.bottom
                }
                Switch {
                    id: boardcastSwitch
                    x: (root.width-width)/2+15
                    checked: true
                    anchors{
                        //right: menuButton.right
                        //horizontalCenter: menuButton.horizontalCenter
                        //leftMargin: 1
                        top: menuButton.top
                        //verticalCenter: menuButton.verticalCenter
                        topMargin:  0.1*menuButton.height
                        //verticalCenterOffset: -5
                    }
                    //property color checkedColor: "#0ACF97"

                    onCheckedChanged: {
                        if (checked)
                            _guiController.startBroadcast();
                        else
                            _guiController.stopBroadcast();
                    }
                    indicator: Rectangle {
                        width: 2*height
                        height: 0.7*menuButton.height/2
                        radius: height / 2
                        color: boardcastSwitch.checked ? checkedColor : "white"
                        border.width: 2
                        border.color: boardcastSwitch.checked ? checkedColor : "white"

                        Rectangle {
                            x: boardcastSwitch.checked ? parent.width - width - 2 : 1
                            width: boardcastSwitch.checked ? parent.height - 4 : parent.height - 2
                            height: width
                            radius: width / 2
                            anchors.verticalCenter: parent.verticalCenter
                            color: "white"
                            border.color: "#D5D5D5"

                            Behavior on x {
                                NumberAnimation { duration: 200 }
                            }
                        }
                    }
                }
                Text {
                    id: boardcastText
                    text: qsTr("Turn on/off boardcast")
                    anchors{
                        top: boardcastSwitch.bottom
                        //topMargin: 1
                        horizontalCenter: boardcastSwitch.horizontalCenter
                    }
                    font.pixelSize: 14
                    color: "#f4eeeb"
                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter
                    minimumPixelSize: 12
                    font.family: "Verdana"
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

                onClicked: _guiController.setReceiverIpAddress(currentRow)

                model: _onlineDevicesList

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
                    title: qsTr("Nickname")
                    width: usertable.width * 0.4
                    delegate: itemDelegateText
                }

                TableViewColumn {
                    role: "ip"
                    title: qsTr("Address")
                    width: usertable.width * 0.4
                    delegate: itemDelegateText
                }

                TableViewColumn {
                    role: "status"
                    title: qsTr("Status")
                    width: usertable.width * 0.2
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
                currentIndex: swipePageNum
            }
        // The home page ends here
        }
    }
}
