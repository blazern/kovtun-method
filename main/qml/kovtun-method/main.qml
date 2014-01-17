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
            text: "Выполнить шаг"
            action: Action {
                onTriggered: {
                    kovtunMethodExecuterQmlInterface.performNextStep();
                    kovtunMethodExecuterQmlInterface.setUnitsDimension(unitsDimensionTextField.text);
                }
            }
        }

        Button {
            id: resetButton
            anchors.top: performStepButton.bottom
            text: "Сбросить"
            action: Action {
                onTriggered: {
                    kovtunMethodExecuterQmlInterface.reset();
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
                kovtunMethodExecuterQmlInterface.setUnitsDimension(text);
            }

            Component.onCompleted: {
                text = kovtunMethodExecuterQmlInterface.getUnitsDimension();
            }
        }

        KovtunMethodPainter {
            anchors.left: performStepButton.right
            anchors.leftMargin: 10
            height: parent.height
            width: parent.width - x
            objectName: "kovtunMethodPainter"
        }
    }


}
