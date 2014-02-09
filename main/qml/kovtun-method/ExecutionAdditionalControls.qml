import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

RowLayout {
    id: controls
    property int unitsDimension: 50
    property bool filledRectanglesLinesVisible: false
    property bool gravityCentersVisible: false
    property bool unitsVisible: false

    function invalidate() {
        controls.unitsDimension = unitsDimensionText.text;
    }

    Row {
        spacing: 10
        Text {
            text: "Размерность юнитов:"
        }

        TextField {
            id: unitsDimensionText
            text: controls.unitsDimension
            onAccepted: {
                controls.unitsDimension = text;
            }
        }
    }

    CheckBox {
        text: "Видимые линии"
        onClicked: {
            filledRectanglesLinesVisible = checked;
        }
    }

    CheckBox {
        text: "Видимые грав. центры"
        onClicked: {
            gravityCentersVisible = checked;
        }
    }

    CheckBox {
        text: "Показать юниты"
        onClicked: {
            unitsVisible = checked;
        }
    }
}
