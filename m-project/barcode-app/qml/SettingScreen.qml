import QtQuick 2.12
import QtQuick.Controls 2.12
import Qt.labs.platform 1.1
import "qrc:/qml/common"
import Qt.labs.settings 1.0

Popup {
    id: root
    width: 550
    height: 250
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside


    property string folderPath: ""

    signal folderSelected(string path)

    Settings {
        id: appSettings
        category: "General"
        property alias savedPath: root.folderPath
    }

    background: Rectangle {
        radius: 10
        color: "#79CAD4"
        border.color: "#ccc"
    }
    onOpened: {
        pathField.forceActiveFocus()
    }
    Column {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 20

        Text {
            text: "Cài đặt - thông tin lưu log file"
            font.pixelSize: 16
            font.bold: true
        }

        // ===== TEXTFIELD =====
        TextField {
            id: pathField
            width: parent.width
            height: 40
            placeholderText: "Nhập đường dẫn tại đây ..."
            text: root.folderPath
            font.pixelSize: 17
            focus: true
            activeFocusOnPress: true
            selectByMouse: true

            onTextChanged: root.folderPath = text

            background: Rectangle {
                radius: 6
                border.color: "#417E85"
                border.width: 2
                color: "#92C4C9"
            }
        }

        // ===== BUTTON ROW =====
        Row {
            spacing: 40
            anchors.horizontalCenter: parent.horizontalCenter

            MyButton {
                text: "Chọn đường dẫn"
                width: 130
                height: 45
                font.pixelSize: 14
                bgColor: "#EBC167"
                onClicked: folderDialog.open()
            }

            MyButton {
                text: "Mở thư mục"
                width: 130
                height: 45
                font.pixelSize: 14
                bgColor: "#EBC167"
                onClicked: {
                    Qt.openUrlExternally("file:///" + root.folderPath)
                }
            }
        }

        // ===== CLOSE BUTTON =====
        MyButton {
            text: "Close"
            width: parent.width
            font.pixelSize: 14
            height: 40
            bgColor: "#81D479"
            onClicked: {
                root.folderSelected(root.folderPath)
                root.close()
            }
        }
    }

    // ===== FOLDER DIALOG =====
    FolderDialog {
            id: folderDialog
            title: "Chọn thư mục lưu Log"
            onAccepted: {
                // Cách lấy đường dẫn sạch nhất
                let path = folderDialog.folder.toString();
                if (Qt.platform.os === "windows") {
                    path = path.replace("file:///", "");
                } else {
                    path = path.replace("file://", "");
                }
                root.folderPath = decodeURIComponent(path); // Giải mã các ký tự đặc biệt như %20 (dấu cách)
            }
        }
}
