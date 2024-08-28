#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

shared_mutex mutex_balde; 
vector<int> classes_reais;

struct VectorHash {
    std::size_t operator()(const std::vector<size_t>& v) const {
        std::hash<size_t> hasher;
        std::size_t seed = 0;
        for (size_t i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

// Função que transforma uma linha de números em tuplas numeradas (combina naipe e valor)
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

// Função que calcula o hash de cada tupla (naipe e valor combinados)
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

// Função que encontra as interseções de linhas onde os elementos da combinação aparecem
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

    /*for(const auto aux : interseccao){
        cout << "Intersecao: " << aux << " " << endl;
    }*/

    return interseccao;
}

vector<int> gerarCombinacoes(const vector<size_t>& hashes, size_t k, int start,
                             const unordered_map<size_t, vector<int>>& tabela_hash,
                             const unordered_map<int, vector<int>>& classes,
                             unordered_map<vector<size_t>, int, VectorHash>& combinatorias) {
                 
    vector<size_t> combinacao;
    vector<int> intersecoes_combinadas(10, 0); // Vetor para armazenar os tamanhos das interseções por classe

    function<void(size_t, int)> gerar = [&](size_t start, size_t k) {
        if (combinacao.size() == k) {
            // Verificar se a combinação já foi processada
            if (combinatorias.find(combinacao) != combinatorias.end()) {
                // Não precisamos recalcular a interseção se a combinação já foi processada
                return;
            }

            // Calcular a interseção das linhas das assinaturas com as hashes no balde
            vector<int> intersecao = encontraInterseccoes(tabela_hash, combinacao);

            if (!intersecao.empty()) {
                /*cout << "Combinação:";
                for (const auto& hash : combinacao) {
                    cout << " " << hash;
                }
                cout << endl;*/
                
                // Armazena o tamanho da interseção para cada classe relevante
                for (const auto& [classe, linhas] : classes) {
                    vector<int> intersecao_com_classe;
                    set_intersection(intersecao.begin(), intersecao.end(),
                                     linhas.begin(), linhas.end(),
                                     back_inserter(intersecao_com_classe));

                    /*cout << "Classe " << classe << ":";
                    for (const auto& elem : intersecao_com_classe) {
                        cout << " " << elem;
                    }
                    cout << " (Tamanho: " << intersecao_com_classe.size() << ")" << endl;*/

                    intersecoes_combinadas[classe] += intersecao_com_classe.size();
                }

                combinatorias[combinacao] = 1; // Marca a combinação como processada
            }

            /*cout << "Intersecoes combinadas para k=" << k << ": ";
            for (const auto& ic : intersecoes_combinadas) {
                cout << ic << " ";
            }
            cout << endl;*/

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

int determinarClasseMaisProvavel(const vector<size_t>& hashes, 
                                 const unordered_map<size_t, vector<int>>& tabela_hash,
                                 const unordered_map<int, vector<int>>& classes) {

    unordered_map<vector<size_t>, int, VectorHash> combinatorias; 
    vector<int> combinacao_resultados(10, 0); // Vetor para armazenar a soma das interseções por classe
    vector<int> intersecoes_combinadas(10, 0);
    int classeMaisProvavel = -1;

    // Verificar combinações para tamanhos diferentes
    for (size_t k : {5,4,3,2}) {

        // Obter o vetor de interseções para o tamanho da combinação
        intersecoes_combinadas = gerarCombinacoes(hashes, k, 0, tabela_hash, classes, combinatorias);

        // Tratamento especial para k = 5, 4 e 3
        if (k == 5) {
            int maior_intersecao = *max_element(intersecoes_combinadas.begin(), intersecoes_combinadas.end());
            if (maior_intersecao > 0) {
                classeMaisProvavel = distance(intersecoes_combinadas.begin(), max_element(intersecoes_combinadas.begin(), intersecoes_combinadas.end()));
                return classeMaisProvavel; // Retorna imediatamente a classe correspondente
            }
        } else if (k == 3) {
            int maior_intersecao = *max_element(intersecoes_combinadas.begin(), intersecoes_combinadas.end());
            if (maior_intersecao > 0) {
                vector<int>::iterator it_max = max_element(intersecoes_combinadas.begin(), intersecoes_combinadas.end());
                int quantidade = count(intersecoes_combinadas.begin(), intersecoes_combinadas.end(), maior_intersecao);

                // Se houver mais de uma classe com a maior interseção, continue a verificação
                if (quantidade > 1) {
                    continue;
                } else {
                    classeMaisProvavel = distance(intersecoes_combinadas.begin(), it_max);
                    return classeMaisProvavel;
                }
            }
        }

        // Somar os tamanhos das interseções ao vetor de resultados, aplicando o peso
        for (size_t i = 0; i < intersecoes_combinadas.size(); ++i) {
            combinacao_resultados[i] += intersecoes_combinadas[i];
        }
    }

    // Se não houve interseções claras para k = 5, 4 ou 3, determinar a classe mais provável com base nas combinações ponderadas
    classeMaisProvavel = distance(combinacao_resultados.begin(), max_element(combinacao_resultados.begin(), combinacao_resultados.end()));

    return classeMaisProvavel;
}


void processarBalde(const vector<int>& balde, 
                    const vector<vector<size_t>>& hashes_teste,
                    const unordered_map<size_t, vector<int>>& assinaturas,
                    const unordered_map<int, vector<int>>& classes,
                    int& acertos, 
                    int& totalLinhas) {
    auto inicio = high_resolution_clock::now();
    for (int linha_idx : balde) {
        int classe_predita, classe_real;

        // Trava compartilhada para leitura das assinaturas e classes
        {
            shared_lock<shared_mutex> lock(mutex_balde);
            classe_predita = determinarClasseMaisProvavel(hashes_teste[linha_idx], assinaturas, classes);
            classe_real = classes_reais[linha_idx];
        } // lock é liberado automaticamente aqui

        // Trava exclusiva para escrever os resultados de acertos e total de linhas
        {
            unique_lock<shared_mutex> unique_lock(mutex_balde);
            if (classe_predita == classe_real) {
                acertos++;
            } 
            totalLinhas++;
        } // unique_lock é liberado automaticamente aqui
    }

    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<seconds>(fim - inicio).count();
    //cout << "processamento baldes: " << duracao << endl;
}

int main() {
    // Treinamento
    //cout << "Iniciando treinamento..." << endl;
    ifstream arquivoTreinamento("Input/poker-hand-training.data");
    if (!arquivoTreinamento.is_open()) {
        cerr << "Erro ao abrir o arquivo de treinamento." << endl;
        return 1;
    }
    
    unordered_map<size_t, vector<int>> assinaturas;
    unordered_map<int, vector<int>> classes;

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

    

    //cout << "Treinamento concluído!" << endl;

    // Processar o arquivo de teste
    //cout << "Iniciando teste..." << endl;
    ifstream arquivoTeste("Input/poker-hand-testing.data");
    if (!arquivoTeste.is_open()) {
        cerr << "Erro ao abrir o arquivo de teste." << endl;
        return 1;
    }

    vector<vector<size_t>> hashes_teste;
    string linha_testing;
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
        return 1;
    }


    auto inicio = high_resolution_clock::now();
    // Separar as linhas de teste em baldes
    double threshold = 0.4;
    vector<vector<int>> baldes = separarEmBaldes(hashes_teste, threshold);
    

    

    int totalLinhas = 0;
    int acertos = 0;

    int num_threads = thread::hardware_concurrency();
    vector<thread> thread_pool(num_threads);

    atomic<int> next_balde(0);

    auto thread_worker = [&]() {
    while (true) {
        int balde_idx = next_balde.fetch_add(1);
        if (balde_idx >= baldes.size()) break;
        processarBalde(baldes[balde_idx], hashes_teste, assinaturas, classes, acertos, totalLinhas);
    }
    };

    for (auto& th : thread_pool) {
        th = thread(thread_worker);
    }

    for (auto& th : thread_pool) {
        th.join();
    }
    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fim - inicio).count();

    // Resultados
    cout << "Total de linhas testadas: " << totalLinhas << endl;
    cout << "Acertos: " << acertos << endl;
    cout << "Precisão: " << (double(acertos) / totalLinhas) * 100 << "%" << endl;
    cout << "Tempo de execução: " << duracao << " milisegundos" << endl;

    return 0;
}