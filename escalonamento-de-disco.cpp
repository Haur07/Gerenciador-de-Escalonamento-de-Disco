#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <ctime>
using namespace std;

ostream& formatar(ostream& os) {
    return os << setfill('0') << setw(2);
}

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

void inserir_vetor(vector<int>& requisicoes);
void exibir_original(vector<int>& requisicoes, int posicao_inicial);
void exibir_resultado(vector<int>& requisicoes, int posicao_atual, int deslocamento);

void escalonador_fcfs(vector<int>& requisicoes, int posicao_atual);

int main() {
    int posicao_inicial = 88;
    vector<int> requisicoes;

    cout << "===== Gerenciador de Escalonamento de Acesso ao Disco =====" << endl;
    inserir_vetor(requisicoes);
    escalonador_fcfs(requisicoes, posicao_inicial);

    return 0;
}

void inserir_vetor(vector<int>& requisicoes) {
    int lista_rebonatto[10] = {30, 70, 54, 59, 29, 64, 87, 38, 5, 40};
    //srand((unsigned) time(NULL));
    for(int i = 0; i < 10; i++) {
        //int valor_gerado = 1 + (rand() % 99);
        requisicoes.push_back(lista_rebonatto[i]);
    }
}

void exibir_original(vector<int>& requisicoes, int posicao_inicial) {
    cout << "\nPosição inicial da cabeça de leitura e gravação: " << posicao_inicial << endl;
    cout << "Original: " << requisicoes << endl;
}

void exibir_resultado(vector<int>& requisicoes, int posicao_atual, int deslocamento) {
    cout << "Acessou " << formatar << posicao_atual << " " << requisicoes << " Já deslocou " << deslocamento << endl;
}

void escalonador_fcfs(vector<int>& requisicoes, int posicao_inicial) {
    int deslocamento = 0;
    exibir_original(requisicoes, posicao_inicial);
    cout << "\n========== Algoritmo FCFS! ==========" << endl;
    while(requisicoes.size() > 0) {
        int posicao_atual = requisicoes.front();
        deslocamento += abs(posicao_inicial - posicao_atual);
        posicao_inicial = posicao_atual;
        requisicoes.erase(requisicoes.begin());
        exibir_resultado(requisicoes, posicao_atual, deslocamento);
    }
    cout << "FCFS - Quantidade total de deslocamentos: " << deslocamento << endl;
}
