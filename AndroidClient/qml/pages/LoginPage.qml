import QtQuick 2.15
import QtQuick.Controls 2.15
import "../components"  // ✅ Импортируем CountrySelector

Rectangle {
    width: parent.width
    height: parent.height
    color: "white"

    Column {
        width: parent.width - 40
        anchors.centerIn: parent
        spacing: 32  // 🔹 Увеличен отступ между элементами

        // 🔹 Заголовок
        Text {
            text: "Welcome Back 👋"
            font.pixelSize: 26
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // 🔹 Подзаголовок
        Text {
            text: "Sign in to continue"
            font.pixelSize: 14
            color: "#666"
            horizontalAlignment: Text.AlignHCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // 🔹 Выбор страны (Увеличенный)
        CountrySelector {
            id: loginCountrySelector
            width: parent.width  // 🔥 Увеличим ширину селектора
            onCountryChanged: (code, flag) => {
                countryCode.text = code;
                console.log("🌍 Selected Country:", code, flag);
            }
        }

        // 🔹 Ввод номера телефона
        Row {
            width: parent.width
            spacing: 10

            // 🔹 Код страны
            Rectangle {
                width: 70  // 🔹 Увеличен блок кода страны
                height: 45
                color: "#f5f5f5"
                border.color: "#ccc"
                border.width: 1
                radius: 5

                Text {
                    id: countryCode
                    text: loginCountrySelector.selectedCountryCode
                    font.pixelSize: 16
                    color: "#000"
                    anchors.centerIn: parent
                }
            }

            // 🔹 Поле ввода номера
            TextField {
                id: phoneInput
                width: parent.width - 80
                height: 45
                placeholderText: "Enter phone number"
                font.pixelSize: 16
                inputMethodHints: Qt.ImhDigitsOnly
                validator: RegularExpressionValidator { regularExpression: /[0-9]{9}/ }

                background: Rectangle {
                    color: "white"
                    border.color: "#ccc"
                    border.width: 1
                    radius: 5
                }
            }
        }

        // 🔹 Кнопка "Next"
        Button {
            id: nextButton
            text: "Next"
            width: parent.width
            height: 50
            enabled: phoneInput.text.length === 9
            background: Rectangle {
                color: nextButton.enabled ? "#0088CC" : "#ccc"
                radius: 5
            }
            contentItem: Text {
                text: nextButton.text
                color: nextButton.enabled ? "white" : "#666"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: {
                let fullPhone = countryCode.text + phoneInput.text;
                console.log("📱 Full Phone Number:", fullPhone);

                if (appWindow.usersDatabase.hasOwnProperty(fullPhone)) {
                    appWindow.goToPasswordPage(fullPhone);  // ✅ Передаем номер телефона
                } else {
                    errorText.visible = true;  // ❌ Номер не найден
                }
            }
        }

        // 🔹 Ошибка
        Text {
            id: errorText
            text: "❌ Невірний номер! Зареєструйтесь."
            color: "red"
            visible: false
            anchors.horizontalCenter: parent.horizontalCenter
        }

        // 🔹 Кнопка "Register"
        Button {
            text: "Register"
            width: parent.width
            height: 50
            background: Rectangle {
                color: "#FF9900"
                radius: 5
            }
            contentItem: Text {
                text: "Register"
                color: "white"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            onClicked: {
                appWindow.goToRegisterPage();
            }
        }
    }
}
