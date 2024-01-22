#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

typedef struct parque{
	int piso;
	int linha;
	int coluna;
	float tarifa;
	char parque[100][100][100]; //este parque fica uma variável em memória
}Parque;

typedef struct data{
    int dia;
    int mes; 
    int ano; 
    int hora; 
    int minuto;
    int segundo;
}Data;

typedef struct carro{
	char matricula[9];
	int piso;
	int linha;
	int coluna;
	Data hora_entrada;
	Data hora_saida;
	float preco;
}*Carro;

void hora_atual(Data* data){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	
	data->ano = tm.tm_year + 1900;
	data->mes = tm.tm_mon + 1;
	data->dia = tm.tm_mday;
	data->hora = tm.tm_hour;
	data->minuto = tm.tm_min;	
	data->segundo = tm.tm_sec;
}

void criar_parque(Parque* p){
	int cp, cl, cc;

 printf("---------------- SISTEMA DE ESTACIONAMENTO -------------\n");
 printf("Dimensoes do parque:\n");
	
	printf("Insira os pisos que o parque vai ter: ");
	scanf("%d", &p->piso);
	printf("Insira as linhas que o parque vai ter: ");
	scanf("%d", &p->linha);
	printf("Insira as colunas que o parque vai ter: ");
	scanf("%d", &p->coluna);
	printf("Insira a tarifa do parque(por hora): ");
	scanf("%f", &p->tarifa);
	
	if(p->piso <= 0 || p->linha <= 0 || p->coluna <= 0){
		printf("Dados invalidos, o programa sera reinciado!");
		exit(0);
	}else
		printf("Parque criado com sucesso!\n");
	
	
		for(cp=0; cp < p->piso; cp++)
			for(cl=0; cl < p->linha; cl++)
				for(cc=0; cc < p->coluna; cc++)
					p->parque[cp][cl][cc] = 'L';
	system("pause");
	system("cls");	
			
}

void mostrar_parque(Parque p){
	
	int contadorpisos = 1;
	int cp, cl, cc;
	
	for(cp=0; cp<p.piso; cp++){
		printf("Piso %d:\n" , contadorpisos);
		contadorpisos++;
		for(cl=0; cl<p.linha; cl++){
			for(cc=0; cc<p.coluna; cc++){
				printf(" %c ", p.parque[cp][cl][cc]);
			}
			printf("\n");
		}
		printf("\n");
	}	
}

int disponiveis(Parque p){
	
	int cp, cl, cc;
	int disponiveis = 0, totais = 0;
	
	for(cp=0; cp < p.piso; cp++)	
		for(cl=0; cl < p.linha; cl++)	
			for(cc=0; cc < p.coluna; cc++)
				if(p.parque[cp][cl][cc] == 'L')
					disponiveis++;							
	return disponiveis;	
}

int verificar_matricula(Carro c, Parque* p, int* contador, char matricula[]){
	
	int i;
	
	for(i = 0; i < *contador; i++)
		if(stricmp(c[i].matricula, matricula) == 0)
		
		return 1;
		else
		return 0;
		
}
	
void estacionar(Carro c, Parque* p, int* contador){
	
	int cp, cl, cc;
	int piso, linha, coluna;
	int totais = 0, disponiveis= 0;
	char temp[9]; //variável temporária para guardar a matricula, porque se nao ia guardar a matricula que ja há na struct
	
	printf("Insira a matricula da viatura (ex ##-##-##): ");
	scanf("%s", temp);
	
	if(strlen(temp) < 8 || temp[2] != '-' || temp[5] != '-'){
		printf("Matricula invalida!\n");
		return;	
	}

	if(verificar_matricula(c, p, contador, temp) == 1){
		printf("Ja existe uma matricula igual estacionada no parque!");
		return;
	}
	
	do{
		printf("Insira o piso que deseja estacionar: ");
		scanf("%d", &piso);
		printf("Insira a linha que deseja estacionar: ");
		scanf("%d", &linha);
		printf("Insira a coluna que deseja estacionar: ");
		scanf("%d", &coluna);
		
	
	
		if(piso > p->piso|| linha > p->linha || coluna > p->coluna || piso <= 0 || linha <= 0 || coluna <= 0){
			puts("Lugar introduzido nao existe");
			continue;
		}
		
		
		if(p->parque[--piso][--linha][--coluna] == 'O'){
			puts("Lugar ocupado");	
			continue;
		}
		
		c=realloc(c, sizeof(struct carro) * (*contador+1));
		p->parque[piso][linha][coluna] = 'O';
		hora_atual(&c[*contador].hora_entrada);
		strcpy(c[*contador].matricula, temp);
		c[*contador].piso = ++piso;
		c[*contador].linha = ++linha;
		c[*contador].coluna = ++coluna;
		printf("Lugar estacionado com sucesso!\n");
		printf("Informacao do estacionamento: \nPiso: %d  \nLinha: %d \nColuna: %d\n", piso, linha, coluna);
		printf("Data de entrada: %02d:%02d:%02d\t%02d/%02d/%04d\n", c[*contador].hora_entrada.hora, c[*contador].hora_entrada.minuto, c[*contador].hora_entrada.segundo, c[*contador].hora_entrada.dia, c[*contador].hora_entrada.mes, c[*contador].hora_entrada.ano);
		(*contador)++;
		break;
	}while(1);
}

void retirar(Carro c, Parque* p, int* contador){
	char matricula[9];
	int i;
	printf("Introduza a matricula da viatura que pretende retirar: ");
	scanf("%s", matricula);
	
	if(verificar_matricula(c, p, contador, matricula) == 0){
		printf("Esta matricula nao existe!\n");
		return;
	}
	
	for(i = 0; i < *contador; i++)
		if(stricmp(c[i].matricula, matricula) == 0){
			p->parque[c[i].piso - 1][c[i].linha - 1][c[i].coluna - 1] = 'L';
			puts("Carro removido com sucesso!");	
			hora_atual(&c[i].hora_saida);
			printf("Data de saida: %02d:%02d:%02d\t%02d/%02d/%04d\n", c[i].hora_saida.hora, c[i].hora_saida.minuto, c[i].hora_saida.segundo, c[i].hora_saida.dia, c[i].hora_saida.mes, c[i].hora_saida.ano);
			hora_atual(&c->hora_saida);
    		float hora_saida = c->hora_saida.hora*3600+c->hora_saida.minuto*60+c->hora_saida.segundo;
    		float hora_entrada = c->hora_entrada.hora*3600+c->hora_entrada.minuto*60+c->hora_entrada.segundo;
    		float tempo = (hora_saida-hora_entrada)/3600;
    		c->preco = ceil(tempo)*p->tarifa; //ceil: arredonda as horas para a unidade cima. Pois caso o carro estiver só 1 segundo na parque, ele mesmo assim vai pagar o preço da hora
    		printf("Esteve %f horas no parque\n", tempo);
    		printf("Preco a pagar: %f\n", c->preco);		
			break;
		}	 

	for(i; i < *contador - 1; i++){  //basicamente ''ordenar'' a minha lista de carros
		c[i] = c[i+1];			
	}
	(*contador)--;  //retiro um carro, logo o contador diminui
	//c = realloc(c, sizeof(struct carro) * (*contador)); //retirar a memoria que o carro que retirei tinha

}

void listar(Carro c, int contador){
	int i;
	for(i = 0; i < contador; i++){
		printf("Matricula: %s\n", c[i].matricula);
		printf("Lugar: | Piso:  %d | Linha: %d | Coluna: %d\n", c[i].piso, c[i].linha, c[i].coluna);
		printf("Data de entrada: %02d:%02d:%02d\t%02d/%02d/%04d\n", c[i].hora_entrada.hora, c[i].hora_entrada.minuto, c[i].hora_entrada.segundo, c[i].hora_entrada.dia, c[i].hora_entrada.mes, c[i].hora_entrada.ano);
	}
	
	if(contador == 0){
		printf("Nao existem carros estacionados neste parque!\n");
	}
	
}

main(){
	Parque p;
	Carro carros = malloc(sizeof(struct carro));
	int opcao;
	int contador = 0; 
	
	criar_parque(&p);
	
	
//	Carro c = (Carro) malloc(sizeof(Carro));
    //int *contador = malloc(sizeof(int));
    

	do{
        printf("\n1.LISTAR O PARQUE DE ESTACIONAMENTO");
        printf("\n2.ESTACIONAR O CARRO");
        printf("\n3.LISTAR OS CARROS");
        printf("\n4.REMOVER O CARRO");
        printf("\n0.EXIT");

        printf("\nESCOLHA O QUE PRETENDE FAZER\n");
        scanf("%d", &opcao);

        switch(opcao){
            case 1: 
            	mostrar_parque(p);  
				system("pause");
				system("cls");      
            	break;
            case 2:
            	if(disponiveis(p)){
            		printf("Lugares disponiveis: %d\n", disponiveis(p));
            		estacionar(carros, &p, &contador);		
				}else
					printf("Nao ha lugares disponiveis!\n");	
				system("pause");
				system("cls");
            	break;
            	
            case 3: 
				listar(carros, contador);
				system("pause");
				system("cls");
                break;

            case 4: 
				if(contador == 0)
					puts("Nao existem carros estacionados neste parque!");		
				else
					retirar(carros, &p, &contador);	
				system("pause");
				system("cls");
                break;

            case 0: 
            	system("cls");
            	printf("Trabalho realizado por: \nJoao Mendes \nAlexandre Antunes \nCatarina Araujo");	
                exit(0);
                break;

            default: 
                 printf("Valor invalido, tente outra opcao!");
        }

    }while(1);
    
    
	
	
}
