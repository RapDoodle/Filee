import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.2
//import QtQuick.Controls 1.2


Page{
    Rectangle{
        color: "#525252"
        anchors.fill: parent

        Rectangle{
            id:fileNamePlace
            width: parent.width-40
            height: fileName.height
            color: parent.color
            anchors{
                top: parent.top
                topMargin: parent.height*0.1
                leftMargin: parent.width*0.01
                rightMargin: leftMargin
                horizontalCenter: parent.horizontalCenter
            }
            TextEdit{
                id:fileName
                width: parent.width-selectButton.width*2
                anchors{
                    horizontalCenter: parent.horizontalCenter
                }
                font{
                    pointSize: 12
                    family: "Verdana"
                }
                color: "#f4eeeb"
                text: "H:\\mobile computing"
                persistentSelection: false
                selectByMouse: true
                readOnly: true
                //wrapMode: "WordWrap"
                function textwidth(){

                }
            }
//            RoundButton{
//                id: selectButton
//                width: parent.width*0.2
//                height: parent.height
//                anchors{
//                    leftMargin: 10

//                    right: parent.right
//                }
//                text: "select"
//                font{
//                    pointSize: 15
//                }
//                palette.button: "salmon"// change button color
//            }
        }

        Rectangle{
            id: progressField
            width: parent.width-40
            anchors{
                top: fileNamePlace.bottom

                topMargin: 35
                horizontalCenter: parent.horizontalCenter
            }
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
                    }else{
                        receivePause.text = "\u003D"
                        receivePause.rotation = 90
                        receivePause.font.pointSize = 30

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
//        Rectangle{
//            id:sendField
//            width: parent.width
//            height:0.125*parent.height
//            anchors{
//                bottom: parent.bottom
//            }
//            Button{
//                id:send
//                width: parent.width
//                text: "send"
//                font{
//                    pointSize: 15
//                }
//            }
//        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
