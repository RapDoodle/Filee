
import QtQuick 2.0

Rectangle {
    id: test

    property alias text: ableUserName.text

    width: root.width*0.9
    height: root.height*0.15
    color: "lightsteelblue"
//    border.color: "slategrey"

    anchors {
        left: ableUserList.left
        leftMargin: ableUserList.width*0.05
        top: ableUserList.bottom
        topMargin: ableUserList.height*0.02
    }

    Rectangle{
        id: ableUserPic
        width: test.height*0.75
        height: test.height*0.75
        color: "pink"
        radius: 10
        anchors {
            left: test.left
            leftMargin: test.width*0.1
            top: test.top
            topMargin: test.height*0.125

        }
        TextInput {
                id: ableUserShortName
                text: "f"
                anchors.fill: parent

                font.capitalization: Font.Capitalize//单词首字母大写
                readOnly: true //只读
                inputMask:"A"//xianzhi
//              focus:true
//                for(var i=0;i<26;i++){ console.log(String.fromCharCode(65+i))
                font.pointSize: 14
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

//              font.pixelSize: parent.height-4

           }
    }

    Text {
        id: ableUserName
        width: test.width*0.6
        height: ableUserPic.height*0.5
        anchors {
            left: ableUserPic.right
            leftMargin: test.width*0.02
            top: ableUserPic.top
        }
        color: "#000000"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.capitalization: Font.Capitalize//单词首字母大写
        text:"foolish debu"
    }
    Text {
        id: ableUserNetworkStatus
        width: test.width*0.6
        height: ableUserPic.height*0.5
        anchors {
            left: ableUserPic.right
            leftMargin: test.width*0.02
            top: ableUserName.bottom
        }
        color: "#000000"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.capitalization: Font.Capitalize//单词首字母大写

        text:"Connection"

    }

//判断网络是否连接 咋整？


//    MouseArea {
//        anchors.fill: parent;
//        onClicked: {
//            if (stackView.depth == 0) {
//                // Only push the control view if we haven't pushed it
//                stackView.push(transferPanelView);
//                stackView.currentItem.forceActiveFocus();
//            }


//            }
//        }


//    }

}


