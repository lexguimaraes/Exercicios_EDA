#include "TARVB.h"

TARVB *TARVB_Cria(int t){
    TARVB* novo = (TARVB*)malloc(sizeof(TARVB));
    novo->nchaves = 0;
    novo->chave =(int*)malloc(sizeof(int*)*((t*2)-1));
    novo->folha=1;
    novo->filho = (TARVB**)malloc(sizeof(TARVB*)*t*2);
    int i;
    for(i = 0; i < (t*2); i++) novo->filho[i] = NULL;
    return novo;
}


TARVB *TARVB_Libera(TARVB *a){
    if(a){
        if(!a->folha){
            int i;
            for(i = 0; i <= a->nchaves; i++) TARVB_Libera(a->filho[i]);
        }
        free(a->chave);
        free(a->filho);
        free(a);
        return NULL;
    }
}


void imp_rec(TARVB *a, int andar){
    if(a){
        int i,j;
        for(i=0; i<=a->nchaves-1; i++){
            imp_rec(a->filho[i],andar+1);
            for(j=0; j<=andar; j++) printf("\t");
            printf("%d\n", a->chave[i]);
        }
        imp_rec(a->filho[i],andar+1);
    }
}


void TARVB_Imprime(TARVB *a){
    imp_rec(a, 0);
}

void imp_rec2(TARVB *a, int andar){
    if(a){
        int i,j;
        imp_rec2(a->filho[a->nchaves],andar+1);
        for(i=a->nchaves-1; i>=0; i--){
            for(j=0; j<=andar; j++) printf("\t");
            printf("%d\n", a->chave[i]);
            imp_rec2(a->filho[i],andar+1);
        }
    }
}


void TARVB_Imprime2(TARVB *a){
    imp_rec2(a, 0);
}

TARVB *TARVB_Busca(TARVB* x, int ch){
    if(!x) return NULL;
    int i = 0;
    while(i < x->nchaves && ch > x->chave[i]) i++;
    if(i < x->nchaves && ch == x->chave[i]) return x;
    if(x->folha) return NULL;
    return TARVB_Busca(x->filho[i], ch);
}


TARVB *TARVB_Inicializa(){
    return NULL;
}


TARVB *Divisao(TARVB *x, int i, TARVB* y, int t){
    TARVB *z=TARVB_Cria(t);
    z->nchaves= t - 1;
    z->folha = y->folha;
    int j;
    for(j=0;j<t-1;j++) z->chave[j] = y->chave[j+t];
    if(!y->folha){
        for(j=0;j<t;j++){
            z->filho[j] = y->filho[j+t];
            y->filho[j+t] = NULL;
        }
    }
    y->nchaves = t-1;
    for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
    x->filho[i] = z;
    for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
    x->chave[i-1] = y->chave[t-1];
    x->nchaves++;
    return x;
}


TARVB *Insere_Nao_Completo(TARVB *x, int k, int t){
    int i = x->nchaves-1;
    if(x->folha){
        while((i>=0) && (k<x->chave[i])){
            x->chave[i+1] = x->chave[i];
            i--;
        }
        x->chave[i+1] = k;
        x->nchaves++;
        return x;
    }
    while((i>=0) && (k<x->chave[i])) i--;
    i++;
    if(x->filho[i]->nchaves == ((2*t)-1)){
        x = Divisao(x, (i+1), x->filho[i], t);
        if(k>x->chave[i]) i++;
    }
    x->filho[i] = Insere_Nao_Completo(x->filho[i], k, t);
    return x;
}


TARVB *TARVB_Insere(TARVB *T, int k, int t){
    if(TARVB_Busca(T,k)) return T;
    if(!T){
        T=TARVB_Cria(t);
        T->chave[0] = k;
        T->nchaves=1;
        return T;
    }
    if(T->nchaves == (2*t)-1){
        TARVB *S = TARVB_Cria(t);
        S->nchaves=0;
        S->folha = 0;
        S->filho[0] = T;
        S = Divisao(S,1,T,t);
        S = Insere_Nao_Completo(S,k,t);
        return S;
    }
    T = Insere_Nao_Completo(T,k,t);
    return T;
}


TARVB *TARVB_Libera_Remocao(TARVB *a){
    free(a->chave);
    free(a->filho);
    free(a);
    return NULL;
}


TARVB* remover(TARVB* arv, int ch, int t){
    if(!arv) return arv;
    int i;
    printf("Removendo %d...\n", ch);
    for(i = 0; i<arv->nchaves && arv->chave[i] < ch; i++);
    if(i < arv->nchaves && ch == arv->chave[i]){ //CASOS 1, 2A, 2B e 2C
        if(arv->folha){ //CASO 1
            printf("\nCASO 1\n");
            int j;
            for(j=i; j<arv->nchaves-1;j++) arv->chave[j] = arv->chave[j+1];
            arv->nchaves--;
            if(!arv->nchaves){ //ultima revisao: 04/2020
                TARVB_Libera(arv);
                arv = NULL;
            }
            return arv;
        }
        if(!arv->folha && arv->filho[i]->nchaves >= t){ //CASO 2A
            printf("\nCASO 2A\n");
            TARVB *y = arv->filho[i];  //Encontrar o predecessor k' de k na árvore com raiz em y
            while(!y->folha) y = y->filho[y->nchaves];
            int temp = y->chave[y->nchaves-1];
            arv->filho[i] = remover(arv->filho[i], temp, t);
            //Eliminar recursivamente K e substitua K por K' em x
            arv->chave[i] = temp;
            return arv;
        }
        if(!arv->folha && arv->filho[i+1]->nchaves >= t){ //CASO 2B
            printf("\nCASO 2B\n");
            TARVB *y = arv->filho[i+1];  //Encontrar o sucessor k' de k na árvore com raiz em y
            while(!y->folha) y = y->filho[0];
            int temp = y->chave[0];
            y = remover(arv->filho[i+1], temp, t); //Eliminar recursivamente K e substitua K por K' em x
            arv->chave[i] = temp;
            return arv;
        }
        if(!arv->folha && arv->filho[i+1]->nchaves == t-1 && arv->filho[i]->nchaves == t-1){ //CASO 2C
            printf("\nCASO 2C\n");
            TARVB *y = arv->filho[i];
            TARVB *z = arv->filho[i+1];
            y->chave[y->nchaves] = ch;          //colocar ch ao final de filho[i]
            int j;
            for(j=0; j<t-1; j++)                //juntar chave[i+1] com chave[i]
                y->chave[t+j] = z->chave[j];
            for(j=0; j<t; j++){                 //juntar filho[i+1] com filho[i]
                y->filho[t+j] = z->filho[j];
                z->filho[j] = NULL; //ultima revisao: 04/2020
            }
            y->nchaves = 2*t-1;
            for(j=i; j < arv->nchaves-1; j++)   //remover ch de arv
                arv->chave[j] = arv->chave[j+1];
            for(j=i+1; j < arv->nchaves; j++)
                arv->filho[j] = arv->filho[j+1]; //remover ponteiro para filho[i+1]
            arv->filho[j] = NULL;
            TARVB_Libera_Remocao(z);
            arv->nchaves--;
            if(!arv->nchaves){ //ultima revisao: 04/2020
                TARVB *temp = arv;
                arv = arv->filho[0];
                temp->filho[0] = NULL;
                TARVB_Libera_Remocao(temp);
                arv = remover(arv, ch, t);
            }
            else arv->filho[i] = remover(arv->filho[i], ch, t);
            return arv;
        }
    }

    TARVB *y = arv->filho[i], *z = NULL;
    if (y->nchaves == t-1){ //CASOS 3A e 3B
        if((i < arv->nchaves) && (arv->filho[i+1]->nchaves >=t)){ //CASO 3A
            printf("\nCASO 3A: i menor que nchaves\n");
            z = arv->filho[i+1];
            y->chave[t-1] = arv->chave[i];   //dar a y a chave i da arv
            y->nchaves++;
            arv->chave[i] = z->chave[0];     //dar a arv uma chave de z
            int j;
            for(j=0; j < z->nchaves-1; j++)  //ajustar chaves de z
                z->chave[j] = z->chave[j+1];
            y->filho[y->nchaves] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
            for(j=0; j < z->nchaves; j++)       //ajustar filhos de z
                z->filho[j] = z->filho[j+1];
            z->nchaves--;
            arv->filho[i] = remover(arv->filho[i], ch, t);
            return arv;
        }
        if((i > 0) && (!z) && (arv->filho[i-1]->nchaves >=t)){ //CASO 3A
            printf("\nCASO 3A: i igual a nchaves\n");
            z = arv->filho[i-1];
            int j;
            for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chave
                y->chave[j] = y->chave[j-1];
            for(j = y->nchaves+1; j>0; j--)             //encaixar lugar dos filhos da nova chave
                y->filho[j] = y->filho[j-1];
            y->chave[0] = arv->chave[i-1];              //dar a y a chave i da arv
            y->nchaves++;
            arv->chave[i-1] = z->chave[z->nchaves-1];   //dar a arv uma chave de z
            y->filho[0] = z->filho[z->nchaves];         //enviar ponteiro de z para o novo elemento em y
            z->nchaves--;
            arv->filho[i] = remover(y, ch, t);
            return arv;
        }
        if(!z){ //CASO 3B
            if(i < arv->nchaves && arv->filho[i+1]->nchaves == t-1){
                printf("\nCASO 3B: i menor que nchaves\n");
                z = arv->filho[i+1];
                y->chave[t-1] = arv->chave[i];     //pegar chave [i] e coloca ao final de filho[i]
                y->nchaves++;
                int j;
                for(j=0; j < t-1; j++){
                    y->chave[t+j] = z->chave[j];     //passar filho[i+1] para filho[i]
                    y->nchaves++;
                }
                if(!y->folha){
                    for(j=0; j<t; j++){
                        y->filho[t+j] = z->filho[j];
                        z->filho[j] = NULL; //ultima revisao: 04/2020
                    }
                }
                TARVB_Libera(z);
                for(j=i; j < arv->nchaves-1; j++){ //limpar referências de i
                    arv->chave[j] = arv->chave[j+1];
                    arv->filho[j+1] = arv->filho[j+2];
                }
                arv->filho[arv->nchaves] = NULL;
                arv->nchaves--;
                if(!arv->nchaves){ //ultima revisao: 04/2020
                    TARVB *temp = arv;
                    arv = arv->filho[0];
                    temp->filho[0] = NULL;
                    TARVB_Libera(temp);
                }
                arv = remover(arv, ch, t);
                return arv;
            }
            if((i > 0) && (arv->filho[i-1]->nchaves == t-1)){
                printf("\nCASO 3B: i igual a nchaves\n");
                z = arv->filho[i-1];
                if(i == arv->nchaves)
                    z->chave[t-1] = arv->chave[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
                else
                    z->chave[t-1] = arv->chave[i];   //pegar chave [i] e poe ao final de filho[i-1]
                z->nchaves++;
                int j;
                for(j=0; j < t-1; j++){
                    z->chave[t+j] = y->chave[j];     //passar filho[i+1] para filho[i]
                    z->nchaves++;
                }
                if(!z->folha){
                    for(j=0; j<t; j++){
                        z->filho[t+j] = y->filho[j];
                        y->filho[j] = NULL; //ultima revisao: 04/2020
                    }
                }
                TARVB_Libera(y);
                arv->filho[arv->nchaves] = NULL;
                arv->nchaves--;
                if(!arv->nchaves){ //ultima revisao: 04/2020
                    TARVB *temp = arv;
                    arv = arv->filho[0];
                    temp->filho[0] = NULL;
                    TARVB_Libera(temp);
                }
                else arv->filho[i-1] = z;
                arv = remover(arv, ch, t);
                return arv;
            }
        }
    }
    arv->filho[i] = remover(arv->filho[i], ch, t);
    return arv;
}


TARVB* TARVB_Retira(TARVB* arv, int k, int t){
    if(!arv || !TARVB_Busca(arv, k)) return arv;
    return remover(arv, k, t);
}
