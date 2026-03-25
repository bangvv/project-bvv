import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
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
        spacing: 10

        // ===== TITLE =====
        Text {
            text: "Thông tin ứng dụng"
            font.pixelSize: 18
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }

        // ===== TEXT AREA =====
        TextArea {
            id: infoArea
            Layout.fillWidth: true
            Layout.fillHeight: true
            readOnly: true
            wrapMode: Text.Wrap

            text: "Version: 1.0.0\nBộ phận: E-EN PD\n"

            background: Rectangle {
                radius: 6
                border.color: "#417E85"
                border.width: 2
                color: "#92C4C9"
            }
        }

        // ===== CLOSE BUTTON =====
        MyButton {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 100
            Layout.preferredHeight: 40
            font.pixelSize: 14
            height: 40
            text: "Đóng"
            bgColor: "#81D479"

            onClicked: root.close()
        }
    }
}
