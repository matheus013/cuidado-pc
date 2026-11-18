import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 900; height: 680; minimumWidth: 700; minimumHeight: 540
    visible: true; title: "Cuidado PC"
    color: "#f3f6fb"
    palette.window: "#f3f6fb"
    palette.windowText: "#17283f"
    palette.text: "#17283f"
    palette.buttonText: "#17283f"
    palette.button: "#ffffff"
    palette.base: "#ffffff"
    palette.highlight: "#2863d9"
    palette.highlightedText: "#ffffff"

    component ActionButton: Button {
        id: control
        property bool primary: false
        implicitHeight: 40
        implicitWidth: Math.max(145, label.implicitWidth + 32)
        padding: 12
        contentItem: Text {
            id: label
            text: control.text
            color: !control.enabled ? "#9aa8b9" : control.primary ? "#ffffff" : "#254466"
            font.pixelSize: 13; font.weight: Font.DemiBold
            horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter
        }
        background: Rectangle {
            radius: 8
            color: !control.enabled ? "#e8edf4" : control.primary ? (control.down ? "#1948aa" : control.hovered ? "#2257c0" : "#2863d9") : (control.hovered ? "#edf3fc" : "#ffffff")
            border.color: control.primary ? "transparent" : "#d4deeb"
        }
    }
    component Card: Rectangle { color: "#ffffff"; radius: 12; border.color: "#dce5f0" }
    ColumnLayout {
        anchors.fill: parent; anchors.margins: 28; spacing: 16
        RowLayout {
            Layout.fillWidth: true
            ColumnLayout {
                spacing: 4
                Label { text: "Cuidado PC"; font.pixelSize: 28; font.bold: true; color: "#14263f" }
                Label { text: "Seu computador, mais organizado."; font.pixelSize: 13; color: "#61738a" }
            }
            Item { Layout.fillWidth: true }
            Rectangle {
                implicitWidth: statusLabel.implicitWidth + 22; implicitHeight: 30; radius: 15; color: "#e4f5ec"
                Label { id: statusLabel; anchors.centerIn: parent; text: "●  PRONTO"; font.pixelSize: 10; font.bold: true; color: "#207d53" }
            }
        }
        Card {
            Layout.fillWidth: true; implicitHeight: diagColumn.implicitHeight + 36
            ColumnLayout {
                id: diagColumn; anchors.fill: parent; anchors.margins: 18; spacing: 10
                RowLayout {
                    Layout.fillWidth: true
                    ColumnLayout {
                        spacing: 3
                        Label { text: "Diagnóstico"; font.pixelSize: 17; font.bold: true; color: "#17283f" }
                        Label { text: "Informações básicas deste computador"; font.pixelSize: 12; color: "#687b91" }
                    }
                    Item { Layout.fillWidth: true }
                    ActionButton { text: "Atualizar"; onClicked: backend.refresh() }
                }
                Rectangle { Layout.fillWidth: true; implicitHeight: 1; color: "#e8edf4" }
                Label { text: backend.diagnostic; wrapMode: Text.WrapAnywhere; Layout.fillWidth: true; lineHeight: 1.35; color: "#344b65"; font.pixelSize: 13 }
            }
        }
        RowLayout {
            Layout.fillWidth: true; spacing: 10
            ColumnLayout {
                Layout.fillWidth: true; spacing: 3
                Label { text: "Arquivos temporários"; font.pixelSize: 18; font.bold: true; color: "#17283f" }
                Label { text: "Revise antes de excluir"; font.pixelSize: 12; color: "#687b91" }
            }
            ActionButton { text: "Analisar temporários"; primary: true; onClicked: backend.scan() }
            ActionButton { text: "Limpar arquivos"; enabled: backend.count > 0; onClicked: confirmDialog.open() }
        }
        Card {
            Layout.fillWidth: true; Layout.fillHeight: true
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 18; spacing: 10
                Label { text: backend.summary; wrapMode: Text.Wrap; Layout.fillWidth: true; font.bold: true; font.pixelSize: 13; color: "#263e5b" }
                Label { text: "Somente arquivos diretamente na pasta temporária, com mais de 7 dias."; wrapMode: Text.Wrap; Layout.fillWidth: true; font.pixelSize: 12; color: "#687b91" }
                Rectangle { Layout.fillWidth: true; implicitHeight: 1; color: "#e8edf4" }
                ListView {
                    Layout.fillWidth: true; Layout.fillHeight: true; clip: true; model: backend.files
                    ScrollBar.vertical: ScrollBar { }
                    delegate: ItemDelegate {
                        required property var modelData
                        width: ListView.view.width
                        contentItem: Label { text: modelData.size + "   " + modelData.path; color: "#344b65"; elide: Text.ElideMiddle }
                        background: Rectangle { color: parent.hovered ? "#f3f7fd" : "transparent"; radius: 6 }
                    }
                    Label { anchors.centerIn: parent; visible: backend.count === 0; text: "Nenhum arquivo listado"; font.pixelSize: 13; color: "#7b8da4" }
                }
            }
        }
        Label { text: "A exclusão é permanente e não envia arquivos para a lixeira."; font.pixelSize: 11; color: "#687b91"; Layout.fillWidth: true; wrapMode: Text.Wrap }
    }
    Dialog {
        id: confirmDialog; title: "Confirmar limpeza"; modal: true
        anchors.centerIn: parent; standardButtons: Dialog.Yes | Dialog.No
        Label { width: 420; wrapMode: Text.Wrap; color: "#17283f"; text: "Excluir permanentemente os " + backend.count + " arquivos listados? Os arquivos não serão enviados para a lixeira." }
        onAccepted: backend.clean()
    }
}
