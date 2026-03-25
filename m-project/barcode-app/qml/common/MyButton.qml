import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    id: root
    property color bgColor: "blue"

    background: Rectangle {
        color: root.bgColor
        radius: 6
        border.color: "#A1821B"
        border.width: 2
    }


    contentItem: Text {
        text: root.text
        color: "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        anchors.fill: parent
        font.pixelSize: root.font.pixelSize
    }
}
