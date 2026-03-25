import QtQuick 2.12
import QtQuick.Controls 2.12

ComboBox {
    id: root

    property color bgColor: "#67CAEB"

    // 👉 ô chính
    background: Rectangle {
        radius: 6
        color: root.bgColor
    }

    // 👉 dropdown
    popup: Popup {
        y: root.height
        width: root.width

        contentItem: ListView {
            id: listView
            clip: true

            model: root.delegateModel

            // 👇 giới hạn 5 item
            implicitHeight: Math.min(contentHeight, 5 * 40)

            delegate: Item {
                width: ListView.view.width
                height: 40

                property int itemIndex: index
                property bool isSelected: root.currentIndex === itemIndex

                Rectangle {
                    anchors.fill: parent

                    color: isSelected
                           ? "#d0ebff"
                           : (mouseArea.containsMouse
                              ? "#eeeeee"
                              : (itemIndex % 2 === 0 ? "#BAD6DB" : "#96AFB3"))
                }

                Text {
                    text: modelData
                    anchors.centerIn: parent
                    color: "black"
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true

                    onClicked: {
                        root.currentIndex = itemIndex
                        root.popup.close()
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
            }
        }

        background: Rectangle {
            radius: 8
            color: "white"
            border.color: "#ccc"
        }
    }
}
