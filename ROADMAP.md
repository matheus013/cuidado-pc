# Roadmap — Cuidado PC

## Objetivo

Criar um aplicativo de desktop para Windows e macOS que ajude pessoas a entender o estado do computador, recuperar espaço e executar tarefas de manutenção com clareza e controle. Nenhuma limpeza ou reparo deve ocorrer automaticamente sem autorização explícita.

## Princípios

- Mostrar o que será alterado, estimar espaço e pedir confirmação antes de excluir ou reparar.
- Usar recursos documentados do sistema e limitar as permissões ao necessário.
- Tratar diferenças entre Windows e macOS separadamente e testar em ambos.
- Nunca prometer melhoria de desempenho ou correção de problemas sem evidências.

## Etapa 0 — Validar a base (prioridade imediata)

- [ ] Executar e testar a interface em instalações reais de Windows e macOS; resolver a incompatibilidade gráfica observada no ambiente de desenvolvimento.
- [ ] Confirmar versão mínima de sistema e versão de Qt e compilador compatíveis.
- [ ] Testar diagnóstico e análise com pastas vazias, arquivos inacessíveis e nomes Unicode.
- [ ] Testar limpeza somente em diretórios temporários de teste, incluindo arquivos modificados entre análise e exclusão.
- [ ] Ampliar os testes automatizados de limpeza e adicionar integração contínua em ambos os sistemas.

**Concluída quando:** o protótipo abre nos dois sistemas e os testes essenciais passam sem tocar nos arquivos pessoais do usuário.

## Etapa 1 — Limpeza confiável (MVP)

- [x] Separar interface QML, integração Qt/C++ e operações de arquivos em módulos.
- [ ] Permitir seleção individual, filtragem e exclusão de itens da lista.
- [ ] Identificar claramente origem, tamanho e idade dos arquivos; explicar limitações das estimativas.
- [ ] Tornar a operação responsiva, exibir progresso e permitir cancelamento seguro.
- [ ] Registrar resultados e erros localmente, sem incluir dados pessoais desnecessários.
- [ ] Estudar uso da lixeira por plataforma e deixar claro quando a exclusão for permanente.
- [ ] Definir uma política conservadora para pastas de cache adicionais, sem remover dados de aplicativos em uso.

**Concluída quando:** uma pessoa consegue analisar, selecionar e limpar arquivos elegíveis, entender o resultado e recuperar ou evitar exclusões indevidas conforme a opção escolhida.

## Etapa 2 — Diagnóstico e manutenção orientada

- [ ] Melhorar a visualização de espaço em disco e informações do sistema.
- [ ] Exibir itens de inicialização em modo de leitura, usando APIs adequadas para cada sistema.
- [ ] Oferecer instruções e abertura das ferramentas nativas de manutenção.
- [ ] Avaliar ações de reparo específicas por sistema, com prévia, explicação, tratamento de privilégios e confirmação individual.
- [ ] Criar histórico local de operações e instruções para solução de falhas.

**Concluída quando:** cada ação suportada tem comportamento documentado, mensagens claras e testes na plataforma correspondente.

## Etapa 3 — Distribuição e qualidade

- [ ] Empacotar executáveis e instaladores para Windows e macOS; testar instalação, atualização e remoção.
- [ ] Definir assinatura de código, notarização no macOS e distribuição confiável no Windows.
- [ ] Melhorar acessibilidade, navegação por teclado, tradução e legibilidade.
- [ ] Criar política de privacidade e documentar coleta de dados antes de introduzir qualquer telemetria.
- [ ] Executar testes em versões suportadas e publicar notas de versão.

**Concluída quando:** usuários conseguem instalar e remover o aplicativo, e uma versão candidata passou pelos testes nas plataformas suportadas.

## Etapa 4 — Evolução após validação

- [ ] Avaliar agendamento opcional, com controles claros e sem limpeza automática por padrão.
- [ ] Investigar relatórios de saúde e recomendações baseadas em dados verificáveis.
- [ ] Priorizar novas funcionalidades a partir de feedback e problemas observados.

## Riscos e decisões pendentes

- A compatibilidade gráfica do ambiente atual ainda está aberta; a interface C++/QML precisa ser validada visualmente nos sistemas suportados.
- A identificação de arquivos temporários não prova que são dispensáveis; a prévia e o controle do usuário são essenciais.
- Reparos automáticos, limpeza de registro e otimizações agressivas ficam fora do MVP até existir justificativa, documentação e testes adequados.
- Datas, equipe, licenciamento e modelo de distribuição ainda não foram definidos; o roadmap é uma ordem de trabalho, não um cronograma fechado.
