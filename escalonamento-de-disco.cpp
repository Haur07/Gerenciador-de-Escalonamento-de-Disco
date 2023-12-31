/*
    GERENCIADOR DE ESCALONAMENTO DE ACESSO AO DISCO

    Universidade de Passo Fundo (UPF)
    Sistemas Operacionais II
    Professor: Marcelo Trindade Rebonatto - Doutor
    Acadêmico: Yeun Haur Kang (193593)
*/

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <ctime>
using namespace std;

/*
    Sobrecarga de saída de dados personalizado
*/

// Formatar números
ostream& formatar(ostream& os) {
    return os << setfill('0') << setw(2);
}

// Exibição de vetores
ostream& operator<<(ostream& os, vector<int>& requisicoes) {
    os << "[";
    bool primeiro = true;

    for(unsigned int i = 0; i < requisicoes.size(); i++) {
        if(!primeiro) cout << " - ";
        cout << setfill('0') << setw(2) << requisicoes[i];
        primeiro = false;
    }

    os << "]";
    return os;
}

/*
    Declarações
*/

// Funções básicas
void inserir_vetor(vector<int>& requisicoes, int& posicao_inicial);
void exibir_original(const vector<int>& requisicoes, const int posicao_inicial);
void exibir_resultado(const vector<int>& requisicoes, const int posicao_atual, const int deslocamento);

// Tipos de algoritmos de escalonamento
void escalonador_fcfs(vector<int> requisicoes, int posicao_inicial);
void escalonador_sstf(vector<int> requisicoes, int posicao_inicial);
void escalonador_scan(vector<int> requisicoes, int posicao_inicial);
void escalonador_cscan(vector<int> requisicoes, int posicao_inicial);
void escalonador_clook(vector<int> requisicoes, int posicao_inicial);

/*
    Main
*/

int main() {
    int posicao_inicial;
    vector<int> requisicoes;

    cout << "\033[1m===== Gerenciador de Escalonamento de Acesso ao Disco =====\033[0m" << endl;
    inserir_vetor(requisicoes, posicao_inicial);

    escalonador_fcfs(requisicoes, posicao_inicial);
    escalonador_sstf(requisicoes, posicao_inicial);
    escalonador_scan(requisicoes, posicao_inicial);
    escalonador_cscan(requisicoes, posicao_inicial);
    escalonador_clook(requisicoes, posicao_inicial);

    cout << "\n\033[1m===========================================================\033[0m" << endl;
    return 0;
}

/*
    Definição das funções
*/

// Funções básicas
void inserir_vetor(vector<int>& requisicoes, int& posicao_inicial) {
    int valor_gerado;
    srand((unsigned) time(NULL));

    for(int i = 0; i < 11; i++) {
        valor_gerado = 1 + (rand() % 99);
        if(i < 10) {
            requisicoes.push_back(valor_gerado);
        } else {
            posicao_inicial = valor_gerado;
        }
    }
}

void exibir_original(vector<int>& requisicoes, int posicao_inicial) {
    cout << "\nPosição inicial da cabeça de leitura e gravação: " << posicao_inicial << endl;
    cout << "Original: " << requisicoes << endl;
}

void exibir_resultado(vector<int>& requisicoes, int posicao_atual, int deslocamento) {
    cout << "Acessou " << formatar << posicao_atual << " " << requisicoes << " Já deslocou " << deslocamento << endl;
}

// Funções de escalonamento
void escalonador_fcfs(vector<int> requisicoes, int posicao_inicial) {
    int deslocamento = 0;
    exibir_original(requisicoes, posicao_inicial);
    cout << "\n\033[1m========== Algoritmo FCFS! ==========\033[0m" << endl;

    while(requisicoes.size() > 0) {
        int posicao_atual = requisicoes.front();
        deslocamento += abs(posicao_inicial - posicao_atual);
        posicao_inicial = posicao_atual;
        requisicoes.erase(requisicoes.begin());
        exibir_resultado(requisicoes, posicao_atual, deslocamento);
    }
    cout << "\033[1;33mFCFS - Quantidade total de deslocamentos: " << deslocamento << "\033[0m" << endl;
}

void escalonador_sstf(vector<int> requisicoes, int posicao_inicial) {
    int deslocamento = 0;
    exibir_original(requisicoes, posicao_inicial);
    cout << "\n\033[1m========== Algoritmo SSTF! ==========\033[0m" << endl;

    while(requisicoes.size() > 0) {
        int menor_diferenca = 100, indice;
        for(size_t i = 0; i < requisicoes.size(); i++) {
            int diferenca = abs(requisicoes[i] - posicao_inicial);
            if(diferenca < menor_diferenca) {
                menor_diferenca = diferenca;
                indice = i;
            }
        }
        int posicao_atual = requisicoes[indice];
        deslocamento += menor_diferenca;
        posicao_inicial = posicao_atual;
        requisicoes.erase(requisicoes.begin() + indice);
        exibir_resultado(requisicoes, posicao_atual, deslocamento);
    }
    cout << "\033[1;33mSSTF - Quantidade total de deslocamentos: " << deslocamento << "\033[0m" << endl;
}

void escalonador_scan(vector<int> requisicoes, int posicao_inicial) {
    int deslocamento = 0;
    bool fim_trilha = false;
    exibir_original(requisicoes, posicao_inicial);
    cout << "\n\033[1m========== Algoritmo Scan (elevador)! ==========\033[0m" << endl;

    while(requisicoes.size() > 0) {
        int posicao_atual = 0, indice = 0;
        bool requisicao_presente = false;
        
        for(size_t i = 0; i < requisicoes.size(); i++) {
            if((requisicoes[i] <= posicao_inicial && !fim_trilha) || (requisicoes[i] >= posicao_inicial && fim_trilha)) {
                if(!requisicao_presente || (fim_trilha && requisicoes[i] < requisicoes[indice]) || (!fim_trilha && requisicoes[i] > requisicoes[indice])) {
                    indice = i;
                }
                requisicao_presente = true;
            }
        }

        if(requisicao_presente) {
            posicao_atual = requisicoes[indice];
            deslocamento += abs(posicao_inicial - posicao_atual);
            posicao_inicial = posicao_atual;
            requisicoes.erase(requisicoes.begin() + indice);
        } else {
            if(fim_trilha) {
                fim_trilha = false;
                deslocamento += abs(posicao_inicial - 99);
                posicao_inicial = 99;
                posicao_atual = 99;
            } else {
                fim_trilha = true;
                deslocamento += abs(posicao_inicial - 0);
                posicao_inicial = 0;
                posicao_atual = 0;
            }
        }
        exibir_resultado(requisicoes, posicao_atual, deslocamento);
    }
    cout << "\033[1;33mSCAN - Quantidade total de deslocamentos: " << deslocamento << "\033[0m" << endl;
}

void escalonador_cscan(vector<int> requisicoes, int posicao_inicial) {
    int deslocamento = 0;
    vector<int> baixo, alto;
    exibir_original(requisicoes, posicao_inicial);
    cout << "\n\033[1m========== Algoritmo Circular SCAN! ==========\033[0m" << endl;

    for(size_t i = 0; i < requisicoes.size(); i++) {
        if (requisicoes[i] < posicao_inicial) baixo.push_back(requisicoes[i]);
        if (requisicoes[i] >= posicao_inicial) alto.push_back(requisicoes[i]);
    }

    stable_sort(baixo.begin(), baixo.end());
    stable_sort(alto.begin(), alto.end());

    for(size_t i = 0; i < alto.size(); i++) {
        int posicao_atual = alto[i];
        int diferenca = abs(posicao_atual - posicao_inicial);
        deslocamento += diferenca;
        posicao_inicial = posicao_atual;

        size_t j;
        for(j = 0; j < requisicoes.size(); j++) {
            if(requisicoes[j] == posicao_atual) break;
        }
        requisicoes.erase(requisicoes.begin() + j);
        exibir_resultado(requisicoes, posicao_atual, deslocamento);
    }

    if(requisicoes.size() > 0) {
        deslocamento += abs(posicao_inicial - 99);
        posicao_inicial = 0;
        if(posicao_inicial != 99) exibir_resultado(requisicoes, 99, deslocamento);
        deslocamento += 99;
        exibir_resultado(requisicoes, posicao_inicial, deslocamento);
    }

    for(size_t i = 0; i < baixo.size(); i++) {
        int posicao_atual = baixo[i];
        int diferenca = abs(posicao_atual - posicao_inicial);
        deslocamento += diferenca;
        posicao_inicial = posicao_atual;

        size_t j;
        for(j = 0; j < requisicoes.size(); j++) {
            if(requisicoes[j] == posicao_atual) break;
        }
        requisicoes.erase(requisicoes.begin() + j);
        exibir_resultado(requisicoes, posicao_atual, deslocamento);
    }
    cout << "\033[1;33mCircular SCAN - Quantidade total de deslocamentos: " << deslocamento << "\033[0m" << endl;
}

void escalonador_clook(vector<int> requisicoes, int posicao_inicial) {
    int deslocamento = 0, diferenca, posicao_atual;
    vector<int> baixo, alto;
    exibir_original(requisicoes, posicao_inicial);
    cout << "\n\033[1m========= Algoritmo C-look ==========\033[0m" << endl;

    for(size_t i = 0; i < requisicoes.size(); i++) {
        if (requisicoes[i] < posicao_inicial) baixo.push_back(requisicoes[i]);
        if (requisicoes[i] >= posicao_inicial) alto.push_back(requisicoes[i]);
    }

    stable_sort(baixo.begin(), baixo.end());
    stable_sort(alto.begin(), alto.end());

    for(size_t i = 0; i < alto.size(); i++) {
        posicao_atual = alto[i];
        diferenca = abs(posicao_atual - posicao_inicial);
        deslocamento += diferenca;
        posicao_inicial = posicao_atual;

        size_t j = 0;
        for(j = 0; j < requisicoes.size(); j++) {
            if(requisicoes[j] == posicao_atual) break;
        }
        requisicoes.erase(requisicoes.begin() + j);
        exibir_resultado(requisicoes, posicao_atual, deslocamento);
    }

    for(size_t i = 0; i < baixo.size(); i++) {
        posicao_atual = baixo[i];
        diferenca = abs(posicao_atual - posicao_inicial);
        deslocamento += diferenca;
        posicao_inicial = posicao_atual;

        size_t j;
        for(j = 0; j < requisicoes.size(); j++) {
            if (requisicoes[j] == posicao_atual) break;
        }
        requisicoes.erase(requisicoes.begin() + j);
        exibir_resultado(requisicoes, posicao_atual, deslocamento);
    }
    cout << "\033[1;33mC-look - Quantidade total de deslocamentos: " << deslocamento << "\033[0m" << endl;
}
