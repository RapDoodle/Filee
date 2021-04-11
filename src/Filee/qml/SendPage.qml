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
                console.log("You chose: " + fileDialog.fileUrls)
                _guiController.setSenderFilePath(fileDialog.fileUrls)
                fileNameLabel.text = _guiController.qmlSenderFileName
            }
        }

        Rectangle{
            id:fileNamePlace
            width: parent.width-40
            height: fileNameLabel.height+10
            color: parent.color
            anchors{
                top: parent.top
                topMargin: parent.height*0.1
                leftMargin: parent.width*0.01
                rightMargin: leftMargin
                horizontalCenter: parent.horizontalCenter
            }
            TextEdit {
                id: fileNameLabel
                width: parent.width-selectButton.width*2
                anchors{
                    //horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                font{
                    pointSize: 15
                    family: "Verdana"
                }
                color: "#f4eeeb"
                text: _guiController.qmlSenderFileName
                persistentSelection: false
                selectByMouse: true
                readOnly: true
                //wrapMode: "WordWrap"
                function textwidth(){

                }
            }
            RoundButton {
                id: selectButton
                width: parent.width*0.2
                height: parent.height
                anchors{
                    leftMargin: 10

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
            id: progressField
            width: parent.width-40
            anchors{
                top: fileNamePlace.bottom

                topMargin: 35
                horizontalCenter: parent.horizontalCenter
            }
//            ProgressBar {
//                id: progressBar
//                width: parent.width
//                height: 24
//                anchors{
//                    //top: fileNamePlace.bottom
//                }
//                antialiasing: true
//                //hoverEnabled: true
//                indeterminate: false
//                //value: 0.5
//                value:slider.value

////                ProgressBarStyle{
////                    background: Rectangle{
////                        radius: 2
////                        color: "lightgray"
////                        border.color: "gray"
////                        border.width: 1
////                        implicitWidth: 200
////                        implicitHeight: 24
////                    }
////                    progress: Rectangle {
////                        color: "lightsteelblue"
////                        border.color: "steelblue"
////                    }
////                }
        //var progressValue = 0
            Rectangle{
                id:progressBackground
                width: parent.width
                height: 20
                color: "lightgrey"
                radius: 4
            }
            Rectangle{
                id:progress
                width: parent.width*slider.value
                height: 20
                color: "#d6845a"
                radius: 4
                //ColorAnimation on color { to: "yellow";duration:1000}
                onWidthChanged: {
                    if(progress.width==0){
                        animation.width=0
                    }
                }
            }
            Rectangle{
                id:animation
                radius: 4
                x:parent.x+4
                LinearGradient{
                    anchors.fill: parent
                    start: Qt.point(0,0)
                    end: Qt.point(width, 0)      ///1.横向渐变
                    //end: Qt.point(0, height)     ///2.竖向渐变
                    //end: Qt.point(width, height) ///3.斜向渐变
                    gradient: Gradient {
                        GradientStop {  position: 0.0;    color: "#d6845a" }
                        GradientStop {  position: 0.5;    color: "#e0ae94" }
                        GradientStop {  position: 1.0;    color: "#d6845a" }
                    }
                }

                height:progress.height
                color: "#d6845a"
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
                        if(animation.x+animation.width>=progress.x+progress.width-4){
                            if(animation.width>0){
                                animation.width--
                                animation.x += 1
                            }else{
                                animation.x = progress.x+4
                            }
                        }else{
                            if(animation.width<0.5*progress.width){
                                animation.width++
                            }else{
                                animation.x += 1
                                //interval = interval-1
                            }
                        }
//                        console.log(progress.width+","+animation.x+","+progress.x+","+(progress.x+progress.width))
                    }
                }

            }

            Slider {
                id: slider
                width: progressBackground.width
                anchors{
                    top: progress.bottom
                }
                value: 0.5
                visible: false
            }
        }


        Rectangle{
            id:buttonArea
            width:parent.width-40
            height:50
            color: parent.color
            anchors{
                top: progressField.bottom
                topMargin: 60
                horizontalCenter: parent.horizontalCenter
            }
            RoundButton{
                id:receivePause
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
                    if(receivePause.text==="\u003D"){
                        receivePause.text = "\u25B2"
                        //receivePause.rotation = 0
                        receivePause.font.pointSize = 15
                        animationTimer.stop()
                    }else{
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
            }
        }
        Rectangle{
            id:sendField
            width: parent.width
            height:0.18*parent.height
            anchors{
                bottom: parent.bottom
            }
            Button{
                id:send
                width: parent.width
                height: parent.height
                text: "SEND"
                font{
                    pointSize: 15
                    family: "Verdana"
                }
                palette.button: "#323232"
                palette.buttonText: "#cea392"
            }
        }
    }
}

