import QtQuick 2.0
import OpenGLUnderQML 1.0

Item {

    width: 1350
    height: 690

    Squircle {
        id: squircle
        function update()
        {
            squircle.t += 1/600;
        }
        /*
        SequentialAnimation on t {
            NumberAnimation { from: 0; to: 1; duration: 2500}
            NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
            loops: Animation.Infinite
            running: true
        }
        */
    }

    Timer {
        id: gameTimer
        interval: 1
        running: true
        repeat: true
        triggeredOnStart: true
        onTriggered:
        {
            squircle.update();
        }
    }

    Rectangle {
        color: Qt.rgba(1, 1, 1, 0.7)
        radius: 0
        border.width: 1
        border.color: "white"
        anchors.fill: label
        anchors.margins: -10
        MouseArea {
            id: button
            hoverEnabled: true
            anchors.fill: parent
            onClicked: { Qt.quit(); }
        }
    }

    Text {
        id: label
        color: "black"
        wrapMode: Text.WordWrap
        text: "The background here is a squircle rendered with raw OpenGL using the 'beforeRender()' signal in QQuickWindow. This text label and its border is rendered using QML"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }


}
