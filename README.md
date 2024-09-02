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
        - [Criação dos Baldes](#criação-dos-baldes)
        - [Encontrar Interseções](#encontrar-interseções)
        - [Gerar Combinações](#gerar-combinações)
        - [Determinação da classe ](#determinação-da-classe)
        - [Processamento dos Baldes](#processamento-dos-baldes)
        

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
- Onde pair
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

Para a lógica do desenvolvimento do código, foi implementado, antes do processo de classificação feito pelo LAC, o processo do Locality Sensitive Hashing.Locality Sensitive Hashing (LSH) é uma técnica utilizada para encontrar aproximações de similaridade em grandes conjuntos de dados.
## Locality Sensitive Hashing
O objetivo do LSH é agrupar pontos semelhantes em buckets (baldes) utilizando funções de hash. Essas funções são projetadas de tal forma que a probabilidade de colisão é maior para pontos semelhantes do que para pontos distintos.

Diante disso, o LSH foi implementado para otimizar a classificação da massa de dados, pois o pré-processamento do arquivo de input em baldes facilita a recuperação eficiente durante a classificação. Ademais, utilizando esse processo, o LAC pode rapidamente comparar uma nova mão de poker com as mãos armazenadas em buckets semelhantes, aumentando a eficiência do processo de classificação.

A implementação das funções e seus respectivos detalhamentos serão discutidos na seção [Implementação](#Implementação).

## LAC

A classificação da massa de dados ocorreu de maneira que cada linha fosse analisada de maneira isolada. O processo para classificação de cada linha segue os seguintes passos:

- **Geração de Combinações**: Para cada linha de dados, o algoritmo gera todas as combinações possíveis dos atributos presentes. Essas combinações são usadas para verificar quais padrões são mais comuns e podem ser associados a uma determinada classe.

- **Cálculo da Confiança**: A confiança é uma métrica que avalia a probabilidade de uma classe ser correta dado que uma certa combinação de atributos está presente. Ela é calculada como a interseção entre as linhas de interseção da combinação com a classe. Combinações com alta confiança indicam que há uma forte associação entre esses atributos e a classe correspondente.
- **Cálculo do Suporte**: O suporte de uma combinação é calculado dividindo o valor da confiança pelo número total de linhas da tabela de hashes. Mais informações sobre o suporte serão discutidas na seção [Implementação](#Implementação).

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

Ao final da função, as linhas que antes eram formadas por tuplas <naipe, valor> são agora formadas por assinaturas únicas que representam cada uma das 52 cartas de um baralho convencional.
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

### Encontrar Interseções
A função `encontraInterseccoes` busca identificar as linhas de dados que contêm todos os elementos de uma combinação de cartas, cruzando informações em uma tabela hash. Para cada combinação de hash, a função encontra as linhas correspondentes e calcula a interseção dessas linhas, retornando as linhas comuns a todos os elementos da combinação.

A função utiliza <a href="https://en.cppreference.com/w/cpp/algorithm/set_intersection" target="_blank">set_intersection</a> para encontrar a interseção entre as linhas analisadas. O resultado da interseção é salvo em um vetor utilizando <a href="https://en.cppreference.com/w/cpp/utility/move" target="_blank">std :: move</a>. A utilização do move possibilita que os dados sejam "movidos" para o vetor ao inés de copiados. Foi utlizado essa abordagem para otimizar o desempenho da função, com o intuito de evitar a cópia desnecessária do vector `temp_interseccao`.

Ademais, é importante ressaltar que a ordenação do vetor foi apenas possível por conta da utilização da abordagem de 5 tuplas <naipe, valor>. Essa abordagem faz com que a ordem das cartas não importe, pois a ordem das cartas na mão não altera o quão boa ou ruim uma mão de poker é.
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

\[ 
C(n, k) = \frac{n!}{k!(n - k)!} 
\]

onde:

- n é o número total de elementos no conjunto.
- k é o número de elementos que você deseja selecionar

Diante disso, as combinações feitas por linha são:
1. **Combinações de 1 a 1:**
   \[
   C(5, 1) = \frac{5!}{1!(5 - 1)!} = \frac{5!}{1! \times 4!} = \frac{5 \times 4!}{1 \times 4!} = \frac{5}{1} = 5
   \]

2. **Combinações de 2 a 2:**
   \[
   C(5, 2) = \frac{5!}{2!(5 - 2)!} = \frac{5!}{2! \times 3!} = \frac{5 \times 4 \times 3!}{2 \times 1 \times 3!} = \frac{20}{2} = 10
   \]

3. **Combinações de 3 a 3:**
   \[
   C(5, 3) = \frac{5!}{3!(5 - 3)!} = \frac{5!}{3! \times 2!} = \frac{5 \times 4 \times 3!}{3! \times 2 \times 1} = \frac{20}{2} = 10
   \]

4. **Combinações de 4 a 4:**
   \[
   C(5, 4) = \frac{5!}{4!(5 - 4)!} = \frac{5!}{4! \times 1!} = \frac{5 \times 4!}{4! \times 1} = 5
   \]

5. **Combinações de 5 a 5:**
   \[
   C(5, 5) = \frac{5!}{5!(5 - 5)!} = \frac{5!}{5! \times 0!} = \frac{5!}{5! \times 1} = 1
   \]

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

A função `determinarClasseMaisProvavel` tem como objetivo identificar a classe mais provável associada a uma combinação de valores de hash, as combinações e avaliando as interseções entre as linhas de dados associadas. A função utiliza uma estrutura de dados `unordered_map` para armazenar combinações previamente processadas, evitando cálculos redundantes e acelerando a execução.

O processo se inicia fazendo as comparações 5 a 5. Como foi utilizada a abordagem de 5 tuplas <naipe,valor>, caso for encontrada alguma interseção de 5 elementos o código já retornará que a classe em que ocorreu a interseção é a classe da linha. 

Para as interseções de 4 a 4, o código faz um processo similar. Caso haja mais de uma classe que possua interseções de 4 a 4, é feito uma análise do suporte de cada classe para essas combinações em específico, e a classe com maior suporte é referida como a classe da linha. 

Após essas duas análises, caso não haja interseções de (5 a 5) e (4 a 4), o código faz todas as outras combinações e faz o cálculo padrão de suporte e confiança, descrito na seção [LAC](#lac).

Essa abordagem foi tomada pela necessidade de balancear o resultado de predição de classes. O arquivo de treino em que o código teste se baseia para preditar a classe é composto predominantemente pelas classes 0 (High card) e 1 (One pair), pois elas são as mões mais comuns do jogo. Assim, caso não ocorra esse tratamento, o código apenas varia entre essas duas classes para a classiicação de cada linha.

 Em vista disso, a abordagem de aplicar pesos às interseções permite que o algoritmo seja flexível e robusto na classificação, garantindo que a classe mais provável seja determinada com base em uma análise abrangente dos dados disponíveis.
```markdown
int determinarClasseMaisProvavel(const vector<size_t>& hashes, 
                                 const unordered_map<size_t, vector<int>>& tabela_hash,
                                 const unordered_map<int, vector<int>>& classes) {

    unordered_map<vector<size_t>, int, VectorHash> combinatorias; 
    vector<int> combinacao_resultados(10, 0); 
    vector<int> intersecoes_combinadas(10, 0);
    int classeMaisProvavel = -1;

    for (size_t k : {5,4,3,2,1}) {

        
        intersecoes_combinadas = gerarCombinacoes(hashes, k, 0, tabela_hash, classes, combinatorias);

        // Tratamento especial para k = 5, 4 
        if (k == 5) {
            int maior_intersecao = *max_element(intersecoes_combinadas.begin(), intersecoes_combinadas.end());
            if (maior_intersecao > 0) {
                classeMaisProvavel = distance(intersecoes_combinadas.begin(), max_element(intersecoes_combinadas.begin(), intersecoes_combinadas.end()));
                return classeMaisProvavel; // Retorna imediatamente a classe correspondente
            }
        } else if (k == 4) {
            int maior_intersecao = *max_element(intersecoes_combinadas.begin(), intersecoes_combinadas.end());
            if (maior_intersecao > 0) {
                vector<int>::iterator it_max = max_element(intersecoes_combinadas.begin(), intersecoes_combinadas.end());
                int quantidade = count(intersecoes_combinadas.begin(), intersecoes_combinadas.end(), maior_intersecao);

                
                if (quantidade > 1) {
                    continue;
                } else {
                    classeMaisProvavel = distance(intersecoes_combinadas.begin(), it_max);
                    return classeMaisProvavel;
                }
            }
        }

        
        for (size_t i = 0; i < intersecoes_combinadas.size(); ++i) {
            combinacao_resultados[i] += intersecoes_combinadas[i];
        }
    }

    
    classeMaisProvavel = distance(combinacao_resultados.begin(), max_element(combinacao_resultados.begin(), combinacao_resultados.end()));

    return classeMaisProvavel;
}
```
### Processamento dos Baldes

A função `processarBalde` é responsável por processar um conjunto de linhas agrupadas em um balde, avaliando a classe predita para cada linha e comparando-a com a classe real para determinar a precisão do modelo. 

Para cada linha no balde, a função determina a classe mais provável. A determinação da classe é feita dentro de um bloco protegido por uma `shared_lock`, que permite leitura simultânea segura das assinaturas e classes por múltiplas threads. Após determinar a classe predita, a função compara essa predição com a classe real da linha. Se ambas forem iguais, o contador de acertos é incrementado. O total de linhas processadas também é atualizado. Ambas as operações de escrita são realizadas dentro de um bloco protegido por um `unique_lock`, que garante que apenas uma thread possa modificar os valores de `acertos` e `totalLinhas` por vez.
```markdown
void processarBalde(const vector<int>& balde, 
                    const vector<vector<size_t>>& hashes_teste,
                    const unordered_map<size_t, vector<int>>& assinaturas,
                    const unordered_map<int, vector<int>>& classes,
                    int& acertos, 
                    int& totalLinhas) {
    
    for (int linha_idx : balde) {
        int classe_predita, classe_real;

        
        {
            shared_lock<shared_mutex> lock(mutex_balde);
            classe_predita = determinarClasseMaisProvavel(hashes_teste[linha_idx], assinaturas, classes);
            classe_real = classes_reais[linha_idx];
        } 
        
        {
            unique_lock<shared_mutex> unique_lock(mutex_balde);
            if (classe_predita == classe_real) {
                acertos++;
            } 
            totalLinhas++;
        } 
    }
    
}
```
