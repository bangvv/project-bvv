import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12

Item {
    id: root
    width: 220
    height: 42
    focus: true

    property var model: ["A", "B", "C"]
    property int currentIndex: 0
    property string currentText: model.length > 0 ? model[currentIndex] : ""

    signal activated(int index, string text)
    property bool hovered: false

    // ===== MAIN BOX =====
    Rectangle {
        id: box
        anchors.fill: parent
        radius: 4
        color: "#67CAEB"
        border.width: root.activeFocus ? 2 : 1
        border.color: root.activeFocus ? "#1976D2" : "#999"

        Behavior on border.color {
            ColorAnimation { duration: 150 }
        }

        Text {
            text: root.currentText
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 12
            font.pixelSize: 14
            color: "#222"
            elide: Text.ElideRight
        }

        // icon ▼
        Text {
            text: "▼"
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.verticalCenter: parent.verticalCenter
            color: "#333"
            font.pixelSize: 12
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                root.forceActiveFocus()
                popup.opened ? popup.close() : popup.open()
            }
        }
    }

    // ===== POPUP =====
    Popup {
        id: popup
        width: root.width
        y: root.height + 4
        padding: 4

        modal: false
        focus: true

        closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape

        background: Rectangle {
            radius: 10
            color: "white"
            border.color: "#ccc"
        }

        enter: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 120 }
                NumberAnimation { property: "scale"; from: 0.95; to: 1; duration: 120 }
            }
        }

        exit: Transition {
            NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 80 }
        }

        contentItem: ListView {
            id: listView
            clip: true
            model: root.model
            currentIndex: root.currentIndex
            spacing: 3

            implicitHeight: Math.min(contentHeight, 5 * 40)
            highlightMoveDuration: 120

            delegate: Rectangle {
                width: parent.width
                height: 40
                radius: 8

                property int  indexCurrent: index
                property bool isSelected: indexCurrent === root.currentIndex

                color: isSelected
                       ? "#5FB7ED"
                       : (index % 2 === 0 ? "#D1E0E6" : "#95B9C6")

                Behavior on color {
                    ColorAnimation { duration: 120 }
                }

                Text {
                    text: modelData
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    font.pixelSize: 14
                    color: isSelected ? "white" : "#333"
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        root.currentIndex = index
                        root.activated(index, modelData)
                        popup.close()
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }
        }
    }

    // ===== KEYBOARD SUPPORT =====
    Keys.onPressed: (event) => {
        if (event.key === Qt.Key_Down) {
            popup.open()
            listView.currentIndex = Math.min(listView.currentIndex + 1, model.length - 1)
            event.accepted = true
        }
        else if (event.key === Qt.Key_Up) {
            popup.open()
            listView.currentIndex = Math.max(listView.currentIndex - 1, 0)
            event.accepted = true
        }
        else if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
            if (popup.opened) {
                root.currentIndex = listView.currentIndex
                root.activated(root.currentIndex, root.model[root.currentIndex])
                popup.close()
            } else {
                popup.open()
            }
            event.accepted = true
        }
        else if (event.key === Qt.Key_Escape) {
            popup.close()
        }
    }
}
