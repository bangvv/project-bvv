import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.settings 1.0
import "qrc:/qml/common"

Popup {
    id: root
    width: 550
    height: 250
    anchors.centerIn: parent
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

    background: Rectangle {
        radius: 10
        color: "#79CAD4"
        border.color: "#ccc"
    }

    contentItem: ColumnLayout {
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15

        // ===== Tiêu đề =====

        Text {
            text: "Thông tin người vận hành"
            font.pixelSize: 18
            font.bold: true
        }

        // ===== Operator =====
        Item {
            Layout.fillWidth: true
            height: 40

            Row {
                anchors.fill: parent
                spacing: 8

                TextField {
                    id: operatorField
                    placeholderText: "Nhập id người vận hành..."
                    selectByMouse: true

                    width: parent.width - 40
                    height: parent.height
                    font.pixelSize: 17
                    background: Rectangle {
                        radius: 6
                        border.color: "#417E85"
                        border.width: 2
                        color: "#92C4C9"
                    }
                }

                Rectangle {
                    width: 32
                    height: 32
                    radius: 16
                    visible: operatorField.text.length > 0
                    anchors.verticalCenter: parent.verticalCenter

                    color: mouseArea.pressed ? "#ddd" : "#f5f5f5"
                    border.color: "#A1821B"
                    border.width: 2

                    Text {
                        text: "✕"
                        anchors.centerIn: parent
                        font.pixelSize: 14
                        color: "#333"
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        onClicked: operatorField.clear()
                    }
                }
            }
        }

        // ===== Checkbox =====
//        CheckBox {
//            text: "Âm cảnh báo"
//            checked: true
//            Layout.fillWidth: true
//        }

        // ===== Spacer đẩy nút xuống dưới =====
        Item {
            Layout.fillHeight: true
        }

        // ===== Close Button =====
        MyButton {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 100
            font.pixelSize: 14
            Layout.preferredHeight: 40
            text: "Đóng"
            bgColor: "#81D479"

            onClicked: {
                appController.changeCount(appController.count + 1)
                isPass = !isPass
                root.close()
            }
        }
    }
}
