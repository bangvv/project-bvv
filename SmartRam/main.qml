import QtQuick 2.12
import QtQuick.Controls 2.12

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "Smart RAM Qt"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Text {
            id: ramText
            text: "Used RAM: " + ramCleaner.getUsedRamMB() + " MB"
            font.pointSize: 16
        }

        Button {
            text: "Clean RAM"
            onClicked: {
                ramCleaner.cleanRam()
                ramText.text = "Used RAM: " + ramCleaner.getUsedRamMB() + " MB"
            }
        }
    }
}
