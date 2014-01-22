import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Dialogs 1.0
import KovtunMethod 1.0

ApplicationWindow {
    id: window
    title: qsTr("Kovtun method")
    width: 640
    height: 480

    Item {
        id: content
        anchors.fill: parent

        Button {
            id: performStepButton
            anchors.verticalCenter: parent.verticalCenter
            text: "Шаг вперед"
            action: Action {
                onTriggered: {
                    loadingText.visible = true;
                    kovtunMethodExecuter.setUnitsDimension(unitsDimensionTextField.text);
                    kovtunMethodExecuter.performNextStep();
                }
            }
        }

        Button {
            id: resetButton
            anchors.top: performStepButton.bottom
            text: "Сбросить"
            action: Action {
                onTriggered: {
                    kovtunMethodExecuter.reset();
                    errorValue.text = "0";
                }
            }
        }

        Text {
            id: unitsDimensionText
            anchors.top: resetButton.bottom
            text: "Размерность юнитов:"
        }

        TextField {
            id: unitsDimensionTextField
            anchors.top: unitsDimensionText.bottom

            onAccepted: {
                kovtunMethodExecuter.setUnitsDimension(text);
            }

            Component.onCompleted: {
                text = kovtunMethodExecuter.getUnitsDimension();
            }
        }

        KovtunMethodPainter {
            id: kovtunMethodPainter
            anchors.left: performStepButton.right
            anchors.leftMargin: 10
            height: parent.height
            width: parent.width - x
            objectName: "kovtunMethodPainter"
        }

        Text {
            id: loadingText
            text: "Подождите, выполняются расчёты..."
            color: "red"
            font.pointSize: 0.035 * kovtunMethodPainter.width
            anchors.centerIn: kovtunMethodPainter
            visible: false
        }

        Text {
            id: errorText
            text: "погрешность:"
            anchors.bottom: parent.bottom
        }

        Text {
            id: errorValue
            text: "0"
            anchors.left: errorText.right
            y: errorText.y
        }
    }

    KovtunMethodExecuterQmlInterface {
        id: kovtunMethodExecuter
        onExecutionReset: kovtunMethodPainter.update();
        onStepPerformed: {
            kovtunMethodPainter.update();
            loadingText.visible = false;
            errorValue.text = getCurrentError();
        }
        objectName: "kovtunMethodExecuter"
    }
}
