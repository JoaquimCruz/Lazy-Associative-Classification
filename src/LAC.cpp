#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <set>
#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>
#include <functional>
using namespace std;
using namespace std::chrono;


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

vector<tuple<int, int>> transformarTuplas(const string& linha) {
    vector<tuple<int, int>> tuplas;
    stringstream ss(linha);
    string item;
    int index = 0;

    while (index < 5 && getline(ss, item, ',')) {
        int naipe = stoi(item);
        getline(ss, item, ',');
        int valor = stoi(item);

        tuplas.push_back(make_tuple(naipe, valor));
        index++;
    }

    return tuplas;
}

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

vector<int> encontraInterseccoes(const unordered_map<size_t, vector<int>>& tabela_hash, const vector<size_t>& combinacao) {
    if (combinacao.empty()) return {};

    vector<int> interseccao;
    bool primeiro = true;

    for (const auto& hash_value : combinacao) {
        auto it = tabela_hash.find(hash_value);
        if (it == tabela_hash.end()) return {};

        if (primeiro) {
            interseccao = it->second;
            primeiro = false;
        } else {
            vector<int> temp_interseccao;
            const auto& linhas_atual = it->second;

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

vector<int> gerarCombinacoes(const vector<size_t>& hashes, size_t k, int start,
                             const unordered_map<size_t, vector<int>>& tabela_hash,
                             const unordered_map<int, vector<int>>& classes,
                             unordered_map<vector<size_t>, int, VectorHash>& combinatorias) {

    vector<size_t> combinacao;
    vector<int> intersecoes_combinadas(10, 0);

    function<void(size_t, int)> gerar = [&](size_t start, size_t k) {
        if (combinacao.size() == k) {
            if (combinatorias.find(combinacao) != combinatorias.end()) {
                return;
            }

            vector<int> intersecao = encontraInterseccoes(tabela_hash, combinacao);

            if (!intersecao.empty()) {
                for (const auto& [classe, linhas] : classes) {
                    vector<int> intersecao_com_classe;
                    set_intersection(intersecao.begin(), intersecao.end(),
                                     linhas.begin(), linhas.end(),
                                     back_inserter(intersecao_com_classe));
                    intersecoes_combinadas[classe] += intersecao_com_classe.size();
                }

                combinatorias[combinacao] = 1;
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

int determinarClasseMaisProvavel(const vector<size_t>& hashes,
                                 const unordered_map<size_t, vector<int>>& tabela_hash,
                                 const unordered_map<int, vector<int>>& classes) {

    unordered_map<vector<size_t>, int, VectorHash> combinatorias;
    vector<int> combinacao_resultados(10, 0);
    vector<int> intersecoes_combinadas(10, 0);
    int classeMaisProvavel = -1;

    for (size_t k : {1, 2, 3}) {
        intersecoes_combinadas = gerarCombinacoes(hashes, k, 0, tabela_hash, classes, combinatorias);


        for (size_t i = 0; i < intersecoes_combinadas.size(); ++i) {
            combinacao_resultados[i] += intersecoes_combinadas[i]/tabela_hash.size();
        }
    }

    classeMaisProvavel = distance(combinacao_resultados.begin(), max_element(combinacao_resultados.begin(), combinacao_resultados.end()));

    return classeMaisProvavel;
}


// Função de treinamento
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

    int num_threads = 16;
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

int main() {
    unordered_map<size_t, vector<int>> assinaturas;
    unordered_map<int, vector<int>> classes;

    treinamento(assinaturas, classes);

    auto inicio = high_resolution_clock::now();

    teste(assinaturas, classes);

    auto fim = high_resolution_clock::now();
    auto duracao = duration_cast<milliseconds>(fim - inicio).count();

    cout << "Tempo de execução: " << duracao << "ms" << endl;

    return 0;
}