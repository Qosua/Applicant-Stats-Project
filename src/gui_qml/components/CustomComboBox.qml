import QtQuick
import QtQuick.Controls

ComboBox {

    property var comboBoxModel: ["Очная", "Заочная", "Очно-заочная"]
    property bool

    id: comboBox
    model: comboBoxModel

    contentItem: Text {
        leftPadding: 12
        text: comboBox.displayText
        color: "#cccccc"
        font.pixelSize: 14
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        radius: 8
        color: comboBox.hovered ? "#34363a" : "#232527"
        border.color: comboBox.popup.visible ? "#556da3" : "#323232"
        border.width: 1

        Behavior on color {
            ColorAnimation { duration: 100 }
        }
    }

    indicator: Text {
        x: comboBox.width - width - 10
        y: comboBox.topPadding + (comboBox.availableHeight - height) / 2
        text: comboBox.popup.visible ? "⯅" : "⯆"
        color: "#607ab5"
        font.pixelSize: 12
    }

    delegate: ItemDelegate {
        width: comboBox.width
        height: 30

        contentItem: Text {
            text: modelData
            color: "#cccccc"
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            leftPadding: 12
            elide: Text.ElideRight
        }

        background: Rectangle {
            color: highlighted ? "#3a4660" : hovered ? "#34363a" : "#191a1c"
            Behavior on color {
                ColorAnimation { duration: 100 }
            }
        }

        highlighted: comboBox.highlightedIndex === index

        HoverHandler {
            cursorShape: Qt.PointingHandCursor
        }
    }

    popup: Popup {
        y: comboBox.height + 4
        width: comboBox.width
        implicitHeight: contentItem.implicitHeight + 8
        padding: 4

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: comboBox.popup.visible ? comboBox.delegateModel : null
            currentIndex: comboBox.highlightedIndex
        }

        background: Rectangle {
            color: "#191a1c"
            border.color: "#323232"
            border.width: 1
            radius: 8
        }
    }
}