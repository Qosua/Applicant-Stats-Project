import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import ApplicantStatsProjectModule

Item {
    id: page

    property var payload: null
    property int sourceIndex: payload ? (payload.sourceIndex ?? -1) : -1
    property var stats: null

    function reloadStats() {
        stats = (sourceIndex >= 0)
            ? statsPageModel.directionStatsAt(sourceIndex)
            : null
    }

    Connections {
        target: statsPageModel
        function onLoadDirectionPage(index) { page.reloadStats() }
    }

    onSourceIndexChanged: reloadStats()
    Component.onCompleted: reloadStats()

    //panel component
    component StatCard: Rectangle {
        property string label: ""
        property var value: "—"
        property color accentColor: "#888888"

        Layout.fillWidth: true
        Layout.preferredHeight: 78
        radius: 8
        color: "#222326"
        border.color: "#2d2d35"
        border.width: 1

        // Цветной акцент слева
        Rectangle {
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 1
            width: 3
            radius: 2
            color: accentColor
            opacity: 0.85
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 14
            anchors.leftMargin: 18
            spacing: 4

            Label {
                text: label
                color: "#dddddd"
                font.pixelSize: 11
                font.weight: Font.DemiBold
            }
            Item {
                Layout.fillHeight: true
            }
            Label {
                text: value
                color: accentColor
                font.pixelSize: 26
                font.weight: Font.DemiBold
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12


        //page header
        Label {
            text: stats ? stats.name : "STATS ARE NULL"
            color: "#eeeeee"
            font.pixelSize: 24
            font.weight: Font.Medium
        }
        Label {
            text: stats ? "Факультет: " + stats.facultyName : "STATS ARE NULL"
            color: "#aaaaaa";
            font.pixelSize: 16
            font.weight: Font.Medium
        }

        //page header
        RowLayout {
            spacing: 12
            Label {
                text: "Вид обучения:";
                color: "#dddddd"; font.pixelSize: 15
                font.weight: Font.Medium
            }
            Label {
                text: (stats ? stats.studyType : "ОШИБКА ВИДА ОБУЧЕНИЯ (QML)");
                color: "#aad3a8"; font.pixelSize: 15
                font.weight: Font.Medium
            }
            Item {
            }
            Label {
                text: "Форма обучения:";
                color: "#dddddd"; font.pixelSize: 15
                font.weight: Font.Medium
            }
            Label {
                text: (stats ? stats.studyForm : "ОШИБКА ФОРМЫ ОБУЧЕНИЯ (QML)");
                color: "#9ca9c8"; font.pixelSize: 15
                font.weight: Font.Medium
            }
            Item {
            }
            Label {
                text: "Код направления:";
                color: "#dddddd"; font.pixelSize: 15
                font.weight: Font.Medium
            }
            Label {
                text: (stats ? stats.code : "ОШИБКА КОДА НАПРАВЛЕНИЯ (QML)");
                color: "#b891a2"; font.pixelSize: 15
                font.weight: Font.Medium
            }
        }

        //page header divider
        Rectangle {
            Layout.fillWidth: true;
            height: 1
            color: "#404040"
        }

        //score graph and applicants list
        SplitView {

            handle: Rectangle {

                implicitWidth: 6

                color: SplitHandle.hovered ? "#556da3" : "transparent"

                Behavior on color {
                    ColorAnimation {
                        duration: 100
                    }
                }
            }

            anchors.margins: 2
            spacing: 4

            Layout.fillWidth: true
            Layout.preferredHeight: (parent.height / 2) + 120

            //score graph
            ScoreDist {
                SplitView.preferredHeight: parent.height
                SplitView.preferredWidth: parent.width / 2
                SplitView.minimumWidth: parent.width / 2.4

                distribution: stats ? stats.scoreDistribution : []
                firstBucketScore: stats ? stats.scoreBucketStart : 100
                bucketStep: stats ? stats.scoreBucketStep : 10
            }

            //applicants list
            Item {
                SplitView.minimumWidth: parent.width / 4

                //header
                Row {
                    id: listHeader
                    width: parent.width
                    height: 24
                    anchors.top: parent.top
                    anchors.leftMargin: 20
                    spacing: 6
                    leftPadding: 20

                    Label {
                        width: 25
                        text: "#"
                        color: "#555555"
                        font.pixelSize: 15
                        font.family: "Consolas"
                    }
                    Label {
                        width: 70
                        text: "ID"
                        color: "#557799"
                        font.pixelSize: 15
                        font.family: "Consolas"
                    }
                    Label {
                        width: 70
                        text: "Баллы"
                        color: "#6a9960"
                        font.pixelSize: 15
                    }
                    Label {
                        width: 140
                        text: "Телефон"
                        color: "#6a6a6a"
                        font.pixelSize: 15
                        font.family: "Consolas"
                    }
                    Label {
                        text: "ФИО"
                        font.pixelSize: 15
                        color: "#9a9aa0"
                        font.family: "Consolas"
                    }
                }

                //graph divider
                Rectangle {
                    id: headerDivider
                    anchors.top: listHeader.bottom
                    width: parent.width
                    height: 1
                    color: "#3a3c40"
                }

                //list
                ListView {
                    id: applicantListView
                    clip: true

                    anchors.top: headerDivider.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.topMargin: 4

                    model: applicantsListModel
                    spacing: 1

                    NumberAnimation {
                        id: scrollAnim
                        target: applicantListView
                        property: "contentY"
                        duration: 180
                        easing.type: Easing.OutQuad
                    }
                    WheelHandler {
                        onWheel: (event) => {
                            const delta = event.angleDelta.y * 0.54
                            const animY = scrollAnim.running
                                ? scrollAnim.to
                                : applicantListView.contentY

                            scrollAnim.to = Math.max(0, Math.min(animY - delta, applicantListView.contentHeight - applicantListView.height))
                            scrollAnim.restart()
                        }
                    }

                    delegate: Rectangle {
                        id: applicantDelegate

                        required property int index
                        required property string applicantName
                        required property int applicantId
                        required property string applicantEmail
                        required property string applicantPhone
                        required property string applicantScore

                        width: ListView.view.width
                        height: 24
                        radius: 4

                        color: hoverHandler.hovered ? "#34363a" : "#26282b"

                        Behavior on color {
                            ColorAnimation {
                                duration: 100
                            }
                        }

                        HoverHandler {
                            id: hoverHandler
                        }

                        RowLayout {
                            anchors.fill: parent
                            anchors.leftMargin: 20
                            anchors.rightMargin: 20
                            spacing: 6

                            Label {
                                text: applicantDelegate.index + 1
                                color: "#777777"
                                font.pixelSize: 11
                                font.family: "Consolas"
                                horizontalAlignment: Text.AlignLeft
                                Layout.preferredWidth: 25
                            }

                            Label {
                                text: applicantDelegate.applicantId
                                color: "#7f9cc4"
                                font.pixelSize: 12
                                font.family: "Consolas"
                                Layout.preferredWidth: 70
                            }

                            Label {
                                text: applicantDelegate.applicantScore
                                color: "#91cd84"
                                font.pixelSize: 11
                                elide: Text.ElideRight
                                Layout.preferredWidth: 70
                            }

                            Label {
                                text: applicantDelegate.applicantPhone
                                color: "#9a9a9a"
                                font.pixelSize: 12
                                font.family: "Consolas"
                                Layout.preferredWidth: 140
                            }

                            Label {
                                text: applicantDelegate.applicantName
                                color: "#e8e8ec"
                                font.pixelSize: 12
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                                Layout.preferredWidth: 1
                            }
                        }
                    }
                }
            }
        }

        //divider2
        Rectangle {
            id: divider2
            Layout.fillWidth: true;
            height: 1
            color: "#404040"
        }

        //progressBar and panels
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 10

            //progressBar
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 78
                radius: 8
                color: "#222326"
                border.color: "#2d2d35"
                border.width: 1

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 14
                    spacing: 10

                    RowLayout {
                        spacing: 10
                        Label {
                            text: "ЗАЧИСЛЕНО"
                            color: "#dddddd"
                            font.pixelSize: 11
                            font.weight: Font.DemiBold
                            Layout.fillWidth: true
                        }
                        Label {
                            text: (stats ? stats.size : 0) + " / " + (stats ? stats.capacity : 0)
                            color: "#e8e8ec"
                            font.pixelSize: 18
                            font.weight: Font.DemiBold
                        }
                        Label {
                            text: stats && stats.capacity > 0
                                ? Math.round((stats.size / stats.capacity) * 100) + "%"
                                : "0%"
                            color: "#6fa86f"
                            font.pixelSize: 18
                            font.weight: Font.Medium
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 6
                        radius: 3
                        color: "#2a2a32"

                        Rectangle {
                            width: parent.width * (stats && stats.capacity > 0
                                ? Math.min(stats.size / stats.capacity, 1)
                                : 0)
                            height: parent.height
                            radius: parent.radius
                            gradient: Gradient {
                                orientation: Gradient.Horizontal
                                GradientStop {
                                    position: 0.0; color: "#4a8a4a"
                                }
                                GradientStop {
                                    position: 1.0; color: "#7ab87a"
                                }
                            }
                            Behavior on width {
                                NumberAnimation {
                                    duration: 350; easing.type: Easing.OutCubic
                                }
                            }
                        }
                    }
                }
            }

            //panels
            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                StatCard {
                    label: "ПРОХОДНОЙ"
                    value: stats && stats.size === stats.capacity ? stats.minScore : "-"
                    accentColor: "#d4a373"
                }
                StatCard {
                    label: "МАКСИМУМ"
                    value: stats && stats.maxScore !== undefined ? stats.maxScore : "-"
                    accentColor: "#7fb37f"
                }
                StatCard {
                    label: "МИНИМУМ"
                    value: stats && stats.minScore !== undefined ? stats.minScore : "-"
                    accentColor: "#c47878"
                }
                StatCard {
                    label: "СРЕДНИЙ"
                    value: stats && stats.meanScore !== undefined ? stats.meanScore.toFixed(1) : "-"
                    accentColor: "#7f9cc4"
                }
                StatCard {
                    label: "МЕДИАНА"
                    value: stats && stats.midScore !== undefined ? stats.midScore.toFixed(1) : "-"
                    accentColor: "#a37fc4"
                }
            }
        }

        Item {
            Layout.fillWidth: true; Layout.fillHeight: true
        }
    }
}