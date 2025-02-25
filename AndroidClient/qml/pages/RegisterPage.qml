// qml/pages/RegisterPage.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import "../components"  // ✅ Импортируем CountrySelector

Rectangle {
    width: parent.width
    height: parent.height
    color: "#f9f9f9"  // ✅ Светлый фон

    Column {
        width: parent.width - 40
        anchors.centerIn: parent
        spacing: 16

        // 🔥 Заголовок
        Text {
            text: "Register"
            font.pixelSize: 24
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // 🔹 Поле ввода имени
        TextField {
            id: usernameInput
            width: parent.width
            placeholderText: "Enter your name"
            font.pixelSize: 16
            background: Rectangle { color: "white"; border.color: "#ccc"; border.width: 1; radius: 5 }
        }

        // 🔹 CountrySelector для выбора страны
        CountrySelector {
            id: registerCountrySelector

            onCountryChanged: (code, flag) => {
                countryCode.text = code;
                console.log("🌍 Selected Country:", code, flag);
            }
        }

        // 🔹 Ввод номера телефона с кодом страны
        Row {
            width: parent.width
            spacing: 10

            Rectangle {
                width: 60
                height: 40
                color: "transparent"
                border.color: "#ccc"
                border.width: 1
                radius: 5

                Text {
                    id: countryCode
                    text: registerCountrySelector.selectedCountryCode
                    font.pixelSize: 16
                    color: "#000"
                    anchors.centerIn: parent
                }
            }

            TextField {
                id: phoneRegisterInput
                width: parent.width - 70
                height: 40
                placeholderText: "Enter phone number"
                font.pixelSize: 16
                inputMethodHints: Qt.ImhDigitsOnly
                validator: RegularExpressionValidator { regularExpression: /[0-9]{9}/ }
                background: Rectangle { color: "white"; border.color: "#ccc"; border.width: 1; radius: 5 }
            }
        }

        // 🔹 Ввод пароля
        TextField {
            id: passwordRegisterInput
            width: parent.width
            height: 40
            placeholderText: "Create password"
            echoMode: TextInput.Password
            font.pixelSize: 16
            background: Rectangle { color: "white"; border.color: "#ccc"; border.width: 1; radius: 5 }
        }

        // 🔹 Сообщение об ошибке
        Text {
            id: registerError
            text: "❌ This number is already registered!"
            color: "red"
            visible: false
            font.pixelSize: 14
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // 🔹 Кнопка "Sign Up"
        Button {
            text: "Sign Up"
            width: parent.width
            height: 50
            background: Rectangle { color: "#0088CC"; radius: 5 }
            contentItem: Text { text: "Sign Up"; color: "white"; font.pixelSize: 16; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }

            onClicked: {
                let fullPhone = countryCode.text + phoneRegisterInput.text;
                console.log("Registering:", usernameInput.text, fullPhone);

                if (appWindow.usersDatabase.hasOwnProperty(fullPhone)) {
                    registerError.visible = true;  // ❌ Номер уже зарегистрирован
                } else {
                    appWindow.usersDatabase[fullPhone] = {
                        name: usernameInput.text,
                        password: passwordRegisterInput.text
                    };
                    console.log("✅ User registered:", appWindow.usersDatabase);
                    appWindow.goToLoginPage();  // 🔥 Возвращаемся на страницу логина
                }
            }
        }

        // 🔹 Кнопка "Back"
        // 🔹 Кнопка "Back"
        Button {
            text: "Back"
            width: parent.width
            height: 50
            background: Rectangle { color: "#FF9900"; radius: 5 }
            contentItem: Text { text: "Back"; color: "white"; font.pixelSize: 16; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }

            // ✅ Исправленный обработчик
            onClicked: {
                console.log("🔙 Going back to LoginPage");
                if (typeof appWindow.goToLoginPage === "function") {
                    appWindow.goToLoginPage();  // ✅ Вернуться через метод
                } else {
                    // ✅ Альтернатива: загружаем страницу напрямую
                    pageLoader.source = "qml/pages/LoginPage.qml";
                }
            }
        }
    }
}
