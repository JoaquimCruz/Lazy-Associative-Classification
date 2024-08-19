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
std::vector<size_t> calcularHash(const std::vector<std::tuple<int, std::pair<int, int>>>& tuplas){
    std::vector<size_t> hashes;
    size_t numero_primo = 31;

    for (const auto& tupla : tuplas) {

        size_t naipe_hash = std::hash<int>()(std :: get<0>(std::get<1>(tupla)));
        size_t valor_hash = std :: hash<int>()(std :: get <1>(std :: get<1>(tupla)));

        // Combinação dos hashes (ordem é importante para evitar colisões)
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
std::vector<std::vector<int>> agruparLinhasSemelhantes(const std::vector<std::vector<size_t>>& hashes_linhas, double treshold) {
    std::vector<std::vector<int>> baldes;  // Cada balde conterá os índices das linhas semelhantes
    std::vector<bool> processado(hashes_linhas.size(), false);  // Marcar quais linhas já foram processadas

    for (size_t i = 0; i < hashes_linhas.size(); ++i) {
        if (processado[i]) continue;  // Se a linha já foi processada, pular

        std::vector<int> balde_atual;
        balde_atual.push_back(i);
        processado[i] = true;

        for (size_t j = i + 1; j < hashes_linhas.size(); ++j) {
            if (processado[j]) continue;

            double similaridade = calcularSimilaridadeJaccard(hashes_linhas[i], hashes_linhas[j]);
            if (similaridade >= treshold) {  // Verificação de similaridade ajustada para >=
                balde_atual.push_back(j);
                processado[j] = true;  // Marcar a linha como processada
            }
        }

        baldes.push_back(balde_atual);
    }

    return baldes;
}

/*int main() {
    std::ifstream arquivo("Arquivos/poker-hand-testing.data");  // Abre o arquivo para leitura
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
    std::vector<std::vector<int>> baldes = agruparLinhasSemelhantes(hashes_linhas, treshold);

    // Exibir os baldes e as linhas que foram agrupadas neles
    std::cout << "Baldes de linhas semelhantes:" << std::endl;
    for (size_t i = 0; i < baldes.size(); ++i) {
        std::cout << "Balde " << i + 1 << ": ";
        for (const auto& indice : baldes[i]) {
            std::cout << "Linha " << indice + 1 << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}*/