import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 900; height: 650; minimumWidth: 640; minimumHeight: 480
    visible: true; title: "Cuidado PC"
    color: "#f4f6fa"
    ColumnLayout {
        anchors.fill: parent; anchors.margins: 24; spacing: 14
        Label { text: "Cuidado PC"; font.pixelSize: 28; font.bold: true; color: "#17233a" }
        Label { text: "Diagnóstico e limpeza para Windows e macOS"; color: "#52617b" }
        Frame {
            Layout.fillWidth: true
            ColumnLayout {
                anchors.fill: parent
                Label { text: "Diagnóstico"; font.bold: true; font.pixelSize: 18 }
                Label { text: backend.diagnostic; wrapMode: Text.WrapAnywhere; Layout.fillWidth: true }
                Button { text: "Atualizar diagnóstico"; onClicked: backend.refresh() }
            }
        }
        RowLayout {
            Layout.fillWidth: true
            Label { text: "Arquivos temporários"; font.bold: true; font.pixelSize: 18; Layout.fillWidth: true }
            Button { text: "Analisar temporários"; onClicked: backend.scan() }
            Button { text: "Limpar arquivos listados"; enabled: backend.count > 0; onClicked: confirmDialog.open() }
        }
        Label { text: backend.summary; wrapMode: Text.Wrap; Layout.fillWidth: true }
        Label { text: "Apenas arquivos diretamente na pasta temporária, com mais de 7 dias. Revise a lista antes de limpar."; wrapMode: Text.Wrap; Layout.fillWidth: true; color: "#52617b" }
        Frame {
            Layout.fillWidth: true; Layout.fillHeight: true; padding: 0
            ListView {
                anchors.fill: parent; clip: true; model: backend.files
                ScrollBar.vertical: ScrollBar { }
                delegate: ItemDelegate {
                    required property var modelData
                    width: ListView.view.width
                    contentItem: Label { text: modelData.size + "   " + modelData.path; elide: Text.ElideMiddle }
                }
                Label { anchors.centerIn: parent; visible: backend.count === 0; text: "Nenhum arquivo listado"; color: "#7b8798" }
            }
        }
    }
    Dialog {
        id: confirmDialog; title: "Confirmar limpeza"; modal: true
        anchors.centerIn: parent; standardButtons: Dialog.Yes | Dialog.No
        Label { width: 420; wrapMode: Text.Wrap; text: "Excluir permanentemente os " + backend.count + " arquivos listados? Os arquivos não serão enviados para a lixeira." }
        onAccepted: backend.clean()
    }
}
