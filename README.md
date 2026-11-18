# Cuidado PC

Protótipo para Windows e macOS com C++17, Qt e QML. Oferece diagnóstico de armazenamento, análise de temporários e acesso a verificações do sistema, sem instalar atualizações ou executar reparos automaticamente.

## O que faz

- **Limpeza:** percorre a pasta temporária do usuário e suas subpastas, listando arquivos comuns com mais de sete dias, até 10.000 resultados. Nenhum item é selecionado inicialmente. É possível escolher itens individualmente ou marcar todos, ver o tamanho selecionado e confirmar a exclusão.
- **Windows:** executa `sfc /verifyonly` para verificar arquivos protegidos do sistema, incluindo DLLs protegidas. O comando exige privilégios de administrador; se não estiverem disponíveis, a saída mostrará o erro. Não identifica todas as DLLs ausentes de aplicativos. A busca por atualizações usa `winget upgrade --disable-interactivity`, quando o winget está instalado e suas fontes estão disponíveis. Não abrange todos os aplicativos nem substitui o Windows Update.
- **macOS:** executa `diskutil verifyVolume /` para verificar o volume montado; isso não equivale à verificação dos arquivos protegidos do Windows. `softwareupdate --list` consulta atualizações do macOS disponíveis, mas não lista todos os aplicativos de terceiros ou da App Store.
- A saída das ferramentas é mostrada ao usuário e pode exigir interpretação; um código de saída zero por si só não prova que tudo está saudável ou atualizado. As verificações podem levar tempo e a busca por atualizações pode acessar a rede.

## Inicialização automática

O aplicativo faz uma verificação rápida ao abrir: ela examina apenas arquivos diretamente na pasta temporária com mais de sete dias, sem excluir nada. Em **Inicialização**, é possível ativar a abertura ao entrar na conta do Windows ou macOS. No Windows usa a chave Run do usuário; no macOS cria um LaunchAgent na pasta do usuário. A ativação vale para o caminho atual do executável: após mover ou reinstalar, desative e ative novamente. Desativar pelo aplicativo remove a configuração. Esta função não foi validada por reinício de sessão em ambos os sistemas.

## Limites da limpeza

Esta não é uma análise completa de todos os arquivos do computador. Pastas inacessíveis podem ser omitidas. Idade e localização não garantem que um arquivo seja dispensável. A exclusão é **permanente**, sem lixeira, e somente para itens selecionados e confirmados. Antes de excluir, o aplicativo verifica caminho, tipo, tamanho, data e idade novamente; essas verificações não são atômicas. Não segue links simbólicos durante a busca. Não limpa outras pastas pessoais, arquivos do sistema, navegadores ou registros.

## Compilar

Requisitos: Qt 6.5 ou posterior (Core, Gui, Qml, Quick, Quick Controls 2, Concurrent e Test), CMake 3.21 ou posterior e compilador C++17. No terminal:

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH="CAMINHO_PARA_QT"
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Substitua `CAMINHO_PARA_QT` pelo diretório do kit Qt. No Windows, abra o executável em `build/Release/`; no macOS, abra `build/cuidado-pc.app` ou `build/Release/cuidado-pc.app`, conforme o gerador. Para distribuição, use `windeployqt` ou `macdeployqt` e cumpra as licenças das dependências Qt.

O fluxo de CI compila e testa no Windows e macOS. O protótipo ainda não possui instalador, reparos, histórico, verificação de todos os softwares ou inventário completo de arquivos.

## Licença

O código é disponibilizado sob a [licença MIT](LICENSE). Qt e outras dependências têm licenças próprias.
