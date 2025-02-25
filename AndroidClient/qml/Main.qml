// Main.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: appWindow
    visible: true
    width: 360
    height: 640
    title: "Вход по номеру телефона"

    // 🔹 Локальная база пользователей
    property var usersDatabase: ({})
    property string lastRegisteredPhone: ""
    property string lastRegisteredCountryCode: ""
    property string currentPhoneNumber: ""  // ✅ Хранит номер для проверки пароля

    // 🔹 Loader для загрузки страниц
    Loader {
        id: pageLoader
        anchors.fill: parent
        source: "qrc:/pages/LoginPage.qml"  // ✅ Начинаем с LoginPage

        onLoaded: {
            console.log("✅ Loaded page:", pageLoader.source);
        }

        onStatusChanged: {
            if (pageLoader.status === Loader.Error) {
                console.error("❌ Failed to load:", pageLoader.source, pageLoader.errorString());
            }
        }
    }

    // 🔹 Функции для переходов между страницами
    function goToLoginPage() {
        console.log("🔄 Navigating to LoginPage");
        pageLoader.source = "qrc:/pages/LoginPage.qml";
    }

    function goToRegisterPage() {
        console.log("🔄 Navigating to RegisterPage");
        pageLoader.source = "qrc:/pages/RegisterPage.qml";
    }

    function goToPasswordPage(phoneNumber) {  // ✅ Принимает номер телефона
        currentPhoneNumber = phoneNumber;
        console.log("🔄 Navigating to PasswordPage for:", phoneNumber);
        pageLoader.source = "qrc:/pages/PasswordPage.qml";
    }
}
