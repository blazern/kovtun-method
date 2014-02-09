import QtQuick 2.1
import QtQuick.Window 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Window {
    id: window
    title: qsTr("Данные")
    width: content.width
    height: content.height

    property double error: 0
    property int activeRectanglesCount: 0
    property int filledRectanglesCount: 0
    property int usedColorsCount: 0

    MouseArea {
        anchors.fill: parent
        onClicked: {
            window.visible = false;
        }
    }

    Item {
        id: content
        width: childrenRect.width
        height: childrenRect.height

        ColumnLayout {
            ExecutionDataItem {
                dataDescription: "погрешность вычисления центров тяжести:"
                dataValue: error
            }

            ExecutionDataItem {
                dataDescription: "число незакрашенных прямоугольников:"
                dataValue: activeRectanglesCount
            }

            ExecutionDataItem {
                dataDescription: "число незакрашенных прямоугольников:"
                dataValue: filledRectanglesCount
            }

            ExecutionDataItem {
                dataDescription: "число использованных цветов:"
                dataValue: usedColorsCount
            }
        }
    }
}
