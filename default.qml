import QtQuick
/*import QtQuick.Controls*/
import QtQuick.Particles
import Neospoorkl 1.0

Window {
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    visible: true
    title: qsTr("uwu owo")
    color: "transparent"
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.WindowTransparentForInput // | Qt.WindowDoesNotAcceptFocus

    Item {
        id: root
        anchors.fill: parent

        NeospoorklAPI {
            id: spoorkl
        }

        Timer {
            interval: 1
            running: true
            repeat: true
            onTriggered: {
                spoorkl.updateCursorPosition()
            }
        }

        ParticleSystem {
            id: particleSystem
        }

        Emitter {
            id: emitter
            system: particleSystem

            width: 8
            height: 8

            emitRate: 500
            lifeSpan: 2000
            lifeSpanVariation: 300

            size: 40
            endSize: 2

            x: spoorkl.globalPosition.x
            y: spoorkl.globalPosition.y

            // Behavior on x { NumberAnimation { duration: 20 } }
            // Behavior on y { NumberAnimation { duration: 20 } }

            // velocity: AngleDirection {angleVariation: 180; magnitudeVariation: 20;}
            // acceleration: AngleDirection {angleVariation: 180; magnitudeVariation: 20;}
            velocityFromMovement: 3
        }

        Gravity {
            system: particleSystem
            magnitude: 50
            angle: 90
        }

        Wander {
            system: particleSystem
            xVariance: 200
            yVariance: 200
        }

        Friction {
            system: particleSystem
            factor: 0.8
        }

        ImageParticle {
            source: "qrc:///particleresources/star.png"
            system: particleSystem
            alpha: 0
            SequentialAnimation on color {
                loops: Animation.Infinite
                ColorAnimation { from: "magenta"; to: "red";     duration: 1000 }
                ColorAnimation { from: "red";     to: "yellow";  duration: 1000 }
                ColorAnimation { from: "yellow";  to: "green";   duration: 1000 }
                ColorAnimation { from: "green";   to: "cyan";    duration: 1000 }
                ColorAnimation { from: "cyan";    to: "blue";    duration: 1000 }
                ColorAnimation { from: "blue";    to: "magenta"; duration: 1000 }
            }
            colorVariation: 0.3
        }
    }
}
