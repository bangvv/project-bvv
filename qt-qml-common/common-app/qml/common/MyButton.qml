import QtQuick 2.12
import QtQuick.Controls 2.12

Button {
    id: root
    property color bgColor: "blue"

    background: Rectangle {
        color: root.bgColor
        radius: 8
    }

    contentItem: Text {
        text: root.text
        color: "white"
        anchors.centerIn: parent
    }
}
