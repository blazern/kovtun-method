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
        anchors.topMargin: 5
        anchors.rightMargin: 5
        anchors.bottomMargin: 5
        anchors.leftMargin: 5

        Item {
            id: controls
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            height: parent.height / 8

            Button {
                id: performStepButton
                anchors.left: parent.left
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
                anchors.left: performStepButton.right
                anchors.leftMargin: 10
                text: "Сбросить"
                action: Action {
                    onTriggered: {
                        kovtunMethodExecuter.reset();
                        errorValue.text = "0";
                        activeRectanglesCountValue.text = "0";
                        filledRectanglesCountValue.text = "0";
                    }
                }
            }

            Text {
                id: unitsDimensionText
                anchors.left: resetButton.right
                anchors.leftMargin: 40
                anchors.verticalCenter: resetButton.verticalCenter
                text: "Размерность юнитов:"
            }

            TextField {
                id: unitsDimensionTextField
                anchors.left: unitsDimensionText.right
                anchors.leftMargin: 10
                onAccepted: {
                    kovtunMethodExecuter.setUnitsDimension(text);
                }
                Component.onCompleted: {
                    text = kovtunMethodExecuter.getUnitsDimension();
                }
            }

            Text {
                id: errorText
                text: "погрешность вычисления центров тяжести:"
                anchors.top: performStepButton.bottom
                anchors.topMargin: 10
            }

            Text {
                id: errorValue
                text: "0"
                anchors.left: errorText.right
                anchors.leftMargin: 10
                anchors.verticalCenter: errorText.verticalCenter
            }

            Text {
                id: activeRectanglesCountText
                text: "число незаграшенных прямоугольников:"
                anchors.top: errorText.bottom
            }

            Text {
                id: activeRectanglesCountValue
                text: "0"
                anchors.left: activeRectanglesCountText.right
                anchors.leftMargin: 10
                anchors.verticalCenter: activeRectanglesCountText.verticalCenter
            }

            Text {
                id: filledRectanglesCountText
                text: "число заграшенных прямоугольников:"
                anchors.top: activeRectanglesCountText.bottom
            }

            Text {
                id: filledRectanglesCountValue
                text: "0"
                anchors.left: filledRectanglesCountText.right
                anchors.leftMargin: 10
                anchors.verticalCenter: filledRectanglesCountText.verticalCenter
            }

        }

        Item {
            id: result
            anchors.top: controls.bottom
            anchors.topMargin: 10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            KovtunMethodPainter {
                id: kovtunMethodPainter
                anchors.fill: parent
                anchors.topMargin: 5
                anchors.rightMargin: 5
                anchors.bottomMargin: 5
                anchors.leftMargin: 5
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
        }
    }

    KovtunMethodExecuterQmlInterface {
        id: kovtunMethodExecuter
        onExecutionReset: {
            kovtunMethodPainter.update();
        }
        onStepPerformed: {
            kovtunMethodPainter.update();
            loadingText.visible = false;
            errorValue.text = getCurrentError();
            activeRectanglesCountValue.text = getActiveRectanglesCount();
            filledRectanglesCountValue.text = getFilledRectanglesCount();
        }
        objectName: "kovtunMethodExecuter"
    }
}
