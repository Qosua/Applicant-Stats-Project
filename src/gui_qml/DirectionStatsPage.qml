import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import ApplicantStatsProjectModule

Item {
    id: page

    property var payload: null
    readonly property int sourceIndex: payload ? (payload.sourceIndex ?? -1) : -1

    readonly property var stats: sourceIndex >= 0
        ? cppStats.directionStatsAt(sourceIndex)
        : null

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        Label {
            text: stats ? stats.name : "STATS ARE NULL"
            color: "#eeeeee"
            font.pixelSize: 24
        }
        Label {
            text: stats ? "Факультет: " + stats.facultyName : "STATS ARE NULL"
            color: "#aaaaaa"; font.pixelSize: 16
        }

        RowLayout {
            spacing: 12
            Label {
                text: "Вид обучения:";
                color: "#dddddd"; font.pixelSize: 15
            }
            Label {
                text: (stats ? stats.studyType : "ОШИБКА ВИДА ОБУЧЕНИЯ (QML)");
                color: "#aad3a8"; font.pixelSize: 15
            }
            Item{}
            Label {
                text: "Форма обучения:";
                color: "#dddddd"; font.pixelSize: 15
            }
            Label {
                text: (stats ? stats.studyForm : "ОШИБКА ФОРМЫ ОБУЧЕНИЯ (QML)");
                color: "#9ca9c8"; font.pixelSize: 15
            }
        }

        Rectangle {
            Layout.fillWidth: true;
            height: 1
            color: "#404040"
        }

        RowLayout {
            spacing: 24
            Label {
                text: "Зачислено: " + (stats ? stats.size : 0) + " / " + (stats ? stats.capacity : 0);
                color: "#dddddd"; font.pixelSize: 15
            }
            Label {
                text: "Макс: " + (stats && stats.maxScore !== undefined ? stats.maxScore : "—");
                color: "#dddddd"; font.pixelSize: 15
            }
            Label {
                text: "Мин: "  + (stats && stats.minScore !== undefined ? stats.minScore : "—");
                color: "#dddddd"; font.pixelSize: 15
            }
            Label {
                text: "Средн: " + (stats && stats.meanScore !== undefined ? stats.meanScore.toFixed(1) : "—");
                color: "#dddddd"; font.pixelSize: 15
            }
            Label {
                text: "Медиан: " + (stats && stats.midScore !== undefined ? stats.midScore.toFixed(1) : "—");
                color: "#dddddd"; font.pixelSize: 15
            }
        }

        ScoreDist {
            Layout.preferredWidth: Math.min(parent.width/2, parent.height/2) + 120
            Layout.preferredHeight: Math.min(parent.width/2, parent.height/2) + 120

            distribution:     stats ? stats.scoreDistribution : []
            firstBucketScore: stats ? stats.scoreBucketStart  : 100
            bucketStep:       stats ? stats.scoreBucketStep   : 10
        }

        Item { Layout.fillWidth: true; Layout.fillHeight: true }
    }
}