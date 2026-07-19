import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import ApplicantStatsProjectModule
import QtQuick.Effects

Window {
    id: dialog

    signal accepted(string tablePath, string tableName, string preset)
    signal canceled()

    onAccepted: (tablePath, tableName, preset) => {
        console.log(tablePath, tableName, preset)
        qmlHelper.addFileToAppDataAndDB(tablePath, tableName, preset)
    }

    title: "Настройки таблицы"
    width: 480
    height: 320
    color: "transparent"
    flags: Qt.Dialog | Qt.FramelessWindowHint
    modality: Qt.ApplicationModal

    Component.onCompleted: {
        x = Screen.virtualX + (Screen.width  - width)  / 2
        y = Screen.virtualY + (Screen.height - height) / 2
    }


    Shortcut {
        sequence: "Escape"
        onActivated: {
            qmlHelper.isMainWindowShadowed = false
            dialog.canceled();
            dialog.close()
        }
    }


    component CloseButton: Rectangle {
        id: cb
        signal clicked()
        implicitWidth: 36
        implicitHeight: 36
        radius: 7
        color: cbHover.hovered ? "#55373f" : "transparent"
        Behavior on color { ColorAnimation { duration: 100 } }

        Text {
            anchors.centerIn: parent
            text: "✕"
            color: cbHover.hovered ? "#e89d9d" : "#9a9aa0"
            font.pixelSize: 18
        }
        HoverHandler { id: cbHover; cursorShape: Qt.PointingHandCursor }
        TapHandler   { onTapped: cb.clicked() }
    }

    component StyledField: TextField {
        id: tf
        color: "#e8e8ec"
        font.pixelSize: 14
        selectByMouse: true
        leftPadding: 12
        rightPadding: 12
        placeholderTextColor: "#5c6066"

        background: Rectangle {
            radius: 8
            implicitHeight: 34
            color: tf.hovered ? "#292b2e" : "#232527"
            border.width: 1
            border.color: tf.activeFocus ? "#556da3"
                : (tf.hovered ? "#3a3c40" : "#323232")
            Behavior on color { ColorAnimation { duration: 100 } }
        }
    }

    component DialogButton: Button {
        id: btn
        property bool primary: false
        flat: true
        hoverEnabled: true
        implicitHeight: 34
        implicitWidth: Math.max(104, btnText.implicitWidth + 34)

        contentItem: Text {
            id: btnText
            text: btn.text
            color: !btn.enabled ? "#6a6c70"
                : (btn.primary ? "#ffffff" : "#d4d8e0")
            font.pixelSize: 14
            font.weight: btn.primary ? Font.Medium : Font.Normal
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        background: Rectangle {
            radius: 8
            color: !btn.enabled ? "#26272a"
                : btn.primary
                    ? (btn.down ? "#48619c" : btn.hovered ? "#5f79b8" : "#556da3")
                    : (btn.down ? "#2a2c30" : btn.hovered ? "#34363a" : "#2a2b2e")
            border.width: btn.primary ? 0 : 1
            border.color: "#3a3c40"
            Behavior on color { ColorAnimation { duration: 100 } }
        }
        HoverHandler { cursorShape: btn.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor }
    }

    component FieldRow: RowLayout {
        property string label: ""
        Layout.fillWidth: true
        spacing: 16

        Label {
            text: parent.label
            color: "#cccccc"
            font.pixelSize: 16
            Layout.preferredWidth: 150
            Layout.alignment: Qt.AlignVCenter
        }
    }

    Rectangle {
        id: shadowSource
        anchors.fill: bg
        radius: bg.radius + 8
        color: "black"
        visible: false
        layer.enabled: true
    }
    MultiEffect {
        anchors.fill: shadowSource
        source: shadowSource
        shadowEnabled: true
        shadowColor: "#000000"
        shadowOpacity: 0.3
        shadowBlur: 1.3
        shadowVerticalOffset: 9
        shadowHorizontalOffset: 9
    }

    Rectangle {
        id: bg
        anchors.fill: parent
        anchors.margins: 10
        radius: 8
        color: "#1e1f22"
        border.color: "#323232"
        border.width: 1

        ColumnLayout {
            id: contentColumn
            anchors.fill: parent
            spacing: 0

            Rectangle {
                id: titleBar
                Layout.fillWidth: true
                Layout.preferredHeight: 46
                color: "transparent"

                MouseArea {
                    anchors.fill: parent
                    property point clickPos
                    onPressed: (mouse) => clickPos = Qt.point(mouse.x, mouse.y)
                    onPositionChanged: (mouse) => {
                        dialog.x += mouse.x - clickPos.x
                        dialog.y += mouse.y - clickPos.y
                    }
                }

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 18
                    anchors.rightMargin: 10
                    spacing: 10
                    Label {
                        text: "Настройки таблицы"
                        color: "#eeeeee"
                        font.pixelSize: 18
                        font.weight: Font.Medium
                        Layout.fillWidth: true
                    }
                    CloseButton {
                        onClicked: {
                            qmlHelper.isMainWindowShadowed = false
                            dialog.canceled();
                            dialog.close()
                        }
                    }
                }
            }

            Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: "#2e2e2e" }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.margins: 22
                spacing: 18

                FieldRow {
                    label: "Путь таблицы"
                    StyledField {
                        id: pathField
                        Layout.fillWidth: true
                        placeholderText: "Файл не выбран"
                        readOnly: true
                        text: qmlHelper.droppedTablePath
                    }
                }

                FieldRow {
                    label: "Имя таблицы"
                    StyledField {
                        id: nameField
                        Layout.fillWidth: true
                        placeholderText: "Напишите здесь новое имя таблицы"
                        text: qmlHelper.droppedTableName
                    }
                }

                FieldRow {
                    label: "Пресет"
                    CustomComboBox {
                        id: presetCombo
                        Layout.fillWidth: true
                        implicitHeight: 34
                        comboBoxModel: entryCommisionsListModel
                    }
                }

            }

            Rectangle { Layout.fillWidth: true; implicitHeight: 1; color: "#2e2e2e" }

            Item { Layout.fillHeight: true; Layout.fillWidth: true;}

            RowLayout {
                Layout.fillWidth: true
                Layout.margins: 8
                spacing: 8

                Item { Layout.fillWidth: true }

                DialogButton {
                    text: "Подтвердить"
                    primary: true
                    enabled: nameField.text.length > 0
                    onClicked: {
                        dialog.accepted(
                            pathField.text,
                            nameField.text,
                            presetCombo.currentIndex,
                        )
                        dialog.close()
                    }
                }
                DialogButton {
                    text: "Отмена"
                    onClicked: { dialog.canceled(); dialog.close() }
                }
            }
        }
    }
}
