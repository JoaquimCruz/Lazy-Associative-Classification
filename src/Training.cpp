#include "Training.hpp"

PokerHandTreino::PokerHandTreino() = default;

PokerHandTreino::PokerHandTreino(const std::string& inputFilePath, const std::string& outputAssinaturasPath, const std::string& outputClassesPath)
    : inputFile(inputFilePath), outputFileAssinaturas(outputAssinaturasPath), outputFileClasses(outputClassesPath) {
    if (!inputFile) {
        throw std::runtime_error("Erro ao abrir o arquivo de entrada!");
    }

    if (!outputFileAssinaturas || !outputFileClasses) {
        throw std::runtime_error("Erro ao abrir os arquivos de saída!");
    }
}

void PokerHandTreino::setAssinaturas(const std::unordered_map<size_t, std::vector<int>>& novasAssinaturas) {
    Assinaturas = novasAssinaturas;
}

const std::unordered_map<size_t, std::vector<int>>& PokerHandTreino::getAssinaturas() const {
    return Assinaturas;
}

void PokerHandTreino::setClasses(const std::unordered_map<size_t, std::vector<int>>& novasClasses) {
    Classes = novasClasses;
}

const std::unordered_map<size_t, std::vector<int>>& PokerHandTreino::getClasses() const {
    return Classes;
}

void PokerHandTreino::CriarTuplas() {
    std::string linha;
    int linhaIndex = 0;

    while (getline(inputFile, linha)) {
        std::vector<std::tuple<int, int>> tuplas = transformarEmTuplas(linha);
        std::vector<size_t> hashes = calcularHash(tuplas);

        for (size_t i = 0; i < hashes.size() - 1; i++) {
            Assinaturas[hashes[i]].push_back(linhaIndex);
        }
        size_t classe = hashes[hashes.size() - 1];
        Classes[classe].push_back(linhaIndex);

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
    for (const auto& [key, value] : Classes) {
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

}

std::vector<std::tuple<int, int>> PokerHandTreino::transformarEmTuplas(const std::string& linha) const {
    std::vector<std::tuple<int, int>> tuplas;
    std::stringstream ss(linha);
    std::string item;
    int index = 0;

    while (index < 5 && std::getline(ss, item, ',')) {
        int naipe = std::stoi(item);
        std::getline(ss, item, ',');
        int valor = std::stoi(item);

        tuplas.push_back(std::make_tuple(naipe, valor));
        index++;
    }

    return tuplas;
}

std::vector<size_t> PokerHandTreino::calcularHash(const std::vector<std::tuple<int, int>>& tuplas) const {
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

