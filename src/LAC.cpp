#include "Training.cpp"
#include <bits/stdc++.h>

// Função que transforma uma linha de números em tuplas numeradas (combina naipe e valor)
std::vector<std::tuple<int, int>> transformarTuplas(const std::string& linha) {
    std::vector<std::tuple<int, int>> tuplas;
    std::stringstream ss(linha);
    std::string item;
    int index = 0;

    while (index < 5 && std::getline(ss, item, ',')) {  // Loop para cada carta (5 cartas)
        int naipe = std::stoi(item);
        std::getline(ss, item, ',');  // Pular para a próxima coluna (valor da carta)
        int valor = std::stoi(item);
        
        tuplas.push_back(std::make_tuple(naipe, valor));
        index++;
    }

    return tuplas;
}

// Função que calcula o hash de cada tupla (naipe e valor combinados)
std::vector<size_t> calcularHash(const std::vector<std::tuple<int, int>>& tuplas) {
    std::vector<size_t> hashes;
    size_t numero_primo = 31;

    for (const auto& tupla : tuplas) {
        size_t naipe_hash = std::hash<int>()(std::get<0>(tupla));
        size_t valor_hash = std::hash<int>()(std::get<1>(tupla));

        size_t hash_value = (naipe_hash * numero_primo) + valor_hash;
        hashes.push_back(hash_value);
    }

    return hashes;
}

// Função que calcula a similaridade de Jaccard entre duas linhas de hashes
double calcularSimilaridadeJaccard(const std::vector<size_t>& linha1, const std::vector<size_t>& linha2) {
    std::set<size_t> conjunto1(linha1.begin(), linha1.end());
    std::set<size_t> conjunto2(linha2.begin(), linha2.end());

    std::set<size_t> interseccao;
    std::set<size_t> uniao;

    std::set_intersection(conjunto1.begin(), conjunto1.end(),
                          conjunto2.begin(), conjunto2.end(),
                          std::inserter(interseccao, interseccao.begin()));

    std::set_union(conjunto1.begin(), conjunto1.end(),
                   conjunto2.begin(), conjunto2.end(),
                   std::inserter(uniao, uniao.begin()));

    return static_cast<double>(interseccao.size()) / uniao.size();
}

// Função que realiza o agrupamento das linhas semelhantes
std::unordered_map<int, std::vector<int>> agruparLinhasSemelhantes(const std::vector<std::vector<size_t>>& hashes_linhas, double treshold) {
    std::unordered_map<int, std::vector<int>> baldes;  // Mapa para armazenar os índices das linhas semelhantes
    std::vector<bool> processado(hashes_linhas.size(), false);  // Marcar quais linhas já foram processadas
    int balde_atual_id = 0;

    for (size_t i = 0; i < hashes_linhas.size(); ++i) {
        if (processado[i]) continue;  // Se a linha já foi processada, pular

        baldes[balde_atual_id].push_back(i);
        processado[i] = true;

        for (size_t j = i + 1; j < hashes_linhas.size(); ++j) {
            if (processado[j]) continue;

            double similaridade = calcularSimilaridadeJaccard(hashes_linhas[i], hashes_linhas[j]);
            if (similaridade >= treshold) {  // Verificação de similaridade ajustada para >=
                baldes[balde_atual_id].push_back(j);
                processado[j] = true;
            }
        }

        balde_atual_id++;
    }

    return baldes;
}

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);

        return h1 ^ h2;  
    }
};

// Declaração da função analisarCombinacoes
std::unordered_map<int, std::vector<int>> analisarCombinacoes(
    const std::unordered_map<int, std::vector<int>>& baldes,
    const std::unordered_map<size_t, std::vector<int>>& Classes) {

    // Cache para evitar recomputar as interseções
    std::unordered_map<std::pair<int, int>, std::vector<int>, pair_hash> cache;

    // Mapa para armazenar os resultados
    std::unordered_map<int, std::vector<int>> resultados;

    // Percorre cada balde e suas assinaturas
    for (const auto& [baldeId, assinaturas] : baldes) {
        for (size_t i = 0; i < assinaturas.size(); ++i) {
            for (size_t j = i + 1; j < assinaturas.size(); ++j) {
                // Cria um par de assinaturas
                std::pair<int, int> dupla = std::make_pair(assinaturas[i], assinaturas[j]);

                // Verifica se o resultado dessa dupla já está no cache
                if (cache.find(dupla) == cache.end()) {
                    std::vector<int> linhas_comuns;

                    // Intersecção das linhas em que as duas assinaturas aparecem
                    const auto& linhas1 = baldes.at(assinaturas[i]);
                    const auto& linhas2 = baldes.at(assinaturas[j]);

                    std::set_intersection(linhas1.begin(), linhas1.end(),
                                          linhas2.begin(), linhas2.end(),
                                          std::back_inserter(linhas_comuns));

                    // Armazena o resultado da interseção no cache
                    cache[dupla] = linhas_comuns;
                }

                const auto& linhas_comuns = cache[dupla];

                // Verifica a interseção das linhas comuns com as classes
                for (const auto& [classeId, linhasClasse] : Classes) {
                    std::set<int> interseccao;
                    std::set_intersection(linhas_comuns.begin(), linhas_comuns.end(),
                                          linhasClasse.begin(), linhasClasse.end(),
                                          std::inserter(interseccao, interseccao.begin()));

                    // Armazena o resultado no mapa de resultados
                    resultados[classeId].push_back(interseccao.size());
                }
            }
        }
    }

    return resultados;
}

/*int main() {
    try {
        PokerHandTreino aux("Input/poker-hand-testing.data", "Output/Assinaturas.data", "Output/Classes.data");
        aux.CriarTuplas();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}*/

int main() {
    std::ifstream arquivo("Input/poker-hand-testing.data");  // Abre o arquivo para leitura
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo." << std::endl;
        return 1;
    }

    std::vector<std::vector<std::tuple<int, int>>> tuplas_linhas;
    std::string linha;

    // Lê cada linha do arquivo
    while (std::getline(arquivo, linha)) {
        tuplas_linhas.push_back(transformarTuplas(linha));
    }

    arquivo.close();  // Fecha o arquivo após a leitura

    // Etapa 2: Calcular o hash das tuplas
    std::vector<std::vector<size_t>> hashes_linhas;
    for (const auto& tuplas : tuplas_linhas) {
        hashes_linhas.push_back(calcularHash(tuplas));
    }

    // Etapa 3: Agrupar linhas semelhantes usando a similaridade de Jaccard
    double treshold = 0.5;  // Valor de threshold para similaridade de Jaccard
    std::unordered_map<int, std::vector<int>> baldes = agruparLinhasSemelhantes(hashes_linhas, treshold);

    // Exibir os baldes e as linhas que foram agrupadas neles
    std::cout << "Baldes de linhas semelhantes:" << std::endl;
    for (const auto& [balde_id, linhas] : baldes) {
        std::cout << "Balde " << balde_id + 1 << ": ";
        for (const auto& indice : linhas) {
            std::cout << "Linha " << indice + 1 << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
