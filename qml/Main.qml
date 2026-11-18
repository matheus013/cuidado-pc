import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1040; height: 760; minimumWidth: 960; minimumHeight: 600
    visible: true; title: "Cuidado PC"; color: "#f3f6fb"
    palette.window: "#f3f6fb"; palette.windowText: "#17283f"; palette.text: "#17283f"
    palette.buttonText: "#17283f"; palette.button: "#ffffff"; palette.base: "#ffffff"
    palette.highlight: "#2863d9"; palette.highlightedText: "#ffffff"
    component ActionButton: Button {
        id: control
        property bool primary: false
        implicitHeight: 40; implicitWidth: Math.max(120, buttonLabel.implicitWidth + 28); padding: 12
        contentItem: Text { id: buttonLabel; text: control.text; color: !control.enabled ? "#93a3b5" : control.primary ? "#ffffff" : "#254466"; font.pixelSize: 13; font.weight: Font.DemiBold; horizontalAlignment: Text.AlignHCenter; verticalAlignment: Text.AlignVCenter }
        background: Rectangle { radius: 8; color: !control.enabled ? "#e8edf4" : control.primary ? (control.down ? "#1948aa" : control.hovered ? "#2257c0" : "#2863d9") : (control.hovered ? "#edf3fc" : "#ffffff"); border.color: control.primary ? "transparent" : "#d4deeb" }
    }
    component Card: Rectangle { color: "#ffffff"; radius: 12; border.color: "#dce5f0" }
    property int currentPage: 0
    RowLayout {
        anchors.fill: parent; spacing: 0
        Rectangle {
            Layout.fillHeight: true; Layout.preferredWidth: 205; color: "#182940"
            ColumnLayout {
                anchors.fill: parent; anchors.margins: 18; spacing: 9
                Label { text: "CUIDADO PC"; font.pixelSize: 17; font.bold: true; color: "#ffffff"; Layout.topMargin: 12; Layout.bottomMargin: 25 }
                Repeater {
                    model: ["Visão e limpeza", "Sistema e atualizações", "Inicialização"]
                    delegate: Button {
                        required property int index
                        required property string modelData
                        text: modelData
                        Layout.fillWidth: true; implicitHeight: 45
                        contentItem: Text { text: modelData; color: window.currentPage === index ? "#ffffff" : "#b5c4d6"; font.pixelSize: 13; font.bold: window.currentPage === index; verticalAlignment: Text.AlignVCenter; leftPadding: 12 }
                        background: Rectangle { radius: 7; color: window.currentPage === index ? "#2c65bc" : parent.hovered ? "#263d59" : "transparent" }
                        onClicked: window.currentPage = index
                    }
                }
                Item { Layout.fillHeight: true }
                Label { text: "Protótipo · Qt / C++"; font.pixelSize: 11; color: "#9aadc4" }
            }
        }
        ColumnLayout {
        Layout.fillWidth: true; Layout.fillHeight: true; Layout.margins: 24; spacing: 13
        RowLayout {
            Layout.fillWidth: true
            ColumnLayout {
                spacing: 3
                Label { text: "Cuidado PC"; font.pixelSize: 26; font.bold: true; color: "#14263f" }
                Label { text: "Diagnóstico e manutenção do seu computador"; font.pixelSize: 12; color: "#61738a" }
            }
            Item { Layout.fillWidth: true }
            ActionButton { text: "Atualizar diagnóstico"; onClicked: backend.refresh() }
        }
        Card {
            Layout.fillWidth: true; implicitHeight: diag.implicitHeight + 28
            Label { id: diag; anchors.fill: parent; anchors.margins: 14; text: backend.diagnostic; wrapMode: Text.WrapAnywhere; color: "#344b65"; font.pixelSize: 12; lineHeight: 1.25 }
        }
        StackLayout {
            Layout.fillWidth: true; Layout.fillHeight: true; currentIndex: window.currentPage
            ColumnLayout {
                spacing: 10
                RowLayout {
                    Layout.fillWidth: true
                    ColumnLayout {
                        Layout.fillWidth: true; spacing: 3
                        Label { text: "Limpeza de temporários"; font.pixelSize: 18; font.bold: true; color: "#17283f" }
                        Label { text: "Escolha os arquivos que deseja remover"; font.pixelSize: 12; color: "#687b91" }
                    }
                    ActionButton { text: "Verificação rápida"; enabled: !backend.scanning; onClicked: backend.quickScan() }
                    ActionButton { text: backend.scanning ? "Analisando…" : "Análise de temporários"; primary: true; enabled: !backend.scanning; onClicked: backend.scan() }
                    ActionButton { text: "Excluir selecionados"; enabled: backend.selectedCount > 0 && !backend.scanning; onClicked: confirmDialog.open() }
                }
                Card {
                    Layout.fillWidth: true; Layout.fillHeight: true
                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 16; spacing: 8
                        Label { text: backend.summary; wrapMode: Text.Wrap; Layout.fillWidth: true; color: "#263e5b"; font.bold: true; font.pixelSize: 12 }
                        Label { text: "Arquivos temporários com mais de 7 dias · não selecionados por padrão"; wrapMode: Text.Wrap; Layout.fillWidth: true; color: "#687b91"; font.pixelSize: 12 }
                        RowLayout {
                            Layout.fillWidth: true
                            ActionButton { text: "Selecionar todos"; enabled: backend.count > 0; onClicked: backend.selectAll(true) }
                            ActionButton { text: "Desmarcar"; enabled: backend.selectedCount > 0; onClicked: backend.selectAll(false) }
                            Item { Layout.fillWidth: true }
                            Label { text: backend.selectedCount + " selecionados · " + backend.selectedSize; color: "#344b65"; font.pixelSize: 12 }
                        }
                        Rectangle { Layout.fillWidth: true; implicitHeight: 1; color: "#e8edf4" }
                        ListView {
                            id: fileList; Layout.fillWidth: true; Layout.fillHeight: true; clip: true; model: backend.files
                            ScrollBar.vertical: ScrollBar { }
                            delegate: Item {
                                required property var modelData
                                width: fileList.width; height: 45
                                CheckBox {
                                    anchors.left: parent.left; anchors.verticalCenter: parent.verticalCenter
                                    checked: modelData.selected
                                    onClicked: backend.setSelected(modelData.index, checked)
                                }
                                Label { anchors.left: parent.left; anchors.leftMargin: 42; anchors.right: details.left; anchors.rightMargin: 10; anchors.verticalCenter: parent.verticalCenter; text: modelData.path; color: "#344b65"; elide: Text.ElideMiddle; font.pixelSize: 12 }
                                Label { id: details; anchors.right: parent.right; anchors.rightMargin: 16; anchors.verticalCenter: parent.verticalCenter; text: modelData.size + "  ·  " + modelData.date; color: "#61738a"; font.pixelSize: 12 }
                                Rectangle { anchors.bottom: parent.bottom; width: parent.width; height: 1; color: "#eff2f7" }
                            }
                            Label { anchors.centerIn: parent; visible: backend.count === 0; text: backend.scanning ? "Análise em andamento…" : "Nenhum arquivo listado"; color: "#7b8da4" }
                        }
                    }
                }
                Label { text: "A exclusão é permanente e não usa a lixeira. Arquivos temporários podem estar em uso; revise cada item."; color: "#687b91"; font.pixelSize: 11; Layout.fillWidth: true; wrapMode: Text.Wrap }
            }
            ColumnLayout {
                spacing: 10
                Label { text: "Sistema e atualizações"; color: "#17283f"; font.bold: true; font.pixelSize: 18 }
                Label { text: "Verificações sob demanda, sem reparo ou instalação automática."; color: "#687b91"; font.pixelSize: 12; wrapMode: Text.Wrap; Layout.fillWidth: true }
                RowLayout {
                    Layout.fillWidth: true
                    ActionButton { text: "Verificar sistema"; primary: true; enabled: !backend.running; onClicked: backend.verifySystem() }
                    ActionButton { text: "Buscar atualizações"; enabled: !backend.running; onClicked: backend.checkUpdates() }
                    ActionButton { text: "Cancelar"; enabled: backend.running; onClicked: backend.cancelTask() }
                    Item { Layout.fillWidth: true }
                    BusyIndicator { running: backend.running; visible: running; implicitWidth: 28; implicitHeight: 28 }
                }
                Label { text: "Windows: SFC verifica arquivos protegidos, incluindo DLLs do sistema, e pode exigir administrador; winget lista atualizações de apps disponíveis em suas fontes. macOS: verifica o volume e consulta atualizações do sistema; não verifica DLLs ou todos os apps."; color: "#526781"; font.pixelSize: 12; wrapMode: Text.Wrap; Layout.fillWidth: true }
                Card {
                    Layout.fillWidth: true; Layout.fillHeight: true
                    ColumnLayout {
                        anchors.fill: parent; anchors.margins: 16
                        Label { text: backend.taskTitle; color: "#17283f"; font.bold: true; font.pixelSize: 14; Layout.fillWidth: true; wrapMode: Text.Wrap }
                        ScrollView {
                            Layout.fillWidth: true; Layout.fillHeight: true; clip: true
                            TextArea { text: backend.taskOutput; readOnly: true; selectByMouse: true; wrapMode: TextEdit.Wrap; color: "#344b65"; font.pixelSize: 12; background: Rectangle { color: "#f7f9fc"; radius: 6 } }
                        }
                    }
                }
            }
            ColumnLayout {
                spacing: 14
                Label { text: "Inicialização"; color: "#17283f"; font.bold: true; font.pixelSize: 20 }
                Label { text: "Abra o Cuidado PC ao entrar na sua conta e veja o resultado de uma verificação rápida."; color: "#526781"; font.pixelSize: 13; wrapMode: Text.Wrap; Layout.fillWidth: true }
                Card {
                    Layout.fillWidth: true; implicitHeight: startupContents.implicitHeight + 36
                    ColumnLayout {
                        id: startupContents; anchors.fill: parent; anchors.margins: 18; spacing: 12
                        RowLayout {
                            Layout.fillWidth: true
                            ColumnLayout {
                                Layout.fillWidth: true
                                Label { text: "Iniciar com o sistema"; color: "#17283f"; font.bold: true; font.pixelSize: 15 }
                                Label { text: "Executa ao entrar na conta deste usuário"; color: "#687b91"; font.pixelSize: 12 }
                            }
                            ActionButton { text: backend.autoStart ? "Desativar" : "Ativar"; primary: !backend.autoStart; onClicked: backend.setAutoStart(!backend.autoStart) }
                        }
                        Label { text: backend.autoStart ? "Ativado" : "Desativado"; color: backend.autoStart ? "#177b53" : "#687b91"; font.bold: true }
                        Label { text: backend.startupMessage; visible: text.length > 0; color: "#526781"; wrapMode: Text.Wrap; Layout.fillWidth: true }
                    }
                }
                Label { text: "A verificação rápida examina apenas arquivos antigos diretamente na pasta temporária. Nenhum arquivo é removido automaticamente. A análise de temporários também percorre subpastas."; color: "#526781"; font.pixelSize: 12; wrapMode: Text.Wrap; Layout.fillWidth: true }
                Item { Layout.fillHeight: true }
            }
        }
        }
    }
    Dialog {
        id: confirmDialog; title: "Confirmar exclusão"; modal: true; anchors.centerIn: parent
        standardButtons: Dialog.Yes | Dialog.No
        Label { width: 420; wrapMode: Text.Wrap; color: "#17283f"; text: "Excluir permanentemente " + backend.selectedCount + " arquivos selecionados (" + backend.selectedSize + ")? Eles não serão enviados para a lixeira." }
        onAccepted: backend.clean()
    }
}
