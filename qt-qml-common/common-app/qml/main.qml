import QtQuick 2.12
import QtQuick.Controls 2.12
import "qrc:/qml/common"
import App 1.0

ApplicationWindow {
    width: 640
    height: 480
    visible: true
    title: qsTr("App Common")

    minimumWidth: 640
    maximumWidth: 640
    minimumHeight: 480
    maximumHeight: 480

    property int  count: 0

    AppController{ id:appController }

    MyButton{
        anchors.centerIn: parent
        text: "click me"
        bgColor: "red"
        onClicked: {
            appController.changeCount(appController.count + 1)
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
}
