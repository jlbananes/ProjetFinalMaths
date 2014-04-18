import QtQuick 2.0
import OpenGLUnderQML 1.0

Item {

    width : 7680
    height : 4320
    property int xmove: 0
    property int ymove: 0
    property int clickedButton: 0

    Squircle {
        id: squircle
        function update()
        {
            //squircle.t += 1/600;
            squircle.x = xmove;
            squircle.y = ymove;
            squircle.clickedButton = clickedButton;
        }
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
        z:2
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
        z:2 //On front
        id: label
        color: "black"
        wrapMode: Text.WordWrap
        text: "A Wonderful Cube"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 20
    }

    MouseArea {
        z:1 //On the background
        id: mouseNav
        hoverEnabled: true
        anchors.fill: parent
        property int clickedX: 0
        property int clickedY: 0

        onPositionChanged: {
            if (clickedButton == 1)
            {
                xmove = mouseX - clickedX;
                ymove = -(mouseY - clickedY);

                label.text = "déplacement en X :" + xmove + "  déplacement en y : " + ymove;
            }

            else
            {
                label.text = " X: " + mouseX + " Y: " + mouseY;
            }
        }
        onPressed: {label.text = "clic souris"; clickedButton = !clickedButton; clickedX = mouseX; clickedY = mouseY; }
        onReleased: {clickedButton = 0;}
    }
}
