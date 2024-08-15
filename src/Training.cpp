#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <string>
#include <functional>  
#include <unordered_map>


// Função que transforma uma linha de números em tuplas numeradas
std :: vector<std :: tuple<int, int>> transformarEmTuplas(const std :: string& linha) {
    std :: vector<std :: tuple<int, int>> tuplas;
    std :: stringstream ss(linha);
    std :: string item;
    int index = 0;

    while (std :: getline(ss, item, ',')) {
        tuplas.push_back(std :: make_tuple(index, stoi(item)));
        index++;
    }

    return tuplas;
}

// Função que calcula o hash de cada tupla com uma combinação mais robusta
std :: vector<size_t> calcularHash(const std :: vector<std :: tuple<int, int>>& tuplas) {
    std :: vector<size_t> hashes;
    size_t prime_number = 31;  // Número primo para reduzir colisões

    for (const auto& tupla : tuplas) {
        size_t coluna_hash = std :: hash<int>()(std :: get<0>(tupla));
        size_t valor_hash = std :: hash<int>()(std :: get<1>(tupla));

        // Combinação de hashes com uma fórmula que leva em conta a ordem e minimiza colisões
        size_t hash_value = coluna_hash * prime_number + valor_hash;
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
