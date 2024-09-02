# Lazy-Associative-Classification

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
    - [Locality Sensitive Hashing](#locality-sensitive-hashing)
    - [LAC](#lac)
    - [Dados](#dados)
- [Implementação](#implementação)
    - [LAC.cpp](#laccpp)
        - [Criação de tuplas](#criação-de-tuplas)
        - [Cálculo de Assinaturas](#cálculo-de-assinaturas)
        - [Cálculo da Similaridade de Jaccard](#cálculo-da-similaridade-de-jaccard)

# Introdução
Os algoritmos de classificação são técnicas fundamentais no campo da aprendizagem de máquinas e mineração de dados, utilizados para categorizar dados em classes pré-definidas. A classificação é um problema de aprendizado supervizionado onde o objetivo é prever a classe de novos dados com base em um conjunto de dados de treinamento rotulado. 

# Objetivo

Baseado na classe de algoritmos descritos, o objetivo do trabalho é fazer a classificação de massas de dados, baseados em padrões observados e técnicas de aprendizado supervizionado. Com o intuito de, ao final do algoritmo, a massa de dados ser classficada em classes. 

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
- Onde pair
- Two pair
- Three of a kind
- Straight
- Flush
- Full House
- Four of a kind
- Straight flush
- Royal Flush

Cada linha é um exemplo de uma mão composta por cinco cartas de um baralho padrão de 52 cartas. Cada carta é descrita usando dois atributos (naipe e valor), totalizando 10 atributos preditivos e 5 cartas. Cada coluna par é numerada com valores que variam entre 1-4 que representam o nipe da carta, enquanto as colunas ímpares são numeradas com valores que variam entre 1-13 que representam as cartas indo de ás à rei. Para mais informações sobre a massa de dados, acesse esse o site da  <a href="https://archive.ics.uci.edu/dataset/158/poker+hand" target="_blank">UCI</a>.
</p>

# Lógica de Desenvolvimento

Para a lógica do desenvolvimento do código, foi implementado, antes do processo de classificação feito pelo LAC, o processo do Locality Sensitive Hashing.Locality Sensitive Hashing (LSH) é uma técnica utilizada para encontrar aproximações de similaridade em grandes conjuntos de dados.
## Locality Sensitive Hashing
O objetivo do LSH é agrupar pontos semelhantes em buckets (baldes) utilizando funções de hash. Essas funções são projetadas de tal forma que a probabilidade de colisão é maior para pontos semelhantes do que para pontos distintos.

Diante disso, o LSH foi implementado para otimizar a classificação da massa de dados, pois o pré-processamento do arquivo de input em baldes facilita a recuperação eficiente durante a classificação. Ademais, utilizando esse processo, o LAC pode rapidamente comparar uma nova mão de poker com as mãos armazenadas em buckets semelhantes, aumentando a eficiência do processo de classificação.

A implementação das funções e seus respectivos detalhamentos serão discutidos na seção [Implementação](#Implementação).

## LAC

A classificação da massa de dados ocorreu de maneira que cada linha fosse analisada de maneira isolada. O processo para classificação de cada linha segue os seguintes passos:

- **Geração de Combinações**: Para cada linha de dados, o algoritmo gera todas as combinações possíveis dos atributos presentes. Essas combinações são usadas para verificar quais padrões são mais comuns e podem ser associados a uma determinada classe.

- **Cálculo da Confiança**: A confiança é uma métrica que avalia a probabilidade de uma classe ser correta dado que uma certa combinação de atributos está presente. Ela é calculada como a interseção entre as linhas de interseção da combinação com a classe. Combinações com alta confiança indicam que há uma forte associação entre esses atributos e a classe correspondente.
- **Cálculo do Suporte**: O suporte de uma combinação é calculado dividindo o valor da confianã pelo número total de linhas da tabela de hashes. Mais informações sobre o suporte serão discutidas na seção [Implementação](#Implementação).

- **Determinação da classe mais provável para a linha**: Após todo o processo descrito, a classe que tiver o maior suporte será a classe predita para a linha.

## Dados
A análise dos dados inicialmente foi feita para que cada linha fosse dividida em 10 tuplas, de maneira que cada tupla fosse do tipo <coluna, valor>. Entretanto, ao analisar a massa de dados e lendo a sua documentação, foi possível aferir que a abordagemhttps://cplusplus.com/reference/sstream/stringstream/ utilizando 5 tuplas, de maneira que cada tupla fosse do tipo <naipe, valor>, seria mais coerente.

Utilizando essa abordagem, foi possível reduzir o número de combinações feitas durante o processo do LAC.

# Implementação

Nessa seção serão abordadas as funções do código, bem como suas implementações e lógicas.

O código é dividido em 2 arquivos .cpp, o primeiro faz a parte do treinamento e o segundo segue para o teste e a classificação das linhas. 
## Training.cpp

## LAC.cpp

O arquivo `LAC.cpp` implementa o LSH e o LAC, fazendo a classificação das linhas. Suas funções são: 

### Criação de tuplas
A função `transformarTuplas` converte uma linha que representa uma mão de poker para tuplas no formato <naipe, carta>. A função utiliza um <a href="https://cplusplus.com/reference/sstream/stringstream/" target="_blank">stringstream</a> para extrair e converter cada par de valores na linha, organizando-os em uma estrutura que facilita o processamento subsequente, como hashing e comparação.

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

Ao final da função, as linhas que antes eram formadas por tuplas <nipe, valor> são agora formadas por assinaturas únicas que representam cada uma das 52 cartas de um baralho convencional.
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
### Cálculo da Similaridade de Jaccard

A Similaridade de Jaccard é uma métrica que mede a similaridade entre dois conjuntos. Ela é calculada como a razão entre o tamanho da interseção dos conjuntos e o tamanho da união entre eles.

\[ J(A, B) = \frac{|A \cap B|}{|A \cup B|} \]

Onde:
- \( |A \cap B| \) é o tamanho da interseção dos conjuntos \( A \) e \( B \).
- \( |A \cup B| \) é o tamanho da união dos conjuntos \( A \) e \( B \).

No código, a similaridade é utilizada na função de criação de baldes. Ela calcula a similaridade entre as assinaturas de duas linhas, caso a similaridade for alta, essas linhas são agrupadas no mesmo balde. Um fator imporante para a determinação de quais linhas são ou não ditas 'similares' é o threshold. O threshold é o fator que determina a equivalência mínima para uma linha ser agrupada no mesmo balde que outras.
```markdown
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
### Criação dos Baldes

A função `separarEmBaldes` organiza um conjunto de linhas, representadas por vetores de hashes, em grupos chamados "baldes", com base na similaridade de Jaccard. A função usa programação paralela para processar grandes volumes de dados de maneira eficiente. O processo começa dividindo as linhas entre várias threads, cada uma responsável por processar um intervalo específico. Para cada linha, a função tenta encontrar um balde existente onde a similaridade de Jaccard entre os hashes da linha e os do balde seja maior que o limiar (`threshold`). Se uma linha não encontrar um balde adequado, um novo balde é criado.

A função utiliza um `mutex` para garantir que o acesso ao vetor de baldes seja sincronizado entre as threads, evitando condições de corrida. Ademais, utiliza o número máximo de threads disponíveis na máquina, determinado automaticamente, para dividir o trabalho e processar as linhas em paralelo, o que acelera significativamente a execução. 

Após o processamento, todas as threads são sincronizadas, e os baldes resultantes são retornados, contendo os índices das linhas agrupadas por similaridade. Esse método é essencial para lidar com grandes datasets, garantindo que dados semelhantes sejam agrupados de forma eficiente para análise subsequente.
```markdown
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

