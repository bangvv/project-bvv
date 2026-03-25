import QtQuick 2.12
import QtQuick.Controls 2.12

Popup {
    id: root

    property string title: "Dialog"
    property string message: ""

    signal accepted()
    signal rejected()

    modal: true
    focus: true
    closePolicy: Popup.CloseOnPressOutside | Popup.CloseOnEscape

    width: 320
    padding: 0

    background: Rectangle {
        radius: 10
        color: "#C5CFA5"
        border.color: "#798849"
    }

    // ===== CONTENT =====
    Column {
        anchors.fill: parent
        spacing: 20
        padding: 20

        // Title
        Text {
            text: root.title
            font.pixelSize: 18
            font.bold: true
        }

        // Message
        Text {
            text: root.message
            anchors.horizontalCenter: parent.horizontalCenter
            wrapMode: Text.WordWrap
            color: "#444"
        }

        // Buttons
        Row {
            spacing: 70
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                text: "Cancel"
                width: 100
                height: 40
                background: Rectangle {
                    radius: 8
                    color: "#E7BF46"
                    border.color: "#907113"
                }
                onClicked: {
                    root.rejected()
                    root.close()
                }
            }

            Button {
                text: "OK"
                width: 100
                height: 40
                background: Rectangle {
                    radius: 8
                    color: "#71EC6F"
                    border.color: "#159013"
                }
                onClicked: {
                    root.accepted()
                    root.close()
                }
            }
        }
    }

    // ===== ANIMATION =====
    enter: Transition {
        ParallelAnimation {
            NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 120 }
            NumberAnimation { property: "scale"; from: 0.95; to: 1; duration: 120 }
        }
    }

    exit: Transition {
        NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 100 }
    }
}
