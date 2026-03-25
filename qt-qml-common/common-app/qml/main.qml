import QtQuick 2.12
import QtQuick.Controls 2.12
import "qrc:/qml/common"
import App 1.0

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("App Common")
    color: "#CEE8ED"

    minimumWidth: 640
    maximumWidth: 640
    minimumHeight: 480
    maximumHeight: 480

    property int  count: 0

    AppController{ id:appController }

    MyButton{
        anchors.centerIn: parent
        text: "click me"
        bgColor: "#EBC167"
        onClicked: {
            appController.changeCount(appController.count + 1)
            if (appController.count % 3 == 0) {
                dialog.visible = true
            }
        }
    }

    Text {
        id: countText
        anchors.horizontalCenter: parent.horizontalCenter
        y: 100
        text: qsTr("Count Click: "+appController.count.toString())
    }

    Text {
        id: labelName
        text: appController.appName
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

//    MyTextField{
//        x: 100
//        y: 10
//        width: 400
//        height: 40
//        placeholderText: "Barcode Enter"
//        color: "#222"
//        font.pixelSize: 18
//    }

    MyDialog {
        id: dialog
        anchors.centerIn: parent

        title: "Confirm"
        message: "Are you sure you want to continue?"

        onAccepted: {
            console.log("OK")
        }

        onRejected: {
            console.log("Cancel")
        }
    }
}
