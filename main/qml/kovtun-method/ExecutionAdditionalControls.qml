import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Item {
    id: controls
    property int unitsDimension: 50
    property bool filledRectanglesLinesVisible: false
    property bool gravityCentersVisible: false
    property bool unitsVisible: false

    function invalidate() {
        controls.unitsDimension = unitsDimensionText.text;
    }

    function onStepStartedPerforming() {
        unitsDimensionCalculator.onStepStartedPerforming();
    }

    function onStepPerformed() {
        unitsDimensionCalculator.onStepPerformed();
    }

    RowLayout {
        id: firstLayout

        Item {
            id: unitsDimensionCalculator
            property var stepStartingTime

            function onStepStartedPerforming() {
                stepStartingTime = (new Date()).getTime();
            }

            function onStepPerformed() {
                if (
                        secondsForExecutionElement.available
                        && controls.unitsDimension > 1
                        && secondsForExecutionCountTextField.text > 0) {
                    var msLimit = secondsForExecutionCountTextField.text * 1000;
                    var currentTime = (new Date()).getTime();

                    if (currentTime - stepStartingTime > msLimit) {
                        controls.unitsDimension /= 2;
                        unitsDimensionText.text = controls.unitsDimension;
                    }
                }
            }
        }

        Row {
            spacing: 10
            Text {
                anchors.verticalCenter: parent.verticalCenter
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

    RowLayout {
        id: secondLayout
        anchors.top: firstLayout.bottom
        anchors.topMargin: 10

        CheckBox {
            id: secondsForExecutionCheckBox
            text: "Уменьшать размерность юнитов при долгом выполнении шага"
        }

        Row {
            id: secondsForExecutionElement
            spacing: 10
            visible: secondsForExecutionCheckBox.checked
            property bool available: visible

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Предельное время выполнения шага:"
            }

            TextField {
                id: secondsForExecutionCountTextField
            }
        }
    }
}
