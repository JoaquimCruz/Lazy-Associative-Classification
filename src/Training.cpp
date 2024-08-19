#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <string>
#include <functional>  
#include <unordered_map>

// Função que transforma uma linha de números em tuplas numeradas (combina naipe e valor)
std::vector<std::tuple<int,int>> transformarEmTuplas(const std::string& linha) {
    std::vector<std::tuple<int,int>> tuplas;
    std::stringstream ss(linha);
    std::string item;
    int index = 0;

    while (index < 5 && std::getline(ss, item, ',')) {  // Loop para cada carta (5 cartas)
        int naipe = std::stoi(item);
        std::getline(ss, item, ',');  // Pular para a próxima coluna (valor da carta)
        int valor = std::stoi(item);
        
        tuplas.push_back(std::make_tuple(naipe,valor));
        index++;
    }

    return tuplas;
}


std::vector<size_t> calcularHash(const std::vector<std::tuple<int,int>>& tuplas) {
    std::vector<size_t> hashes;
    size_t numero_primo = 31;

    for (const auto& tupla : tuplas) {

        size_t naipe_hash = std::hash<int>()(std::get<0>(tupla));
        size_t valor_hash = std::hash<int>()(std::get<1>(tupla));

        // Combinação dos hashes (ordem é importante para evitar colisões)
        size_t hash_value = (naipe_hash * numero_primo) + valor_hash;
        hashes.push_back(hash_value);
    }

    return hashes;
}

int main() {
    std :: ifstream inputFile("Arquivos/poker-hand-training.data");  
    std :: ofstream outputFileAssinaturas("Output/Assinaturas.data");
    std :: ofstream outputFileClasses("Output/Classes.data");
    if (!inputFile) {
        std :: cerr << "Erro ao abrir o arquivo de entrada!" << std :: endl;
        return 1;
    }

    if (!outputFileAssinaturas || !outputFileClasses) {
        std :: cerr << "Erro ao abrir o arquivo de saída!" << std :: endl;
        return 1;
    }

    std :: unordered_map<size_t, std :: vector<int>> Assinaturas;
    std :: unordered_map<size_t, std :: vector<int>> classes;
    std :: vector<std :: vector<std :: tuple<int, int>>> tuplas_linhas;
    std :: vector<std :: vector<size_t>> hashes_linhas;
    std :: string linha;
    int linhaIndex = 0;

    // Ler o arquivo linha por linha
    while (getline(inputFile, linha)) {
        
        std :: vector<std :: tuple<int, int>> tuplas = transformarEmTuplas(linha);
        
        std :: vector<size_t> hashes = calcularHash(tuplas);
        
         for (size_t i = 0; i < hashes.size() - 1; i++) {
            Assinaturas[hashes[i]].push_back(linhaIndex);
        }
        size_t classe= hashes[hashes.size() - 1];

        classes[classe].push_back(linhaIndex);
        
        linhaIndex++;
    }

    inputFile.close();

     
    for (const auto& [key, value] : Assinaturas) {
        outputFileAssinaturas<< key << "(";
        for (size_t i = 0; i < value.size(); ++i) {
            outputFileAssinaturas << value[i];
            if (i < value.size() - 1) {
                outputFileAssinaturas<< ",";
            }
        }
        outputFileAssinaturas << ")" << std :: endl;
    }

    // Salvar a hash table de classes no arquivo de saída
    for (const auto& [key, value] : classes) {
        outputFileClasses << key << "(";
        for (size_t i = 0; i < value.size(); ++i) {
            outputFileClasses << value[i];
            if (i < value.size() - 1) {
                outputFileClasses << ",";
            }
        }
        outputFileClasses << ")" << std :: endl;
    }

    outputFileAssinaturas.close();
    outputFileClasses.close();

    return 0;
}
