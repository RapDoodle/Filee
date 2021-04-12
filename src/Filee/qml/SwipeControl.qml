import QtQuick 2.0
import QtQuick.Controls 2.2

SwipeView{

    id:swipeView
    width: parent.width
    height: parent.height*0.4
    property alias _send : send
    //interactive: false
    SendPage{
        id:send

    }
    ReceivePage{
        id:receive
    }
}
