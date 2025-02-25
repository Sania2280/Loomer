// qml/components/CountrySelector.qml
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    width: parent.width
    height: 55

    property string selectedCountryCode: "+380"
    property string selectedFlag: "qrc:/new/prefix1/images/640px-Flag_of_Ukraine.svg.png"
    property string selectedCountryName: "Ukraine"

    signal countryChanged(string code, string flag)

    Row {
        spacing: 16  // 🔥 Больше пространства между флагом и ComboBox
        anchors.verticalCenter: parent.verticalCenter

        // 🔥 Увеличенный флаг с тенью
        Rectangle {
            width: 60  // ⬆ Увеличенный размер
            height: 40
            radius: 3
            color: "transparent"
            border.color: "#ddd"
            border.width: 1

            Image {
                id: countryFlag
                source: root.selectedFlag
                anchors.centerIn: parent
                width: parent.width * 0.9
                height: parent.height * 0.9
                fillMode: Image.PreserveAspectFit
            }

            // 🌟 Легкая тень
            Rectangle {
                anchors.fill: parent
                color: "black"
                opacity: 0.1
                radius: 3
                z: -1
                anchors.horizontalCenterOffset: 2
                anchors.verticalCenterOffset: 2
            }
        }

        // 🌟 ComboBox для выбора страны
        ComboBox {
            id: countryComboBox
            width: root.width - 80
            height: 40  // 🔥 Сделали выше для выравнивания
            model: [
                { name: " Ukraine", code: "+380", flag: "qrc:/new/prefix1/images/640px-Flag_of_Ukraine.svg.png" },
                { name: " Germany", code: "+49", flag: "qrc:/new/prefix1/images/Flag_of_Germany.svg" },
                { name: " Poland", code: "+48", flag: "qrc:/new/prefix1/images/Flag_of_Poland.svg.webp" }
            ]
            textRole: "name"
            contentItem: Text {
                text: countryComboBox.displayText
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
                color: "#000"
            }

            // 🌟 Более контрастный фон
            background: Rectangle {
                color: "#f0f0f0"  // ✅ Светло-серый фон для контраста
                border.color: "#bbb"
                border.width: 1
                radius: 5
            }

            // 🌟 Изменение выбранной страны
            onCurrentIndexChanged: {
                const selected = model[currentIndex];
                root.selectedCountryCode = selected.code;
                root.selectedFlag = selected.flag;
                root.selectedCountryName = selected.name;
                countryFlag.source = selected.flag;

                // 🚀 Передаем изменения
                root.countryChanged(selected.code, selected.flag);
                console.log("🌍 Country selected:", selected.name, selected.code);
            }
        }
    }
}
