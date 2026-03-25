import QtQuick 2.12
import QtQuick.Controls 2.12

TextField {
    id: root

    placeholderText: "Enter text"
    padding: 10
    focus: true
    background: Rectangle {
        radius: 6
        border.color: "#0B5C51"
        border.width: 2
        color: "#8DEBDA"
    }
}
