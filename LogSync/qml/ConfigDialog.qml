import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQml 2.12
import QtQuick.Dialogs 1.3

ApplicationWindow {
    id: win
    visible: true
    width: 600
    height: 500
    title: "LogSync Configuration"

    minimumWidth: 600
    minimumHeight: 400
    maximumWidth: 600
    maximumHeight: 400

    Component.onCompleted: {
        console.log("CONFIG =", config)
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 15

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: config.model
            spacing: 5

            delegate: RowLayout {
                spacing: 8
                Layout.fillWidth: true

                Label {
                    text: index + 1
                    Layout.preferredWidth: 20
                    Layout.preferredHeight: 35
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                // SOURCE
                TextField {
                    id: src
                    Layout.preferredHeight: 35
                    Layout.preferredWidth: 180

                    text: model.source

                    onEditingFinished: {
                        config.model.setData(
                            config.model.index(index, 0),
                            text,
                            257
                        )
                    }
                }

                Button {
                    text: "Browse"
                    Layout.preferredWidth: 60
                    Layout.preferredHeight: 35

                    onClicked: {
                        srcDialog.rowIndex = index
                        srcDialog.open()
                    }
                }

                // DEST
                TextField {
                    id: dst
                    Layout.preferredHeight: 35
                    Layout.preferredWidth: 180

                    text: model.dest

                    onEditingFinished: {
                        config.model.setData(
                            config.model.index(index, 0),
                            text,
                            258
                        )
                    }
                }

                Button {
                    text: "Browse"
                    Layout.preferredWidth: 60
                    Layout.preferredHeight: 35

                    onClicked: {
                        dstDialog.rowIndex = index
                        dstDialog.open()
                    }
                }

                Button {
                    text: "X"
                    Layout.preferredWidth: 30
                    Layout.preferredHeight: 35
                    onClicked: config.model.removePair(index)
                }
            }
        }

        FileDialog {
            id: srcDialog
            selectFolder: true

            property int rowIndex: -1

            onAccepted: {
                let path = ("" + fileUrl).replace("file:///", "")

                let idx = config.model.index(rowIndex, 0)
                config.model.setData(idx, path, 257)
            }
        }
        FileDialog {
            id: dstDialog
            selectFolder: true

            property var rowIndex

            onAccepted: {
                let path = ("" + fileUrl).replace("file:///", "")

                let idx = config.model.index(rowIndex, 0)
                config.model.setData(idx, path, 258)
            }
        }

        RowLayout {
            CheckBox {
                id: modeCheck
                text: "Mode YYYYMM"
                checked: config.isMonthMode()

                onCheckedChanged: {
                    config.setMonthMode(checked)
                }
            }

            Button {
                text: "+ Add Pair"
                onClicked: config.model.addPair()
            }

            Button {
                text: "Save"
                onClicked: {
                    win.visible = false
                    config.save()
                }
            }


        }
    }
}
