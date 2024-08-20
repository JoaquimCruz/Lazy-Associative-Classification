#ifndef POKER_HAND_TREINO_HPP
#define POKER_HAND_TREINO_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <string>
#include <functional>  
#include <unordered_map>

class PokerHandTreino{
public:
    
    PokerHandTreino();

    PokerHandTreino(const std::string& inputFilePath, const std::string& outputAssinaturasPath, const std::string& outputClassesPath);

    
    const std::unordered_map<size_t, std::vector<int>>& getAssinaturas() const;
    void setAssinaturas(const std::unordered_map<size_t, std::vector<int>>& novasAssinaturas);

    const std::unordered_map<size_t, std::vector<int>>& getClasses() const;
    void setClasses(const std::unordered_map<size_t, std::vector<int>>& novasClasses);

    void CriarTuplas();

private:

    std::ifstream inputFile;
    std::ofstream outputFileAssinaturas;
    std::ofstream outputFileClasses;

    std::unordered_map<size_t, std::vector<int>> Assinaturas;
    std::unordered_map<size_t, std::vector<int>> Classes;

    std::vector<std::tuple<int, int>> transformarEmTuplas(const std::string& linha) const;
    std::vector<size_t> calcularHash(const std::vector<std::tuple<int, int>>& tuplas) const;
};

#endif 
