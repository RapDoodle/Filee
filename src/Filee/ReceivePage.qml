import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4


Page{
    Rectangle{
        color: "lightblue"
        anchors.fill: parent

        Rectangle{
            id:fileNamePlace
            width: parent.width
            height: fileName.height
            color: parent.color
            anchors{
                top: parent.top
                topMargin: parent.height*0.1
                leftMargin: parent.width*0.01
                rightMargin: leftMargin
            }
            TextEdit{
                id:fileName
                //width: parent.width-selectButton.width*2
                anchors{
                    //horizontalCenter: parent.horizontalCenter
                }
                font{
                    pointSize: 20
                }
                text: "H:\\mobile computing"
                persistentSelection: false
                selectByMouse: true
                readOnly: true
                //wrapMode: "WordWrap"
                function textwidth(){

                }
            }
            RoundButton{
                id: selectButton
                width: parent.width*0.2
                height: parent.height
                anchors{
                    leftMargin: 10
                    left: fileName.right
                }
                text: "select"
                palette.button: "salmon"// change button color
            }
        }

        Rectangle{
            ProgressBar {
                id: progressBar
                x: 150
                y: 306
                antialiasing: true
                hoverEnabled: true
                indeterminate: false
                //value: 0.5
                value:slider.value

                ProgressBarStyle{
                    background: Rectangle{
                        radius: 2
                        color: "lightgray"
                        border.color: "gray"
                        border.width: 1
                        implicitWidth: 200
                        implicitHeight: 24
                    }
                    progress: Rectangle {
                        color: "lightsteelblue"
                        border.color: "steelblue"
                    }
                }

                Slider {
                    id: slider
                    x: 0
                    y: 0
                    value: 0.5
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
