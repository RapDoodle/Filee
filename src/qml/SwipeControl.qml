import QtQuick 2.0
import QtQuick.Controls 2.2

SwipeView{

    id: swipeView
    width: parent.width
    height: parent.height*0.30
    interactive: false  // Set to false to disable swiping
    SendPage {
        id: send

    }
//    ReceivePage {
//        id: receive
//    }
}
