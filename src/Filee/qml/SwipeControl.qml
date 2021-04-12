import QtQuick 2.0
import QtQuick.Controls 2.2

SwipeView{

    id:swipeView
    width: parent.width
    height: parent.height*0.30
    //property alias _send : send
    //interactive: false//禁止手动滑动swipe界面
    SendPage{
        id:send

    }
    ReceivePage{
        id:receive
    }
}
