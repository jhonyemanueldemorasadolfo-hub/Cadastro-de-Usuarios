#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include<windows.h>
#include<mysql.h>

//===============================
//Structs e Vetores Globais
//===============================

typedef struct {
	char nomeUsuario[50];
	char Senha[50];
	
} cadastro;

cadastro cadastros[100];

int nivel;
int UsuarioLogado;
int qtdecadastros = 0;


//===============================
//Prototipo das Funções
//===============================
void tela_inicial(MYSQL* conexao);
void Cadastro(MYSQL* conexao);
void login(MYSQL* conexao);
void menuusuario(MYSQL* conexao);
void menuADM(MYSQL* conexao);

//===============================
//Função de Conexão com o Banco de Dados
//===============================
int bancodedados() {

	MYSQL* conexao;

	conexao = mysql_init(NULL);

	if (conexao == NULL) {
		printf("Erro no mysql_init");
		return 1;
	}

	conexao = mysql_real_connect(
		conexao,
		"localhost",
		"root",
		"SENHA_AQUI",
		"sistema_usuarios",
		3306,
		NULL,
		0
	);

	if (conexao) {
		tela_inicial(conexao);
	}
	else {
		printf("Erro na conexao");
	}

	mysql_close(conexao);

	return 0;
}

//===============================
//Função Principal
//===============================
int main() {
	system("color 0C");
	setlocale(LC_ALL, ".UTF8");

	bancodedados();

}
//===============================
//Menu Login e Sair
//===============================
void tela_inicial(MYSQL* conexao) {

	int op;

	do {

		printf("               MENU INICIAL\n");
		printf("===========================================\n");
		printf("1 - Logar\n");
		printf("2 - Se-Cadastrar\n");
		printf("0 - Sair\n");

		printf("Digite uma opção:");
		scanf("%i", &op);
		while (op < 0 || op > 2) {
			printf("Digite uma opção válida:");
			scanf("%i", &op);
		}
		printf("\n");

		switch (op) {

		case 0:
			printf("Obrigado por Usar o Sitema...Volte Sempre!");
			Sleep(3000);
			exit(0);
			break;

		case 1:
			login(conexao);
			break;


		case 2:
			Cadastro(conexao);
			break;

		}
	} while (op != 0);
}

//===============================
//Cadastro de Usuario
//===============================
void Cadastro(MYSQL* conexao) {

	char query[200];

	system("cls");
	printf("\033[31m");
	printf("        NOME DE USUARIO\n");
	printf("=============================\n\n");
	printf("\033[33m");
	printf("Requisitos:\n");
	printf("Não usar seu nome Real\nMinimo 8 Caracteres\n\n");

	printf("\033[31m");
	printf("Digite um Nome de usuario:");
	getchar();
	fgets(cadastros[qtdecadastros].nomeUsuario, 50, stdin);

	cadastros[qtdecadastros].nomeUsuario[strcspn(cadastros[qtdecadastros].nomeUsuario, "\n")] = '\0';


	printf("             SENHA\n");
	printf("=================================\n");
	printf("\033[33m");
	printf("Requisitos:\n");
	printf("Minimo 8 caracteres\nSer uma senha forte\nUsar caracteres especiais\n\n");

	printf("\033[31m");
	printf("Digite uma Senha:");
	fgets(cadastros[qtdecadastros].Senha, 50, stdin);

	cadastros[qtdecadastros].Senha[strcspn(cadastros[qtdecadastros].Senha, "\n")] = '\0';

	if (qtdecadastros == 0) {
		nivel = 2;
	}
	else {
		nivel = 0;
	}

	system("cls");

	sprintf(query, "INSERT INTO usuario(Nome_Usuario, Senha_Usuario, Nivel_Usuario) VALUES('%s', '%s', %i);"
		, cadastros[qtdecadastros].nomeUsuario, cadastros[qtdecadastros].Senha, nivel);

	mysql_query(conexao, query);
	qtdecadastros++;

	tela_inicial(conexao);

}

//===============================
//Menu de Login
//===============================
void login(MYSQL* conexao) {

	char query[200];

	MYSQL_RES* resultado;
	MYSQL_ROW linha;

	bool Login = false;

	char Usuario[50];
	char senhalogin[50];

	system("cls");
	sprintf(query, "SELECT * FROM usuario;");
	mysql_query(conexao, query);
	resultado = mysql_store_result(conexao);
	linha = mysql_fetch_row(resultado);

		int op;

		if (linha == NULL) {

			printf("Nenhum Usuario Encontrado, Deseja se Cadastrar?\n");

			printf("1 - Sim\n2 - Não\n0 - Sair\n");
			printf("Digite uma opção:");
			scanf("%i", &op);
			while (op > 2 || op < 0) {
				printf("Digite uma opção Válida:");
				scanf("%i", &op);
			}

			switch (op) {

			case 1:

				Cadastro(conexao);
				break;

			case 2:
				tela_inicial(conexao);
				break;

			case 0:
				printf("Obrigado por usar o Sitema!, Volte Sempre");
				Sleep(3000);
				exit(0);
				break;

			}
		}
		else {
			system("cls");
		}

	do {

		printf("Digite seu nome de Usuario e senha:\n");
		printf("Usuario:");
		getchar();
		fgets(Usuario, 50, stdin);

		Usuario[strcspn(Usuario, "\n")] = '\0';

		
		printf("Digite Sua senha:");
		fgets(senhalogin, 50, stdin);

		senhalogin[strcspn(senhalogin, "\n")] = '\0';

		sprintf(query, "SELECT * FROM usuario WHERE Nome_Usuario = '%s' AND Senha_Usuario = '%s';", Usuario, senhalogin);
		mysql_query(conexao, query);

		resultado = mysql_store_result(conexao);

		linha = mysql_fetch_row(resultado);

		if (linha != NULL) {

			nivel = atoi(linha[3]);
			printf("Login realizado com sucesso\n");
			Login = true;
			system("cls");
			if (nivel == 2) {
				menuADM(conexao);
			}

			else if (nivel == 1) {
				
			}
			else {
				menuusuario(conexao);
			}
		}
		else {
			system("cls");
			printf("Login ou senha incorretos, tente novamente\n\n");
			Login = false;
		}
	} while (Login != true);

}

//===============================
//Menu do Usuario(nivel 0)
//===============================
void menuusuario(MYSQL* conexao) {

	int op;

	do {

		system("cls");
		printf("          MENU USUARIO\n");
		printf("==================================\n");
		printf("1 - Ver Perfil\n");
		printf("2 - Alterar Nome\n");
		printf("3 - Alterar Senha\n");
		printf("4 - Ver nivel da Conta\n");
		printf("5 - Sair da conta\n");
		printf("0 - Sair do Programa\n");

		printf("Digite alguma Opção:");
		scanf("%i", &op);
		getchar();
		while (op < 0 || op > 5) {
			printf("Digite alguma Opção válida:");
			scanf("%i", &op);
			getchar();
		}

		switch (op) {

		case 0:
			printf("Obrigado por Usar o Sistema!,Volte sempre");
			Sleep(3000);
			exit(0);
			break;

		case 1:
			system("cls");
			printf("Nome:%s Nivel:%i\n", cadastros[UsuarioLogado].nomeUsuario, nivel);

			if (nivel == 0) {
				printf("Nivel: Usuario\n");
			}
			if (nivel == 1) {
				printf("Nivel: Supervisor\n");
			}
			if (nivel == 2) {
				printf("Nivel: Administrador\n");
			}
			system("pause");
			break;

		case 2:
			system("cls");
			printf("Nome Atual: %s\n", cadastros[UsuarioLogado].nomeUsuario);
			printf("Pra Que nome você deseja mudar:");
			fgets(cadastros[UsuarioLogado].nomeUsuario, 50, stdin);

			cadastros[UsuarioLogado].nomeUsuario[strcspn(cadastros[UsuarioLogado].nomeUsuario, "\n")] = '\0';
			break;

		case 3:
			system("cls");
			char senhaDigitada[50];

			printf("Senha Atual:");
			fgets(senhaDigitada, 50, stdin);

			senhaDigitada[strcspn(senhaDigitada, "\n")] = '\0';

			if (strcmp(senhaDigitada, cadastros[UsuarioLogado].Senha) == 0) {
				printf("Pra Qual senha voçê deseja Mudar:");
				fgets(cadastros[UsuarioLogado].Senha, 50, stdin);

				cadastros[UsuarioLogado].Senha[strcspn(cadastros[UsuarioLogado].Senha, "\n")] = '\0';
			}

			else {
				printf("Senha Incorreta!\n Tente Novamente:");
				fgets(senhaDigitada, 50, stdin);
			}

			break;

		case 4:

			system("cls");
			if (nivel == 0) {
				printf("Nivel: Usuario\n");
			}
			if (nivel == 1) {
				printf("Nivel: Supervisor\n");
			}
			if (nivel == 2) {
				printf("Nivel: Administrador\n");
			}
			system("pause");
			break;

		case 5:
			system("cls");
			tela_inicial(conexao);
			break;

		}
	} while (op != 0);

}

//===============================
//MENU MODERADOR(nivel 1)
//===============================
void menuMOD(MYSQL* conexao) {

}

//===============================
//Menu Administrador(nivel 2)
//===============================
void menuADM(MYSQL* conexao) {

	int op;

	do {

		printf("             MENU DE ADMINISTRADOR\n");
		printf("===============================================\n");
		printf("1 - Listar Usuários\n");
		printf("2 - Procurar Usuários\n");
		printf("3 - Promover Usuário\n");
		printf("4 - Deletar Usuário\n");
		printf("5 - Ver Logs\n");
		printf("6 - Total de Usuários\n");
		printf("7 - REsetar Senha\n");
		printf("8 - Banir Usuário\n");
		printf("0 - Sair\n");

		printf("Digite uma opção:");
		scanf("%i", &op);

		switch (op) {

		case 0:
			printf("Obrigado por Usar o Sistema...Volte sempre!");
			break;
		}

	} while (op != 0);
}


