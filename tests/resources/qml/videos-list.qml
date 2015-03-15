import QtQuick 2.0
import QYouTube 1.0

Rectangle {
    width: 800
    height: 480
    color: "#000"

    Rectangle {
        id: searchBox

        height: 50
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: 10
        }
        color: "#fff"
        border {
            width: 2
            color: "green"
        }

        TextInput {
            id: searchField

            anchors.fill: parent
            onAccepted: searchModel.list("/search", ["snippet"], {}, {type: "video", maxResults: 20, q: text})
        }
    }

    ListView {
        id: view

        anchors {
            left: parent.left
            right: parent.right
            top: searchBox.bottom
            topMargin: 10
            bottom: parent.bottom
        }
        clip: true
        model: ResourcesModel {
            id: searchModel

            apiKey: "AIzaSyAkoHcCKQBi3A3hsYRuOeyw3r25COg4CJo"
            onStatusChanged: if (status == ResourcesRequest.Failed) console.log("SearchModel error: " + errorString);
        }
        delegate: Item {
            width: view.width
            height: 100

            Image {
                id: image

                width: 120
                height: 90
                anchors {
                    left: parent.left
                    leftMargin: 10
                    verticalCenter: parent.verticalCenter
                }
                source: snippet.thumbnails["default"].url
            }

            Column {
                anchors {
                    left: image.right
                    leftMargin: 10
                    right: parent.right
                    rightMargin: 10
                    verticalCenter: parent.verticalCenter
                }
                spacing: 10

                Text {
                    width: parent.width
                    elide: Text.ElideRight
                    color: "#fff"
                    text: snippet.title
                }

                Text {
                    width: parent.width
                    elide: Text.ElideRight
                    color: "#999"
                    text: "By " + snippet.channelTitle
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: streams.list(id.videoId)
            }
        }
    }

    StreamsRequest {
        id: streams

        onFinished: {
            if (status == StreamsRequest.Ready) {
                for (var i = 0; i < result.length; i++) {
                    var stream = result[i];
                    console.log(stream.description + " - " + stream.width + "x" + stream.height + ": " + stream.url);
                }
            }
            else {
                console.log("StreamsRequest error: " + errorString);
            }
        }
    }
}
