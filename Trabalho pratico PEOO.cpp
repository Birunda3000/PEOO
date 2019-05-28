/*UNIVERSIDADE ESTADUAL DO CEARA
Trabalho pratico de PEOO
Professor: Matheus Magalhães
Equipe: Erick Santos do Nascimento, Otoniel Mateus Teles Cardoso, Eduardo Sena Cavalcante*/
#include <iostream>
#include <stdio.h>
//#include <Windows.h>// Função Sleep
//#include <thread> bibliotecas de  um compilador não padrão
//#include <chrono>
#include <cstdlib>// rand e srand
#include <time.h>
#include <vector>
#include <string>
using namespace std;
enum TipoElemento{pirata, marinha, espacovazio, obstaculo, onepiece};
enum Movimento{Up = 1, Down, Left, Right};
class Elemento{
    protected:
        string nome;
        TipoElemento tipo;
    public:
        Elemento(string nomeInicial, TipoElemento tipoInicial){
            nome = nomeInicial;
            tipo = tipoInicial;
        }
        virtual void SetNome(string novoNome) = 0;
        virtual void SetTipo(TipoElemento novoTipo) = 0;
        virtual string GetNome() = 0;
        virtual TipoElemento GetTipo() = 0;
};
class Pessoa : public Elemento{
    private:
        int hp;
    public:
        Pessoa(int hpInicial, string nomeInicial, TipoElemento tipoInicial) : Elemento(nomeInicial, tipoInicial){
            hp = hpInicial;
        }
        void Sethp(int novoHp){
            hp = novoHp;
        }
        int Gethp(){
            return hp;
        }
        void SetNome(string novoNome){
            nome = novoNome;
        }
        void SetTipo(TipoElemento novoTipo){
            tipo = novoTipo;
        }
        string GetNome(){
            return nome;
        }
        TipoElemento GetTipo(){
            return tipo;
        }
};
class Pirata : public Pessoa{
    private:
        int peso;
        int pesoAdicional;
        bool tesouro;
    public:
        Pirata(int pesoInicial, int pesoAdicionalInicial, int hpInicial) : Pessoa(hpInicial, "Luffy", pirata){
            peso = pesoInicial;
            pesoAdicional = pesoAdicionalInicial;
            tesouro = false;
        }
        void SetPeso(int novoPeso){
            peso = novoPeso;
        }
        void SetPesoAdicional(int novoPesoAdicional){
            pesoAdicional = novoPesoAdicional;
        }
        int GetPeso(){
           return peso;
        }
        int GetPesoAdicional(){
            return pesoAdicional;
        }
        void SetTesouro(bool novoTesouro){
            tesouro = novoTesouro;
        }
};
class Marinha : public Pessoa{
    private:
        bool estado;
    public:
        Marinha(int hpInicial) : Pessoa(hpInicial, "Marinha", marinha){
            estado = false;
        }
        void SetEstado(bool novoEstado){
            estado = novoEstado;
        }
        bool GetEstado(){
            return estado;
        }
};
class OnePiece : public Elemento{
    private:
        int peso;
    public:
        OnePiece(int pesoInicial) : Elemento("One Piece", onepiece){
            peso = pesoInicial;
        }
        int GetPeso(){
            return peso;
        }
        void SetPeso(int novoPeso){
            peso = novoPeso;
            if(peso < 0){
                peso = 0;
            }
        }
        void SetNome(string novoNome){
            nome = novoNome;
        }
        void SetTipo(TipoElemento novoTipo){
            tipo = novoTipo;
        }
        string GetNome(){
            return nome;
        }
        TipoElemento GetTipo(){
            return tipo;
        }
};
class GrandLine{
    private:
        int n;//dimensão do mapa
        vector< vector<char> > ponteiroMapa;//ponteiro para o mapa

        bool carregandoOnePiece;//diz se o pirata esta carregando o one piece (true = carregando / false = não carregando)
        int turnoMarinhaAtiva;//informa o numero de turnos seguidos em que marinha esta ativa (true = ativa / false = dormindo)
        bool marinhaAtiva;//informa se a marinha esta ativa (true = ativa / false = dormindo)

        int nOnePiece;//numero de vezes que o pirata pegou o one piece
        int marinhaLuffy;//informa o numero de vezes que o pirata foi capturado pela marinha

        int lLuffy;//coordenada x atual do pirata
        int cLuffy;//coordenada y atual do pirata

        int lMarinha;//coordenada x atual da marinha
        int cMarinha;//coordenada y atual da marinha

        int ultimoMovimento;//salva o ultimo movimento realizado pelo pirata
        int pesoRestante;//indica o peso restante do tesouro
    public:
        //OBS: NO MAPA O CARACTERE "|" REPRESENTA UM OBSTACULO, "L" PIRATA, "v" ESPAÇO VAZIO E "M" MARINHA
        void Inicializar(int dimensaoDoMapa){//metodo incializar
            int i, c, aleatorioLinha, aleatorioColuna;
            turnoMarinhaAtiva = 0;
            marinhaAtiva = false;
            carregandoOnePiece = false;
            nOnePiece = 0;
            marinhaLuffy = 0;
            n = dimensaoDoMapa;
            char o;
            vector <char> linha;

            for (i = 0; i < n; i++){//aloca o mapa
                linha.push_back(o);
            }
            for (i = 0; i < n; i++){//aloca o mapa
                ponteiroMapa.push_back(linha);
            }

            for(i = 0; i < n ; i++){//iguala todos os pontos a v
                for(c = 0 ; c < n ; c++){
                    ponteiroMapa [i][c] = 'v';
                }
            }
            ponteiroMapa[0][0] = 'L';
            lLuffy = 0;
            cLuffy = 0;
            ponteiroMapa [n-1][n-1] = 'T';
            srand(time (NULL));
            do{
                aleatorioLinha = rand() % n;
                aleatorioColuna = rand() % n;
            }while(ponteiroMapa[aleatorioLinha] [aleatorioColuna] != 'v');
            ponteiroMapa[aleatorioLinha] [aleatorioColuna] = 'M';
            lMarinha = aleatorioLinha;
            cMarinha = aleatorioColuna;
            for(i=0; i<n/2 ; i++){//****N determina o numero de obstaculos****
                do{
                    aleatorioLinha = rand() % n;
                    aleatorioColuna = rand() % n;
                }while(ponteiroMapa[aleatorioLinha] [aleatorioColuna] != 'v' || (aleatorioLinha == 0 || aleatorioColuna == n-1));
                ponteiroMapa[aleatorioLinha] [aleatorioColuna] = '|';
            }
        }
        void VizualizarMapa(){//Esse mmetodo printa o mapa na tela
            int l = 0;
            int c = 0;
            cout<<"\n\n\n\t**MAPA**\n\n"<<endl;
            for(l = 0; l < n ; l++){
                cout<<"   ";
                for(c = 0 ; c < n ; c++){
                    cout<<ponteiroMapa [l] [c]<<"    ";
                }
                cout<<"\n"<<endl;
            }
            cout<<"\n\n----------------------------------\n\n"<<endl;
        }
        void AndarPirata(){//Metodo que realiza o movimento do pirata
            int movimento, i;
            bool movimentoValido = false;
            while(movimentoValido == false){
                movimento = rand() % 4;
                //Esse trecho é responsavel pelo pirata saber onde esta o tesouro se estiver proximo a ele
                if(lLuffy == n - 1 && (cLuffy == n - 3 || cLuffy == n-2) && (ponteiroMapa[n-1] [n-2] != '|' && ponteiroMapa[n-1] [n-2] != 'M') && carregandoOnePiece == false){//sabe onde esta o tesouro
                    movimento = 3;//direita
                }
                if((lLuffy == n - 3 || lLuffy == n - 2) && cLuffy == n-1 && (ponteiroMapa[n-2] [n-1] != '|' && ponteiroMapa[n-2] [n-1] != 'M') && carregandoOnePiece == false){//sabe onde esta o tesouro
                    movimento = 1;//baixo
                }
                //fim do trecho citado acima
                switch(movimento){
                    case 0:{//cima
                        if(lLuffy - 1 >= 0){
                            if(ponteiroMapa[lLuffy-1] [cLuffy] == '|'){
                                cout<<"Luffy se chocou contra um obstaculo"<<endl;
                                marinhaAtiva = true;
                            }
                        }
                        else{
                            cout<<"Luffy tentando sair da Grand Line"<<endl;
                        }
                        if(lLuffy - 1 >= 0 && (ponteiroMapa[lLuffy-1] [cLuffy] == 'v' || ponteiroMapa[lLuffy-1] [cLuffy] == 'T')){
                            if (ponteiroMapa[lLuffy-1] [cLuffy] == 'T'){
                                carregandoOnePiece = true;
                            }
                            ponteiroMapa[lLuffy][cLuffy] = 'v';
                            ponteiroMapa[lLuffy-1] [cLuffy] = 'L';
                            lLuffy = lLuffy-1;
                            movimentoValido = true;
                            ultimoMovimento = 1;
                        }
                        break;
                    }
                    case 1:{//baixo;
                        if(lLuffy + 1 < n){
                            if(ponteiroMapa[lLuffy+1] [cLuffy] == '|'){
                                cout<<"Luffy se chocou contra um obstaculo"<<endl;
                                marinhaAtiva = true;
                            }
                        }
                        else{
                            cout<<"Luffy tentando sair da Grand Line"<<endl;
                        }
                        if(lLuffy + 1 < n && (ponteiroMapa[lLuffy+1] [cLuffy] == 'v' || ponteiroMapa[lLuffy+1] [cLuffy] == 'T')){
                            if (ponteiroMapa[lLuffy+1] [cLuffy] == 'T'){
                                carregandoOnePiece = true;
                            }
                            ponteiroMapa[lLuffy][cLuffy] = 'v';
                            ponteiroMapa[lLuffy+1] [cLuffy] = 'L';
                            lLuffy = lLuffy+1;
                            movimentoValido = true;
                            ultimoMovimento = 0;
                        }
                        break;
                    }
                    case 2:{//esquerda
                        if(cLuffy - 1 >= 0){
                            if(ponteiroMapa[lLuffy] [cLuffy-1] == '|'){
                                cout<<"Luffy se chocou contra um obstaculo"<<endl;
                                marinhaAtiva = true;
                            }
                        }
                        else{
                            cout<<"Luffy tentando sair da Grand Line"<<endl;
                        }
                        if(cLuffy - 1 >= 0 && (ponteiroMapa[lLuffy] [cLuffy-1] == 'v' || ponteiroMapa[lLuffy] [cLuffy-1] == 'T')){
                            if (ponteiroMapa[lLuffy] [cLuffy-1] == 'T'){
                                carregandoOnePiece = true;
                            }
                            ponteiroMapa[lLuffy][cLuffy] = 'v';
                            ponteiroMapa[lLuffy] [cLuffy-1] = 'L';
                            cLuffy = cLuffy-1;
                            movimentoValido = true;
                            ultimoMovimento = 3;
                        }
                        break;
                    }
                    case 3:{//direita
                        if(cLuffy + 1 < n){
                            if(ponteiroMapa[lLuffy] [cLuffy+1] == '|'){
                                cout<<"Luffy se chocou contra um obstaculo"<<endl;
                                marinhaAtiva = true;
                            }
                        }
                        else{
                            cout<<"Luffy tentando sair da Grand Line"<<endl;
                        }
                        if(cLuffy + 1 < n && (ponteiroMapa[lLuffy] [cLuffy+1] == 'v' || ponteiroMapa[lLuffy] [cLuffy+1] == 'T')){
                            if (ponteiroMapa[lLuffy] [cLuffy+1] == 'T'){
                                carregandoOnePiece = true;
                            }
                            ponteiroMapa[lLuffy][cLuffy] = 'v';
                            ponteiroMapa[lLuffy] [cLuffy+1] = 'L';
                            cLuffy = cLuffy+1;
                            movimentoValido = true;
                            ultimoMovimento = 2;
                        }
                        break;
                    }
                }
            }//fim do while
        }//fim do metodo
        void AndarMarinha(){//Esse metodo realiza o movimento da marinha
            int movimento, i;
            bool movimentoValido = false;
            movimento = ultimoMovimento;
            switch(movimento){
                case 0:{//cima
                    if(lMarinha - 1 >= 0){
                        if(ponteiroMapa[lMarinha-1] [cMarinha] == '|'){
                            cout<<"Marinha se chocou contra um obstaculo"<<endl;
                        }
                    }
                    else{
                        cout<<"Marinha tentando sair da Grand Line"<<endl;
                    }
                    if(lMarinha - 1 >= 0 && (ponteiroMapa[lMarinha-1] [cMarinha] == 'v' || ponteiroMapa[lMarinha-1] [cMarinha] == 'L')){
                        if (ponteiroMapa[lMarinha-1] [cMarinha] == 'L'){
                            marinhaLuffy++;
                        }
                        ponteiroMapa[lMarinha][cMarinha] = 'v';
                        ponteiroMapa[lMarinha-1] [cMarinha] = 'M';
                        lMarinha = lMarinha-1;
                        movimentoValido = true;
                    }
                    break;
                }
                case 1:{//baixo;
                    if(lMarinha + 1 < n){
                        if(ponteiroMapa[lMarinha+1] [cMarinha] == '|'){
                            cout<<"Marinha se chocou contra um obstaculo"<<endl;
                        }
                    }
                    else{
                        cout<<"Marinha tentando sair da Grand Line"<<endl;
                    }
                    if(lMarinha + 1 < n && (ponteiroMapa[lMarinha+1] [cMarinha] == 'v' || ponteiroMapa[lMarinha+1] [cMarinha] == 'L')){
                        if (ponteiroMapa[lMarinha+1] [cMarinha] == 'L'){
                            marinhaLuffy ++;
                        }
                        ponteiroMapa[lMarinha][cMarinha] = 'v';
                        ponteiroMapa[lMarinha+1] [cMarinha] = 'M';
                        lMarinha = lMarinha+1;
                        movimentoValido = true;
                    }
                    break;
                }
                case 2:{//esquerda
                    if(cMarinha - 1 >= 0){
                        if(ponteiroMapa[lMarinha] [cMarinha-1] == '|'){
                            cout<<"Marinha se chocou contra um obstaculo"<<endl;
                        }
                    }
                    else{
                        cout<<"Marinha tentando sair da Grand Line"<<endl;
                    }
                    if(cMarinha - 1 >= 0 && (ponteiroMapa[lMarinha] [cMarinha-1] == 'v' || ponteiroMapa[lMarinha] [cMarinha-1] == 'L')){
                        if (ponteiroMapa[lMarinha] [cMarinha-1] == 'L'){
                            marinhaLuffy ++;
                        }
                        ponteiroMapa[lMarinha][cMarinha] = 'v';
                        ponteiroMapa[lMarinha][cMarinha-1] = 'M';
                        cMarinha = cMarinha-1;
                        movimentoValido = true;
                    }
                    break;
                }
                case 3:{//direita
                    if(cMarinha + 1 < n){
                        if(ponteiroMapa[lMarinha] [cMarinha+1] == '|'){
                            cout<<"Marinha se chocou contra um obstaculo"<<endl;
                        }
                    }
                    else{
                        cout<<"Marinha tentando sair da Grand Line"<<endl;
                    }
                    if(cMarinha + 1 < n && (ponteiroMapa[lMarinha] [cMarinha+1] == 'v' || ponteiroMapa[lMarinha] [cMarinha+1] == 'L')){
                        if (ponteiroMapa[lMarinha] [cMarinha+1] == 'L'){
                            marinhaLuffy ++;
                        }
                        ponteiroMapa[lMarinha][cMarinha] = 'v';
                        ponteiroMapa[lMarinha][cMarinha+1] = 'M';
                        cMarinha = cMarinha+1;
                        movimentoValido = true;
                    }
                    break;
                }
            }
        }//fim do metodo
        bool GetEstadoMarinha(){
            return marinhaAtiva;
        }
        int GetMarinhaLuffy(){
            return marinhaLuffy;
        }
        bool GetCarregandoOnePiece(){
            return carregandoOnePiece;
        }
        int GetNOnePiece(){
            return nOnePiece;
        }
        void SetPesoRestante(int novoPesoRestante){
            pesoRestante = novoPesoRestante;
        }
        int Descarregar(){//verifica se o pirata esta com o tesouro e no inicio do mapa e descarrega o tesouro
            if(carregandoOnePiece == true && lLuffy == 0 && cLuffy == 0){
                carregandoOnePiece = false;
                nOnePiece++;
            }
        }
        void Ciclo(){//Junta varios metodos necessarios para a movimentação do pirata e marinha, descarregar o one piece
            AndarPirata();
            Descarregar();
            ponteiroMapa[lMarinha][cMarinha] = 'M';
            if(marinhaAtiva == true){
                AndarMarinha();
                turnoMarinhaAtiva++;
                if(turnoMarinhaAtiva >= 2){
                    marinhaAtiva = false;
                    turnoMarinhaAtiva = 0;
                }
            }
            if(pesoRestante > 0 && (pesoRestante < 70 && carregandoOnePiece == true) == false){
                ponteiroMapa [n-1] [n-1] = 'T';
            }
            VizualizarMapa();
        }
};
int main(){
    long long int i = 0;
    GrandLine mapa;
    Pirata luffy (70, 0, 100);
    Marinha marinha(100);
    OnePiece tesouro (300);
    mapa.Inicializar(7);
    mapa.VizualizarMapa();
//    Sleep(4000);
    while(luffy.Gethp() > 0 && tesouro.GetPeso() > 0){
        mapa.Ciclo();

        //Atualiza as informações nas classes
        tesouro.SetPeso (300 - (luffy.GetPeso() * mapa.GetNOnePiece()));
        mapa.SetPesoRestante(tesouro.GetPeso());

        luffy.Sethp(100 - (10 * mapa.GetMarinhaLuffy()));
        luffy.SetTesouro(mapa.GetCarregandoOnePiece());

        if(tesouro.GetPeso() >= luffy.GetPeso()){//se o peso for igual ou maior que  do pirata
            luffy.SetPesoAdicional(luffy.GetPeso() * mapa.GetCarregandoOnePiece());
        }
        else{//caso o peso atual do tesouro seja menor que o do pirata
            luffy.SetPesoAdicional(tesouro.GetPeso() * mapa.GetCarregandoOnePiece());
        }
        marinha.SetEstado(mapa.GetEstadoMarinha());
        //Fim da troca de informaçõesentre as classes

        i++;
        cout<<"Movimentos realizados: "<<i<<endl;
        cout<<"Pirata Hp restante: "<<luffy.Gethp()<<endl;
        cout<<"Restam "<<tesouro.GetPeso()<<" kg de tesouro a Grand Line"<<endl;
        cout<<"_____________________________________________________________________________________________________________________________________________________"<<endl;
//        Sleep(550);
    }
//    mapa.VizualizarMapa();
    return 0;
}
