# Cuidado PC

Protótipo multiplataforma de diagnóstico e limpeza de arquivos temporários, com backend em C++17 e interface em Qt Quick/QML. Não requer Python em tempo de execução.

## Funcionalidades

- Diagnóstico de sistema e armazenamento.
- Prévia de arquivos comuns diretamente na pasta temporária com mais de sete dias.
- Exclusão dos arquivos listados após confirmação, com revalidação de idade, tamanho e data.

A exclusão é permanente, não usa a lixeira e se aplica à lista inteira. A idade não garante que um arquivo seja dispensável: revise a lista. Arquivos alterados ou inacessíveis são ignorados. A verificação e exclusão não são atômicas; um arquivo pode mudar entre essas etapas. Não há reparos automáticos, histórico nem seleção individual.

## Compilar

Requisitos: Qt 6.5 ou posterior com Core, Gui, Qml, Quick, Quick Controls 2 e Test; CMake 3.21 ou posterior; compilador C++17. Instale Qt para sua plataforma e selecione o compilador correspondente. No terminal:

```sh
cmake -S . -B build -DCMAKE_PREFIX_PATH="CAMINHO_PARA_QT"
cmake --build build --config Release
ctest --test-dir build -C Release --output-on-failure
```

Substitua `CAMINHO_PARA_QT` pelo diretório do kit Qt (por exemplo, `.../6.8.3/macos` ou `.../6.8.3/msvc2022_64`). No Windows, abra o executável em `build/Release/`; no macOS, abra `build/cuidado-pc.app` ou o bundle dentro de `build/Release/`, conforme o gerador. Ao distribuir para outro computador, use as ferramentas de implantação do Qt (`windeployqt` ou `macdeployqt`) e cumpra os termos de licença das bibliotecas Qt escolhidas.

O repositório inclui uma configuração de CI para compilar e testar no Windows e macOS. A interface e os instaladores ainda precisam de validação em máquinas reais.

## Licença

O código do projeto é disponibilizado sob a [licença MIT](LICENSE). O Qt e outras dependências têm licenças próprias, que devem ser consideradas na distribuição.
