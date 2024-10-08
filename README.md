# Lazy-Associative-Classification: Uma abordagem utilizando Assinaturas Hash

<div style="display: inline-block;">
<img align="center" height="20px" width="60px" src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white"/> 
<img align="center" height="20px" width="80px" src="https://img.shields.io/badge/Made%20for-VSCode-1f425f.svg"/> 
</a> 
</div>

<p align= "justify">
<p>
  Este repositório é referente ao trabalho final da disciplina <strong>Algoritmos e Estrutura de Dados I</strong>, ministrada pelo docente Michel Pires (<a href="https://github.com/mpiress" target="_blank">mpiress</a>) do Centro Federal de Educação Tecnológica de Minas Gerais (CEFET-MG).
</p>

# Sumário

- [Introdução](#Introdução)
- [Objetivo](#objetivo)
    - [Massa de dados](#massa-de-dados)
- [Lógica de desenvolvimento](#lógica-de-desenvolvimento)
    - [Criação de Assinaturas Hash](#criação-de-assinaturas-hash)
    - [LAC](#lac)
    - [Dados](#dados)
- [Implementação](#implementação)
    - [LAC.cpp](#laccpp)
        - [Criação de tuplas](#criação-de-tuplas)
        - [Cálculo de Assinaturas](#cálculo-de-assinaturas)
        - [Encontrar Interseções](#encontrar-interseções)
        - [Gerar Combinações](#gerar-combinações)
        - [Determinação da classe ](#determinação-da-classe)
        - [Treinamento](#Treinamento)
        - [Teste](#Teste)
    - [Input e output](#input-e-output)
- [Máquinas de Teste](#máquinas-de-teste)
- [Resultados](#resultados)
    - [Precisão Global](#precisão-global)
    - [Tempo de processamento ](#tempo-de-processamento)
    - [Analise dos Resultados](#analise-dos-resultados)
 - [Decisão pela não utilização do LSH](#decisão-pela-não-utilização-do-LSH)
- [Compilação](#compilação)


        

# Introdução
Os algoritmos de classificação são técnicas fundamentais no campo da aprendizagem de máquinas e mineração de dados, utilizados para categorizar dados em classes pré-definidas. A classificação é um problema de aprendizado supervizionado onde o objetivo é prever a classe de novos dados com base em um conjunto de dados de treinamento rotulado. 

# Objetivo

Baseado na classe de algoritmos descritos, o objetivo do trabalho é fazer a classificação de massas de dados, baseados em padrões observados e técnicas de aprendizado supervizionado. Com o intuito de, ao final do algoritmo, a massa de dados ser classificada em classes. 

<p>
  Para a realização da classificação, foi utilizado como base o algoritmo Lazy Associative Classification (LAC), criado pelos professores Adriano Veloso (<a href="mailto:adrianov@dcc.ufmg.br">adrianov@dcc.ufmg.br</a>) e Wagner Meira Jr. (<a href="mailto:meira@dcc.ufmg.br">meira@dcc.ufmg.br</a>), ambos da Universidade Federal de Minas Gerais (UFMG).
</p>

<p>
  O LAC é um algoritmo de classificação que combina a ideia de aprendizado preguiçoso com a associação de regras. O processo de agrupamento ocorre pelo armazenamento de associações entre os atributos e as classes durante a fase de treinamento. Durante a fase de teste, o algoritmo recupera e usa essas associações para classificar novos dados. Para mais informações sobre o algoritmo, acesse o site <a href="https://www.researchgate.net/publication/220766276_Lazy_Associative_Classification" target="_blank">Researchgate</a>.
</p>


## Massa de Dados

<p>
O programa foi desenvolvido, em presente momento, para a análise de mãos de poker, retornando ao usuário, qual classe de poker a sua mão pertence. Diante disso, o algoritmo considerou as 10 classes clássicas do jogo:

- High card
- One pair
- Two pair
- Three of a kind
- Straight
- Flush
- Full House
- Four of a kind
- Straight flush
- Royal Flush

Cada linha é um exemplo de uma mão composta por cinco cartas de um baralho padrão de 52 cartas. Cada carta é descrita usando dois atributos (naipe e valor), totalizando 10 atributos preditivos e 5 cartas. Cada coluna par é numerada com valores que variam entre 1-4 que representam o naipe da carta, enquanto as colunas ímpares são numeradas com valores que variam entre 1-13 que representam as cartas indo de ás à rei. Para mais informações sobre a massa de dados, acesse esse o site da  <a href="https://archive.ics.uci.edu/dataset/158/poker+hand" target="_blank">UCI</a>.
</p>

# Lógica de Desenvolvimento

Para a lógica do desenvolvimento do código, foi implementado, antes do processo de classificação feito pelo LAC, o processo de criação de assinaturas hash.

## Criação de Assinaturas Hash
A criação de assinaturas Hash foi feita para melhorar a escalabilidade do algoritmo, permitindo que ele lide eficientemente com grandes volumes de dados. Ademais, o uso das assinaturas reduzem o número de colisões, o que mantém a precisão na classificação ao evitar que diferentes dados sejam tratados de maneira igal. A lógica por trás da implementação das assinaturas será melhor explicada na seção [Cálculo de Assinaturas](#cálculo-de-assinaturas).
## LAC

A classificação da massa de dados ocorreu de maneira que cada linha fosse analisada de maneira isolada. O processo para classificação de cada linha segue os seguintes passos:

- **Geração de Combinações**: Para cada linha de dados, o algoritmo gera todas as combinações possíveis dos atributos presentes. Essas combinações são usadas para verificar quais padrões são mais comuns e podem ser associados a uma determinada classe.

- **Cálculo da Confiança**: A confiança é uma métrica que avalia a probabilidade de uma classe ser correta dado que uma certa combinação de atributos está presente. Ela é calculada como a interseção entre as linhas de interseção da combinação com a classe. Combinações com alta confiança indicam que há uma forte associação entre esses atributos e a classe correspondente.
- **Cálculo do Suporte**: O suporte de uma combinação é calculado dividindo o valor da confiança pelo número total de linhas da tabela de hashes. Mais informações sobre o suporte serão discutidas na seção [Implementação](#Implementação).

- **Determinação da classe mais provável para a linha**: Após todo o processo descrito, a classe que tiver o maior suporte será a classe predita para a linha.

## Dados
A análise dos dados inicialmente foi feita para que cada linha fosse dividida em 10 tuplas, de maneira que cada tupla fosse do tipo <coluna, valor>. Entretanto, ao analisar a massa de dados e lendo a sua documentação, foi possível aferir que a abordagem utilizando 5 tuplas, de maneira que cada tupla fosse do tipo <naipe, valor>, seria mais coerente.

Utilizando essa abordagem, foi possível reduzir o número de combinações feitas durante o processo do LAC.

# Implementação

Nessa seção serão abordadas as funções do código, bem como suas implementações e lógicas.

## LAC.cpp

O arquivo `LAC.cpp` implementa a lógica das assinaturas Hash e o LAC, fazendo a classificação das linhas. Suas funções são: 

### Criação de tuplas
A função `transformarTuplas` converte uma linha que representa uma mão de poker para tuplas no formato <naipe, carta>. A função utiliza um <a href="https://cplusplus.com/reference/sstream/stringstream/" target="_blank">stringstream</a> para extrair e converter cada par de valores na linha, organizando-os em uma estrutura que facilita o processamento subsequente, como hashing e comparação.
A função possui custo computacional de `O(n)`.

```markdown
vector<tuple<int, int>> transformarTuplas(const string& linha) {
    vector<tuple<int, int>> tuplas;
    stringstream ss(linha);
    string item;
    int index = 0;

    while (index < 5 && getline(ss, item, ',')) {  // Loop para cada carta (5 cartas)
        int naipe = stoi(item);
        getline(ss, item, ',');  // Pular para a próxima coluna (valor da carta)
        int valor = stoi(item);

        tuplas.push_back(make_tuple(naipe, valor));
        index++;
    }
    
    return tuplas;
}
```
### Cálculo de Assinaturas

A função `calcularHash` tranforma cada tupla da linha em uma assinatura hash única. Para a criação das assinaturas, foi utilizado o seguinte calculo:

(naipe da carta * número primo) + valor da carta

Foi utilizado essa fórmula com o número primo para minimizar o número de colisões. O uso de um número primo na multiplicação ajuda a distribuir os valores de hash de maneira mais uniforme. Isso é importante para evitar padrões repetitivos e agrupamentos que poderiam levar a colisões de hash.

Ao final da função, as linhas que antes eram formadas por tuplas <naipe, valor> são agora formadas por assinaturas únicas que representam cada uma das 52 cartas de um baralho convencional.

A função possui custo computacional de `O(n)`.

```markdown
vector<size_t> calcularHash(const vector<tuple<int, int>>& tuplas) {
    vector<size_t> hashes;
    size_t numero_primo = 13;

    for (const auto& tupla : tuplas) {
        size_t naipe_hash = hash<int>()(get<0>(tupla));
        size_t valor_hash = hash<int>()(get<1>(tupla));

        size_t hash_value = (naipe_hash * numero_primo) + valor_hash;
        hashes.push_back(hash_value);
    }

    return hashes;
}
```
### Encontrar Interseções
A função `encontraInterseccoes` busca identificar as linhas de dados que contêm todos os elementos de uma combinação de cartas, cruzando informações em uma tabela hash. Para cada combinação de hash, a função encontra as linhas correspondentes e calcula a interseção dessas linhas, retornando as linhas comuns a todos os elementos da combinação.

A função utiliza <a href="https://en.cppreference.com/w/cpp/algorithm/set_intersection" target="_blank">set_intersection</a> para encontrar a interseção entre as linhas analisadas. O resultado da interseção é salvo em um vetor utilizando <a href="https://en.cppreference.com/w/cpp/utility/move" target="_blank">std :: move</a>. A utilização do move possibilita que os dados sejam "movidos" para o vetor ao invés de copiados. Foi utlizado essa abordagem para otimizar o desempenho da função, com o intuito de evitar a cópia desnecessária do vector `temp_interseccao`.

Ademais, é importante ressaltar que a ordenação do vetor foi apenas possível por conta da utilização da abordagem de 5 tuplas <naipe, valor>. Essa abordagem faz com que a ordem das cartas não importe, pois a ordem das cartas na mão não altera o quão boa ou ruim uma mão de poker é.

 Ao analisar assintóticamente a função, para cada interseção subsequente, o custo computacional é $O(m \log m + n \log n + m + n)$, pois para ordenar um vetor de tamanho $m$ ou $n$ o custo é  $O(m \log m)$ ou $O(n \log n)$. Para calcular a interseção dos dois vetores é $O(m + n)$. Portanto, para cada interseção subsequente, o custo é $O(m \log m + n \log n + m + n)$.

Considerando $k$ como o número de elementos na combinação, e que $m$ e $n$ são iguais, o custo computacional total da função será de: $O(k \times m \log m)$.

```markdown
vector<int> encontraInterseccoes(const unordered_map<size_t, vector<int>>& tabela_hash, const vector<size_t>& combinacao) {
   
    if (combinacao.empty()) return {};

    vector<int> interseccao;
    bool primeiro = true;

    for (const auto& hash_value : combinacao) {
        auto it = tabela_hash.find(hash_value);
        if (it == tabela_hash.end()) return {}; // Se qualquer elemento não for encontrado, retornar interseção vazia.

        if (primeiro) {
            interseccao = it->second;
            primeiro = false;
        } else {
            vector<int> temp_interseccao;
            const auto& linhas_atual = it->second;

            // Garantir que os vetores estejam ordenados
            vector<int> ordenado_interseccao = interseccao;
            vector<int> ordenado_linhas_atual = linhas_atual;
            std::sort(ordenado_interseccao.begin(), ordenado_interseccao.end());
            std::sort(ordenado_linhas_atual.begin(), ordenado_linhas_atual.end());

            set_intersection(ordenado_interseccao.begin(), ordenado_interseccao.end(),
                             ordenado_linhas_atual.begin(), ordenado_linhas_atual.end(),
                             back_inserter(temp_interseccao));

            interseccao = std::move(temp_interseccao);

            if (interseccao.empty()) {
                break;
            }
        }
    }


    return interseccao;
}
```
### Gerar Combinações

A função `gerarCombinacoes` é responsável por gerar combinações de valores de hash e processá-las para identificar interseções entre as linhas de dados associadas. Utilizando um algoritmo recursivo, a função verifica se cada combinação já foi processada anteriormente para evitar cálculos redundantes, e em seguida, encontra as interseções dessas combinações em uma tabela hash. As interseções são então comparadas com classes de dados predefinidas, e o tamanho dessas interseções é registrado para cada classe, o que auxilia na análise de padrões e na classificação de dados complexos.

O uso da `Funçao lambda` recursiva possibilita que todas as combinações de mesma ordem sejam feitas antes do retorno da função. O código está realizando, para cada linha, combinações que variam de (1 a 1) à (5 a 5). Assim, somente após, por exemplo, fazer todas as combinações de 2 a 2 que a funçaõ irá retornar o valor dessas combinações. 

Para as linhas de 5 tuplas, são feitas 31 combinações, seguindo a seguinte formula matemática:

A fórmula para combinações é dada por:

$C(n, k) = \frac{n!}{k!(n - k)!}$

Onde:

- `n` é o número total de elementos no conjunto.
- `k` é o número de elementos que você deseja selecionar.

Dessa maneira, as combinações feitas por linha são:

1. **Combinações de 1 a 1:**

   
   $C(5, 1) = \frac{5!}{1!(5 - 1)!} = \frac{5!}{1! \times 4!} = \frac{5 \times 4!}{1 \times 4!} = \frac{5}{1} = 5$
   

2. **Combinações de 2 a 2:**

   
   $C(5, 2) = \frac{5!}{2!(5 - 2)!} = \frac{5!}{2! \times 3!} = \frac{5 \times 4 \times 3!}{2 \times 1 \times 3!} = \frac{20}{2} = 10$
   

3. **Combinações de 3 a 3:**

   
   $C(5, 3) = \frac{5!}{3!(5 - 3)!} = \frac{5!}{3! \times 2!} = \frac{5 \times 4 \times 3!}{3! \times 2 \times 1} = \frac{20}{2} = 10$
   

4. **Combinações de 4 a 4:**

   
   $C(5, 4) = \frac{5!}{4!(5 - 4)!} = \frac{5!}{4! \times 1!} = \frac{5 \times 4!}{4! \times 1} = 5$
   

5. **Combinações de 5 a 5:**

   
   $C(5, 5) = \frac{5!}{5!(5 - 5)!} = \frac{5!}{5! \times 0!} = \frac{5!}{5! \times 1} = 1$
   
   
Resultando em um total de **31 combinações** por linha.
```markdown
vector<int> gerarCombinacoes(const vector<size_t>& hashes, size_t k, int start,
                             const unordered_map<size_t, vector<int>>& tabela_hash,
                             const unordered_map<int, vector<int>>& classes,
                             unordered_map<vector<size_t>, int, VectorHash>& combinatorias) {
                 
    vector<size_t> combinacao;
    vector<int> intersecoes_combinadas(10, 0); 

    function<void(size_t, int)> gerar = [&](size_t start, size_t k) {
        if (combinacao.size() == k) {
            // Verificar se a combinação já foi processada
            if (combinatorias.find(combinacao) != combinatorias.end()) {
                
                return;
            }

            // Calcular a interseção das linhas das assinaturas com as hashes no balde
            vector<int> intersecao = encontraInterseccoes(tabela_hash, combinacao);

            if (!intersecao.empty()) {
                
                // Armazena o tamanho da interseção para cada classe relevante
                for (const auto& [classe, linhas] : classes) {
                    vector<int> intersecao_com_classe;
                    set_intersection(intersecao.begin(), intersecao.end(),
                                     linhas.begin(), linhas.end(),
                                     back_inserter(intersecao_com_classe));
                    intersecoes_combinadas[classe] += intersecao_com_classe.size();
                }

                combinatorias[combinacao] = 1; // Marca a combinação como processada
            }

            return;
        }

        for (size_t i = start; i < hashes.size(); ++i) {
            combinacao.push_back(hashes[i]);
            gerar(i + 1, k);
            combinacao.pop_back();
        }
    };

    gerar(start, k);
    return intersecoes_combinadas;
}
```
### Determinação da classe 

A função `determinarClasseMaisProvavel` tem como objetivo identificar a classe mais provável associada a uma combinação de valores de hash, avaliando as interseções entre as linhas de dados associadas. A função utiliza uma estrutura de dados `unordered_map` para armazenar combinações previamente processadas, evitando cálculos redundantes e acelerando a execução.


```markdown
int determinarClasseMaisProvavel(const vector<size_t>& hashes,
                                 const unordered_map<size_t, vector<int>>& tabela_hash,
                                 const unordered_map<int, vector<int>>& classes) {

    unordered_map<vector<size_t>, int, VectorHash> combinatorias;
    vector<int> combinacao_resultados(10, 0);
    vector<int> intersecoes_combinadas(10, 0);
    int classeMaisProvavel = -1;

    for (size_t k : {5, 4, 3, 2, 1}) {
        intersecoes_combinadas = gerarCombinacoes(hashes, k, 0, tabela_hash, classes, combinatorias);


        for (size_t i = 0; i < intersecoes_combinadas.size(); ++i) {
            combinacao_resultados[i] += intersecoes_combinadas[i]/tabela_hash.size();
        }
    }

    classeMaisProvavel = distance(combinacao_resultados.begin(), max_element(combinacao_resultados.begin(), combinacao_resultados.end()));

    return classeMaisProvavel;
}
```
### Treinamento
A função treinamento processa o arquivo de treinamento "poker-hand-training.data" para preencher dois mapas: assinaturas, que associa hashes gerados para cada mão de pôquer a IDs de linhas onde essas mãos aparecem, e classes, que mapeia cada classe (categoria de mão) aos IDs das linhas correspondentes. Após processar todas as linhas, esses mapas são utilizados para classificar novas mãos de pôquer.
```Markdown
void treinamento(unordered_map<size_t, vector<int>>& assinaturas, unordered_map<int, vector<int>>& classes) {
    ifstream arquivoTreinamento("Input/poker-hand-training.data");
    if (!arquivoTreinamento.is_open()) {
        cerr << "Erro ao abrir o arquivo de treinamento." << endl;
        return;
    }

    string linha_treinamento;
    int linha_id = 0;
    while (getline(arquivoTreinamento, linha_treinamento)) {
        vector<tuple<int, int>> tuplas = transformarTuplas(linha_treinamento);
        vector<size_t> hashes = calcularHash(tuplas);

        for (const auto& h : hashes) {
            assinaturas[h].push_back(linha_id);
        }

        stringstream ss(linha_treinamento);
        string temp;
        for (int i = 0; i < 11; ++i) {
            getline(ss, temp, ',');
        }
        int classe_real = stoi(temp);
        classes[classe_real].push_back(linha_id);

        linha_id++;
    }
    arquivoTreinamento.close();
}
```
### Teste

A função teste utiliza uma função lambda `funcao_thread` para que dentro de cada thread realize simultaneamente a previsão da classe baseada nos hashes calculados. Se a classe prevista for igual à classe real, o contador de acertos (acertos_totais) é incrementado; caso contrário, o contador de erros (erros_totais) é incrementado. Esses contadores e a escrita no arquivo de saída também são protegidos pelo mutex para garantir que os resultados não sejam corrompidos por acessos concorrentes.

A função lambda é utilizada para encapsular a lógica que será executada por cada thread, permitindo acesso às variáveis externas capturadas por referência, como linha_atual, acertos_totais, erros_totais, e os mutexes, facilitando a sincronização entre as threads.

```Markdown
void teste(const unordered_map<size_t, vector<int>>& assinaturas, const unordered_map<int, vector<int>>& classes) {

    ifstream arquivoTeste("Input/poker-hand-testing.data");
    if (!arquivoTeste.is_open()) {
        cerr << "Erro ao abrir o arquivo de teste." << endl;
        return;
    }

    vector<vector<size_t>> hashes_teste;
    string linha_testing;
    vector<int> classes_reais;
    while (getline(arquivoTeste, linha_testing)) {
        vector<tuple<int, int>> tuplas = transformarTuplas(linha_testing);
        vector<size_t> hashes = calcularHash(tuplas);
        hashes_teste.push_back(hashes);

        stringstream ss(linha_testing);
        string temp;
        for (int i = 0; i < 11; ++i) {
            getline(ss, temp, ',');
        }
        int classe_real = stoi(temp);
        classes_reais.push_back(classe_real);
    }
    arquivoTeste.close();

    if (hashes_teste.empty()) {
        cerr << "Nenhuma linha foi lida do arquivo de teste." << endl;
        return;
    }

    int num_threads = thread::hardware_concurrency();
    vector<thread> threads;

    int linha_atual = 0, acertos_totais = 0, erros_totais = 0;
    mutex mtx;
    
    ofstream output_file("output/output.txt");
    if (!output_file.is_open()) {
        cerr << "Erro ao abrir o arquivo de saída." << endl;
        return;
    }
    
    auto funcao_thread = [&]() {
        while (true) {
            size_t i;
            {
                lock_guard<mutex> lock(mtx);
                i = linha_atual++;
            }
            if (i >= hashes_teste.size()) break;

            int classe_prevista = determinarClasseMaisProvavel(hashes_teste[i], assinaturas, classes);
            int classe_real = classes_reais[i];

            {
                lock_guard<mutex> lock(mtx);
                if (classe_prevista == classe_real) {
                    acertos_totais++;
                } else {
                    erros_totais++;
                }
                output_file << "Linha " << i + 1 << ": Classe atribuída: " << classe_prevista << endl;
            }
        }
    };

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(funcao_thread);
    }

    for (auto& t : threads) {
        t.join();
    }

    int total = acertos_totais + erros_totais;
    double precisao = static_cast<double>(acertos_totais) / total;

    output_file << "acertos: " << acertos_totais << endl;
    output_file << "erros: " << erros_totais << endl;
    output_file << "acuracia: " << precisao * 100.0 << "%" << endl;
    output_file.close();

}
```
## Input e output

Arquivos de entrada:

- **poker-hand-training.data**: Contém os dados de treinamento para o modelo, onde cada linha representa uma mão de poker com suas cartas e a classe correspondente.
- **poker-hand-testing.data**: Contém os dados de teste, que serão usados para avaliar a precisão do modelo.

Formatação dos arquivos de entrada:
```markdown
1,1,1,13,2,4,2,3,1,12,0  // O ultimo elemento da linha
3,12,3,2,3,11,4,5,2,5,1     representa a classe //
1,9,4,6,1,4,3,2,3,9,1
1,4,3,13,2,13,2,1,3,6,1
3,10,2,7,1,2,2,11,4,9,0
```

Arquivo de saída: 

- **Resultados.txt**: Armazena os resultados finais da análise, incluindo métricas de precisão e outros dados relevantes.

Formatação do arquivo de saída:

```Markdown
Linha 995: Classe atribuída: 1
Linha 994: Classe atribuída: 1
Linha 996: Classe atribuída: 0
Linha 997: Classe atribuída: 0
Linha 998: Classe atribuída: 1
Linha 1000: Classe atribuída: 0
Linha 999: Classe atribuída: 0
acertos: 812
erros: 188
acuracia: 81.2%
```
# Máquinas de Teste

Para testagem do projeto, foram utilizadas 3 máquinas que rodadaram o cógido em sistema operacional Linux ou no Windows Subsystem for Linux (WSL).

| Máquina | Processador            | Memória RAM | Sistema Operacional |
|------------------|------------------------|-------------|---------------------|
| Intel inspiron 15 5000 |Intel(R) Core(TM) i7-11390H    | 16 GB       | Windows 11 Pro (WSL)     |
| Lenovo ideaPad 3i    | AMD Ryzen 7 5700U       | 12 GB        | Ubuntu 22.04       |
| PC        | AMD Ryzen 5600X                | 24 GB        | Ubuntu 24.04       |

# Resultados

## Precisão Global

A precisão geral obtida pelo modelo foi de `81.2%`. Esse resultado foi calculado com base em `1.000 linhas` de teste, onde o modelo acertou `812` previsões e errou `188`. A precisão alcançada mostra uma melhoria considerável em relação aos resultados iniciais, sugerindo que o modelo conseguiu aprender a distinguir, em algum grau, entre as classes "0" e "1". No entanto, é importante notar que o conjunto de treinamento utilizado continha apenas essas duas classes, o que significa que o modelo foi forçado a fazer uma escolha binária entre elas. Embora a precisão seja razoável, o desempenho do modelo ainda é restrito pela natureza simplificada dos dados de treinamento.

Como não foi possível modificar o `LAC` neste trabalho, não houve oportunidade para ajustar o modelo e melhorar sua performance de forma mais equilibrada entre todas as classes.



## Tempo de processamento 

O tempo de processamento para `100 linhas` foi em média `346 milissegundos`, e para `1000 linhas` foi em média `2.6 segundos`. Esse desempenho é considerado rápido e adequado, especialmente considerando a complexidade dos cálculos envolvidos, como operações de combinação e interseção. O algoritmo mostra uma boa eficiência em termos de tempo, permitindo que grandes volumes de dados sejam processados rapidamente. Esse tempo de processamento sugere que o modelo é capaz de lidar com operações intensivas sem comprometer significativamente a velocidade, apresentando um crescimento exponencial em relação a massa de dados. 

Compilação para 100 linhas:

```Markdown
g++ -Wall -Wextra  -Iinclude/ -lstdc++ -lm -o ./build//app ./build/objects/src/LAC.o
././build/app
Total de linhas: 100  //Compilação para 100 linhas
acertos: 85
erros: 15
acuracia: 85%
Tempo de execução:  346ms
```

Compilação para 1000 linhas:

```Markdown
g++ -Wall -Wextra  -Iinclude/ -lstdc++ -lm -o ./build//app ./build/objects/src/LAC.o
././build/app
Total de linhas: 1000  //Compilação para 1000 linhas
acertos: 812
erros: 188
acuracia: 81.2%
Tempo de execução:  2645ms
```
## Analise dos Resultados

A análise dos resultados indica que o desempenho do código foi significativamente influenciado pelos dados fornecidos pelo banco de dados. O treinamento com apenas duas classes limitou as capacidades do modelo, forçando-o a fazer escolhas binárias que nem sempre se alinharam corretamente com os dados de teste. Além disso, a precisão de `81.2%` revela que, embora o modelo tenha aprendido algo, ele ainda é propenso a erros, o que pode ser atribuído tanto à natureza dos dados quanto à simplicidade do algoritmo. A melhoria do desempenho poderia ser alcançada com ajustes no `LAC`, mas essas alterações não eram permitidas neste trabalho, deixando o modelo restrito à sua implementação original.

# Decisão pela não utilização do LSH
Inicialmente, a abordagem utilizada para melhoria no `LAC` foi o Locality-sensitive hashing (LSH), que faz por meio de similaridade o agrupamento das linhas em baldes. Essa etapa de pré-processamento dos dados, em teoria, otimizaria o processo feito pela função de teste, já que cada thread iria processar um balde como completo, ao invés de uma linha. Desse modo, a similaridade de linhas em cada balde seria crucial, pois, tratando das mãos de poker, quanto mais similar a linha, a probabilidade delas serem da mesma classe é maior. Entretanto, tivemos algumas dificuldades em relação ao custo computacional da função de criar baldes. A função possuia custo computacional de `O(n²)`, por conta dos dois laços de repetição aninhados. 
```Markdown
vector<vector<int>> separarEmBaldes(const vector<vector<size_t>>& hashesLinhas, double threshold) {
    vector<vector<int>> baldes; // Vetor de baldes para armazenar grupos de índices de linhas
    mutex baldesMutex; // Mutex para sincronizar acesso aos baldes

    // Função para processar um intervalo de linhas
    auto processarIntervalo = [&](int start, int end) {
        for (int i = start; i < end; ++i) {
            bool encontradoBalde = false;

            // Tentar encontrar um balde adequado para a linha atual
            {
                lock_guard<mutex> lock(baldesMutex);
                for (auto& balde : baldes) {
                    if (calcularSimilaridadeJaccard(hashesLinhas[i], hashesLinhas[balde[0]]) > threshold) {
                        balde.push_back(i);  // Adicionar ao balde encontrado
                        encontradoBalde = true;
                        break;
                    }
                }

                // Se não encontrou um balde adequado, cria um novo
                if (!encontradoBalde) {
                    baldes.push_back({i});
                }
            }
        }
    };

    // Obter o número máximo de threads disponíveis
    unsigned int numThreads = std::thread::hardware_concurrency();
    if (numThreads == 0) {
        numThreads = 8; // Valor padrão se não for possível determinar o número de núcleos
    }

    vector<thread> threads;
    int chunkSize = hashesLinhas.size() / numThreads;

    // Dividir o trabalho entre threads
    for (unsigned int t = 0; t < numThreads; ++t) {
        int start = t * chunkSize;
        int end = (t == numThreads - 1) ? hashesLinhas.size() : start + chunkSize;
        threads.emplace_back(processarIntervalo, start, end);
    }

    // Esperar todas as threads terminarem
    for (auto& thread : threads) {
        thread.join();
    }

    return baldes;
}
```
A função de separação em baldes utilizava o conceito de similaridade de Jaccard para agrupar linhas com características semelhantes em diferentes baldes. A similaridade de Jaccard é uma medida amplamente utilizada para avaliar o grau de semelhança entre dois conjuntos. Ela é definida como a razão entre o tamanho da interseção dos conjuntos e o tamanho de sua união. Essa métrica é particularmente eficaz em cenários onde é necessário quantificar a sobreposição entre conjuntos discretos, como grupos de elementos ou atributos específicos.

Formalmente, a similaridade de Jaccard entre dois conjuntos <strong>A</strong> e <strong>B</strong> é dada por:

<p><strong>S(A, B) = |A ∩ B| / |A ∪ B|</strong></p>

Onde <strong>|A ∩ B|</strong> representa o número de elementos que estão presentes em ambos os conjuntos, e <strong>|A ∪ B|</strong> corresponde ao número total de elementos distintos presentes em pelo menos um dos dois conjuntos. Essa métrica produz um valor entre 0 e 1, onde 0 indica a ausência de elementos em comum e 1 denota que os conjuntos são idênticos.


```Markdown
double calcularSimilaridadeJaccard(const vector<size_t>& conjuntoA, const vector<size_t>& conjuntoB) {
    unordered_set<size_t> setA(conjuntoA.begin(), conjuntoA.end());
    unordered_set<size_t> intersecao;
    unordered_set<size_t> uniao(setA.begin(), setA.end());

    for (const auto& elem : conjuntoB) {
        if (setA.find(elem) != setA.end()) {
            intersecao.insert(elem);
        }
        uniao.insert(elem);
    }

    double similaridade = static_cast<double>(intersecao.size()) / uniao.size();

    return similaridade;
}
```
A remoção da LSH trouxe uma melhora significativa no tempo de processamento, sem impacto perceptível na acurácia, que se manteve. Isso aconteceu porque o algoritmo continuou capaz de comparar diretamente as hashes das linhas e classificar corretamente entre as classes "0" e "1", sem a necessidade de utilizar a LSH para organizar ou aproximar os dados. Um dos desafios encontrados durante a implementação foi o uso da Similaridade de Jaccard, que não trouxe os resultados esperados. Quando o threshold de similaridade era muito elevado, o sistema gerava um número excessivo de baldes. Isso se deve ao fato de que, com a abordagem de utilizar 5 tuplas, o número de feições geradas era muito alta, por volta de 200, o que fazia com que as linhas apresentassem uma grande diferença entre si. Esse fator, combinado com o comportamento da função de criação de baldes, cujo custo computacional é quadrático (n²) devido ao for, fez com que o tempo de execução crescesse de forma considerável para processar grandes volumes de dados. Dessa forma, a remoção da LSH permitiu um caminho mais direto para a classificação, resultando em uma redução substancial do tempo de processamento, sem comprometer a precisão dos resultados. Isso evidenciou que a complexidade da função de criação de baldes era um dos maiores obstáculos para a eficiência do código.

# Compilação

Para compilar e rodar o código usando MakeFile, basta seguir os seguintes comandos.
| Comando                |  Função                                                                                               |                     
| -----------------------| ------------------------------------------------------------------------------------------------------|
|  make clean          | Apaga a última compilação realizada contida na pasta build                                            |
|  make                | Executa a compilação do programa utilizando o gcc, e o resultado vai para a pasta build               |
|  make run            | Executa o programa da pasta build após a realização da compilação                                     |
|make r| Faz os três processos descritos acima em apenas um comando|

# Autores
<p>
  Joaquim Cézar Santana da Cruz - Graduando em Engenharia da Computação pelo <a href="https://www.cefetmg.br" target="_blank">CEFET-MG</a>. Contato: (<a href="mailto:joaquim.cruz@aluno.cefetmg.br">joaquim.cruz@aluno.cefetmg.br</a>)
</p>

<p>
  Lucas Cerqueira Portela - Graduando em Engenharia da Computação pelo <a href="https://www.cefetmg.br" target="_blank">CEFET-MG</a>. Contato: (<a href="mailto:lucas.portela@aluno.cefetmg.br">lucas.portela@aluno.cefetmg.br</a>)
</p>

<p>
  João Francisco Teles da Silva - Graduando em Engenharia da Computação pelo <a href="https://www.cefetmg.br" target="_blank">CEFET-MG</a>. Contato: (<a href="mailto:joao.silva.05@aluno.cefetmg.br">joao.silva.05@aluno.cefetmg.br</a>)
</p>

<p>
  Maíra Beatriz de Almeida Lacerda - Graduando em Engenharia da Computação pelo <a href="https://www.cefetmg.br" target="_blank">CEFET-MG</a>. Contato: (<a href="mailto:maira@aluno.cefetmg.br">maira@aluno.cefetmg.br</a>)
</p>

<p>
  Bruno Prado Dos Santos - Graduando em Engenharia da Computação pelo <a href="https://www.cefetmg.br" target="_blank">CEFET-MG</a>. Contato: (<a href="mailto:bruno.santos@aluno.cefetmg.br">bruno.santos@aluno.cefetmg.br</a>)
</p>

<p>
  Eduardo Henrique Queiroz Almeida - Graduando em Engenharia da Computação pelo <a href="https://www.cefetmg.br" target="_blank">CEFET-MG</a>. Contato: (<a href="mailto:eduardo.almeida@aluno.cefetmg.br">eduardo.almeida@aluno.cefetmg.br</a>)
</p>
