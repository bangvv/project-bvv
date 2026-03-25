import QtQuick 2.12
import QtQuick.Controls 2.12
import "qrc:/qml/common"
import App 1.0

ApplicationWindow {
    id: win
    width: 840
    height: 680
    visible: true
    title: qsTr("Barcode Log")
    color: "#CEE8ED"

    minimumWidth: 840
    maximumWidth: 840
    minimumHeight: 680
    maximumHeight: 680

    property int  count: 0
    property bool allowClose: false
    property bool isPass: false
    property var logList: []

    AppController{ id:appController }
    DataReceiver { id: myReceiver }
    SettingScreen{
        id:settingScreenId
        anchors.centerIn: parent
    }

    MyButton{
        x:500
        y:580
        width: 100
        height: 45
        text: "click me"
        font.pixelSize: 18
        bgColor: "#EBC167"
        onClicked: {
            appController.changeCount(appController.count + 1)
            isPass =! isPass
        }
    }
    Row {
        x:0
        y:0
        Button {
            text: "Cài đặt"
            width: 70
            font.pixelSize: 14
            onClicked: {
                settingScreenId.open()
            }
            background: Rectangle{
                color: "#F5D8A2"
                border.color: "#B3841E"
            }
        }

        Button {
            text: "Info"
            width: 70
            font.pixelSize: 14
            onClicked: {
                console.log("Open Info")
            }
            background: Rectangle{
                color: "#F5D8A2"
                border.color: "#B3841E"
            }
        }

        Button {
            text: "Other"
            width: 70
            font.pixelSize: 14
            onClicked: {
                console.log("Other action")
            }
            background: Rectangle{
                color: "#F5D8A2"
                border.color: "#B3841E"
            }
        }
    }

//    MyComboBoxStyleColor {
//        x: 100
//        y: 10
//        width: 400
//        model: ["Apple", "Banana", "Orange","Gohan","Goku","Vegeta"]

//        onActivated: (i, text) => {
//            console.log("Selected:", text, i)
//        }
//    }

    MyTextField{
        anchors.horizontalCenter: parent.horizontalCenter
        y: 130
        width: 500
        height: 40
        placeholderText: "Barcode Enter"
        color: "#222"
        font.pixelSize: 18
        onAccepted: {
            if (settingScreenId.folderPath == ""){
                dialogWarning.open()
                return
            }

            if (text !== "") {
                var status = isPass==true?"OK":"NG"
                myReceiver.saveToCsv("",text,status,settingScreenId.folderPath)
                isPass = true
                var newEntry = myReceiver.time + "  " + text + " " + status;
                logList.unshift(newEntry);
                if (logList.length > 5) { logList.pop(); }
                logArea.text = logList.join("\n");
                text = ""
            }else{
                isPass = false
            }
        }
    }

    TextArea {
        id: logArea
        x: 20
        y: 200
        width: 500
        height: 220
        readOnly: true
        color: "black"
        wrapMode: TextArea.Wrap
        text: ""
        font.pixelSize: 18
        background: Rectangle {
            radius: 4
            border.color: "white"
            border.width: 2
            color: "#DEEBDD"
        }
    }

    Rectangle{
        id: recResult
        x: 540
        y: 200
        width: 280
        height: 220
        radius: 4
        border.color: isPass == true ? "#1AC70A" : "#F2330D"
        border.width: 2
        color: isPass == true ? "#C1FBBB" : "#F9A190"
        Text {
            id: result
            anchors.fill: recResult
            text: isPass == true ? qsTr("PASS") : qsTr("NG")
            font.pixelSize: 110
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: isPass == true ? "#1AC70A" : "#F2330D"
        }
    }

    MyDialog {
        id: dialogClose
        anchors.centerIn: parent

        title: "Xác nhận"
        message: "Bạn muốn thoát ứng dụng?"
        accept: "Thoát"
        reject: "Trở lại"

        onAccepted: {
            win.allowClose = true
            win.close()
        }

        onRejected: {
            dialogClose.close()
        }
    }

    MyDialog {
        id: dialogWarning
        anchors.centerIn: parent
        width: 600

        title: "Cảnh báo!!!"
        message: "Bạn chưa chọn đường dẫn lưu log file cho ứng dụng, hãy chọn đường dẫn trước!"
        accept: "Đồng ý"
        reject: "Trở lại"

        onAccepted: {
            settingScreenId.open()
        }

        onRejected: {
            dialogClose.close()
        }
    }

    onClosing: function(close) {
        if (allowClose) {
            close.accepted = true
        } else {
            close.accepted = false
            dialogClose.open()
        }
    }
}
