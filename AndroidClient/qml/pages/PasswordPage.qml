import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    width: parent.width
    height: parent.height
    color: "#f5f5f5"

    Column {
        width: parent.width - 40
        anchors.centerIn: parent
        spacing: 20

        // 🔹 Заголовок
        Text {
            text: "Enter your password"
            font.pixelSize: 24
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // 🔹 Поле ввода пароля
        TextField {
            id: passwordInput
            width: parent.width
            height: 45
            placeholderText: "Enter password"
            echoMode: TextInput.Password
            font.pixelSize: 16
            background: Rectangle {
                color: "white"
                border.color: "#ccc"
                border.width: 1
                radius: 5
            }
        }

        // 🔹 Кнопка "Login"
        Button {
            id: loginButton
            text: "Login"
            width: parent.width
            height: 50
            background: Rectangle {
                color: loginButton.hovered ? "#006699" : "#0088CC"
                radius: 5
            }
            contentItem: Text {
                text: loginButton.text
                color: "white"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: {
                let fullPhone = appWindow.currentPhoneNumber;
                console.log("🔑 Trying login for:", fullPhone);

                if (passwordInput.text.length === 0) {
                    incorrectPasswordText.text = "⚠️ Please enter your password.";
                    incorrectPasswordText.visible = true;
                } else if (passwordInput.text === appWindow.usersDatabase[fullPhone]?.password) {
                    console.log("✅ Login successful");
                    incorrectPasswordText.visible = false;
                    // Действие при успешном входе
                } else {
                    incorrectPasswordText.text = "❌ Incorrect password. Please try again.";
                    incorrectPasswordText.visible = true;
                }
            }
        }

        // 🔹 Текст ошибки
        Text {
            id: incorrectPasswordText
            text: ""
            color: "red"
            visible: false
            font.pixelSize: 14
            anchors.horizontalCenter: parent.horizontalCenter

            Behavior on visible {
                NumberAnimation { duration: 200 }
            }
        }

        // 🔹 Кнопка "Back"
        Button {
            text: "Back"
            width: parent.width
            height: 50
            background: Rectangle {
                color: "#FF9900"
                radius: 5
            }
            contentItem: Text {
                text: "Back"
                color: "white"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: appWindow.goToLoginPage
        }
    }
}
