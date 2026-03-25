import QtQuick 2.12
import QtQuick.Controls 2.12

Popup {
    id: root

    property var model: []
    signal itemSelected(int index, string text)

    width: 180
    padding: 4

    modal: false
    focus: true

    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape

    background: Rectangle {
        radius: 8
        color: "white"
        border.color: "#ccc"
    }

    // ===== ANIMATION =====
    enter: Transition {
        NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 120 }
    }

    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 80 }
    }

    // ===== LIST =====
    contentItem: ListView {
        id: listView
        model: root.model
        implicitHeight: contentHeight
        clip: true

        delegate: Rectangle {
            width: parent.width
            height: 36
            radius: 6

            property bool hovered: false

            color: hovered ? "#e7f5ff" : "transparent"

            Text {
                text: modelData
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                color: "#333"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onEntered: hovered = true
                onExited: hovered = false

                onClicked: {
                    root.itemSelected(index, modelData)
                    root.close()
                }
            }
        }

        ScrollBar.vertical: ScrollBar {
            policy: ScrollBar.AsNeeded
        }
    }
}
