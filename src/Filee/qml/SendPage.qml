import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.2
//import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.0
import QtGraphicalEffects 1.12

Page{
    Rectangle{
        color: "#525252"
        anchors.fill: parent

        FileDialog {
            id: fileDialog
            title: "Please choose a file"
            folder: shortcuts.home
            onAccepted: {
                // Fix for: file name containing file:/// is not readable on Windows
                var path = fileDialog.fileUrl.toString()
                path = path.replace(/^(file:\/{3})/, '');
                _guiController.setSenderFilePath(path)
                fileNameLabel.text = _guiController.qmlSenderFileName
            }
        }

        Rectangle{
            id:ipField
            width: parent.width-40
            color: parent.color
            anchors{
                top: parent.top
                topMargin: parent.height*0.1
                leftMargin: parent.width*0.01
                rightMargin: leftMargin
                horizontalCenter: parent.horizontalCenter
            }
            TextEdit {
                id: ipText
                width: parent.width*0.3
                anchors{
                    //horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                font{
                    pointSize: 15
                    family: "Verdana"
                }
                color: "#f4eeeb"
                text: "Receiver    "
                persistentSelection: false
                selectByMouse: true
                readOnly: true
                //wrapMode: "WordWrap"
            }
            TextEdit {
                id: ipTextInfo
                //width: parent.width*0.7
                anchors {
                    //horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                    left: ipText.right
                }
                font {
                    pointSize: 15
                    family: "Verdana"
                }
                color: "#f4eeeb"
                text: _guiController.qmlReceiverIp.length > 0 ? _guiController.qmlReceiverIp : "Select a receiver..."
                persistentSelection: false
                selectByMouse: true
                onTextChanged: _guiController.qmlReceiverIp = text
            }
        }
        Rectangle{
            id: fileNamePlace
            width: parent.width-40
            height: fileNameLabel.height+10
            color: parent.color
            anchors{
                top: ipField.bottom
                topMargin: parent.height*0.1
                leftMargin: parent.width*0.01
                rightMargin: leftMargin
                horizontalCenter: parent.horizontalCenter
            }
            TextEdit {
                id: fileName
                width: fileNamePlace.width*0.3
                anchors{
                    //horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                font{
                    pointSize: 15
                    family: "Verdana"
                }
                color: "#f4eeeb"
                text: "File Name   "
                persistentSelection: false
                selectByMouse: true
                readOnly: true
                //wrapMode: "WordWrap"
            }
            TextEdit {
                id: fileNameLabel
                width: fileNamePlace.width-selectButton.width-fileName.width
                anchors{
                    //horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                    left: fileName.right
                }
                font{
                    pointSize: 15
                    family: "Verdana"
                }
                color: "#f4eeeb"
                text: _guiController.qmlSenderFileName/*"asijdoihfgoisjodfijhowihjefo;iwehjejf;oiaweo;fijh;oiehfo"*/
                persistentSelection: false
                selectByMouse: true
                readOnly: true
                wrapMode: Text.WrapAnywhere

                //                function textwidth(){

//                }
            }
            RoundButton {
                id: selectButton
                width: parent.width*0.2
                height: fileName.height
                anchors{
                    leftMargin: 10
                    verticalCenter: parent.verticalCenter
                    right: parent.right

                }
                text: "Select"

                font{
                    pointSize: 13
                    family: "Verdana"
                }
                palette.button: "#323232"// change button color
                palette.buttonText: "#cea392"
                onClicked: fileDialog.open()
            }
        }
        Rectangle{
            id:buttonArea
            width:parent.width-40
            height:50
            color: parent.color
            anchors{
                //top: progressField.bottom
                top: fileNamePlace.bottom
                //bottom: parent.bottom
                topMargin: 32
                horizontalCenter: parent.horizontalCenter
            }
            RoundButton {
                id: receivePause
                height:parent.height
                width:height
                radius: height
                text: "\u003D"
                //text: U+2713
                font.pointSize: 30
                rotation: 90
                anchors{
                    top: parent.top
                    left: parent.left
                    leftMargin: 0.2*parent.width
                }
                onClicked: {
                    if (receivePause.text==="\u003D") {
                        _guiController.senderPause()
                        receivePause.text = "\u25B2"
                        receivePause.font.pointSize = 15
                        animationTimer.stop()
                    } else {
                        _guiController.senderResume()
                        receivePause.text = "\u003D"
                        receivePause.rotation = 90
                        receivePause.font.pointSize = 30
                        animationTimer.restart()
                    }
                }
                palette.button: "#414141"
                palette.buttonText: "#cea392"
            }
            RoundButton{
                id:receiveCancel

                height:parent.height
                width:height
                radius: height
                text: "\u00D7"
                font.pointSize: 20
                anchors{
                    right: parent.right
                    topMargin: 1

                    rightMargin: 0.2*parent.width
                }
                palette.button: "#414141"
                palette.buttonText: "#cea392"
                onClicked: _guiController.senderCancel()
            }
        }

        Rectangle{
            id:sendField
            width: parent.width
            height:0.18*parent.height
            anchors{
                bottom: parent.bottom
            }
            Rectangle{
                id: progressField
                width: parent.width
                height: parent.height
                anchors{
                    bottom: parent.bottom
                    //topMargin: 35
                    horizontalCenter: parent.horizontalCenter
                }
            //var progressValue = 0
                Rectangle{
                    id:progressBackground
                    width: parent.width
                    height: parent.height
                    color: "#a5a5a5"
                    //radius: 4
                }
                Rectangle{
                    id:progress
                    width: parent.width*sendProgress
                    height: parent.height
                    color: "#323232"
                    //radius: 4
                    //ColorAnimation on color { to: "yellow";duration:1000}
                    onWidthChanged: {
                        if(progress.width==0){
                            animation.width=0
                        }
                    }
                }
                Rectangle{
                    id:animation
                    //radius: 4
                    x:parent.x
                    LinearGradient{
                        anchors.fill: parent
                        start: Qt.point(0,0)
                        end: Qt.point(width, 0)         // 1. Horizontal transition
                        //end: Qt.point(0, height)      // 2. Vertical transition
                        //end: Qt.point(width, height)  // 3. Oblique transition
                        gradient: Gradient {
                            GradientStop {  position: 0.0;    color: "#323232" }
                            GradientStop {  position: 0.5;    color: "#575757" }
                            GradientStop {  position: 1.0;    color: "#323232" }
                        }
                    }

                    height:progress.height
                    //color: "#d6845a"
                    anchors{
                        top: progress.top
                        //left: progress.left
                    }
                    width:0
                    Timer{
                        id:animationTimer
                        interval: 1000/progress.width
                        running: true
                        repeat: true
                        onTriggered:{
                            if(animation.x+animation.width>=progress.x+progress.width){
                                if(animation.width>0){
                                    animation.width--
                                    animation.x += 1
                                }else{
                                    animation.x = progress.x
                                }
                            }else{
                                if(animation.width<0.5*progress.width){
                                    animation.width++
                                }else{
                                    animation.x += 1
                                    //interval = interval-1
                                }
                            }
                        }
                    }

                }

                Slider {
                    id: slider
                    width: progressBackground.width
                    anchors{
                        bottom: progress.top
                    }
                    value: 0.5
                    visible: false
                }
                Button{
                    id:send
                    anchors{
                        bottom:parent.bottom
                    }
                    width: parent.width
                    height: parent.height
                    text: "SEND"
                    font{
                        pointSize: 15
                        family: "Verdana"
                    }
                    palette.button: "#323232"
                    palette.buttonText: "#cea392"
                    visible: sendModify

                    onClicked: {
                        _guiController.senderSend();
                        //send.visible = false
                    }
//                    background: {
//                        opacity: 0//background transparent
                    //                    }
                }
            }

        }
    }
}


/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
