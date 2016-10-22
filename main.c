#include <stdio.h>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include<sqltypes.h>
#include<string.h>

void sqlIUD(SQLHDBC *dbc, char *command){ // função que atualiza, deleta e insere

    SQLHSTMT stmt;
    SQLRETURN ret;
    SQLAllocHandle(SQL_HANDLE_STMT, (*dbc), &stmt);
    ret = SQLExecDirect(stmt,(SQLCHAR *)command,SQL_NTS);
}

void recoverPrint(SQLHDBC *dbc, char *command){ // função que exibe
    SQLHSTMT stmt;
    SQLRETURN ret;
    SQLLEN indicator[6];
    SQLCHAR cpf[11]="";
    SQLCHAR nome[30]="";
    SQLCHAR rua[30]="";
    SQLCHAR bairro[30]="";
    SQLCHAR cep[10]="";
    SQLCHAR pais[10]="";
    stmt=NULL;

    int cont = 0; // contar número de colunas
    SQLAllocHandle(SQL_HANDLE_STMT, (*dbc), &stmt);
    ret = SQLBindCol(stmt,1,SQL_C_CHAR,cpf,sizeof(cpf),&indicator[0]);
    ret = SQLBindCol(stmt,2,SQL_C_CHAR,nome,sizeof(nome),&indicator[1]);
    ret = SQLBindCol(stmt,3,SQL_C_CHAR,rua,sizeof(rua),&indicator[2]);
    ret = SQLBindCol(stmt,4,SQL_C_CHAR,bairro,sizeof(bairro),&indicator[3]);
    ret = SQLBindCol(stmt,5,SQL_C_CHAR,cep,sizeof(cep),&indicator[4]);
    ret = SQLBindCol(stmt,6,SQL_C_CHAR,pais,sizeof(pais),&indicator[5]);
    ret = SQLExecDirect(stmt,(SQLCHAR *)command,SQL_NTS);
    while ((ret=SQLFetch(stmt)) != SQL_NO_DATA){
        printf("\n****** CPF : %s \tNome: %s \tRua: %s \tBairro: %s \tCEP: %s \tPais: %s ******\n", cpf, nome, rua, bairro, cep, pais);
        cont++;
    }

    printf("\nconsulta realizada com sucesso\n\n");
    printf("numero de colunas: %d\n\n",cont);

}

int main()
{
    SQLHENV env;
    SQLHDBC dbc;
    SQLHSTMT stmt;
    SQLRETURN ret;
    int menu;
    char *qri = (char*) malloc(500*sizeof(char));
    char *qrr = (char*) malloc(500*sizeof(char));
    char *qra = (char*) malloc(500*sizeof(char));
    char *qrs = (char*) malloc(500*sizeof(char));
    char *qrs2 = (char*) malloc(500*sizeof(char));

    /* Aloca um handle do tipo environment */
    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);

    /* Seta o ambiente para oferecer suporte a ODBC 3 */
    SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void *) SQL_OV_ODBC3, 0);

    /* Aloca um handle do tipo connection */
    SQLAllocHandle(SQL_HANDLE_DBC, env, &dbc);

    /* Conecta ao DSN chamado "Teste"*/
    /* Mude "Teste" para o do DNS que você já criou*/
    SQLDriverConnect(dbc, NULL, (SQLCHAR*)"DSN=oracle;", SQL_NTS,
                     NULL, 0, NULL, SQL_DRIVER_COMPLETE);

    do
    {
        printf("Menu:\n");
        printf("1-insercao\n2-remocao\n3-atualizar\n4-consulta\n0-sair\n\n");
        scanf("%d",&menu);

        switch (menu){
            case 0:
                printf ("Sair/n");
                break;
            case 1: //Inserção
                printf ("Insercao:/n");
                //= "INSERT INTO CARRO (PLACA, COR, ANO, MODELO) VALUES ('LLL66', 'ABACATE', 2016, 'PODER');";
                scanf(" %500[^\n]s",qri);
                sqlIUD(&dbc, qri);
                printf("Insercao realizada com sucesso!\n");
                break;
            case 2: //remoção
                printf("Remocao:/n");
                //char *qrr = "DELETE FROM CARRO  WHERE <CONDIÇÃO>";
                scanf(" %500[^\n]s",qrr);
                sqlIUD(&dbc, qrr);
                printf("Remocao realizada com sucesso!\n");
                break;
            case 3:
                printf("Update:\n");
                //UPDATE CARRO SET <ATRIBUTO> = <VALOR> WHERE <CONDIÇÃO>
                scanf(" %500[^\n]s",qra);
                sqlIUD(&dbc, qra);
                printf("Update realizado com sucesso!\n");
                break;
            case 4:
                printf("Consulta:\n");
                fflush(stdin);
                scanf("%500[^\n]s",qrs);
                strcpy(qrs2,"SELECT * FROM PESSOA "); // copiando a string para a variavel
                strcat(qrs2,qrs); // pra concatenar aqui
                recoverPrint(&dbc, qrs2);
                break;
            default:
                printf("Comando invalido, tente novamente\n");
        }
    }while (menu != 0);

    return 0;
}
