#pragma once 

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cstring>
#include <new>
#include <iomanip>

using namespace std;

struct Date
{
    int day;
    int month;
    int year;
    bool checkDate();
};

struct Product
{
    double price;
    int quantity;
    int id;
    char nameProducts [31];
    char type;
    char nameImporter [51];
    char warehouse; 
    void print();
};

//Các hàm cho sort
void printProductByStruct(Product* data, int sizeData);
void swapProduct(Product &a, Product &b);
void sortID(Product* &data, int sizeData);
void sortQuantity(Product* &data, int sizeData);
void sortPrice(Product* &data, int sizeData);
void sortNameProduct(Product* &data, int sizeData);
void sortTypeProduct(Product* &data, int sizeData);
void sortWarehouse(Product* &data, int sizeData);
int sortProduct(char fileData[], Product* &data, int sizeData);

//Các hàm cho search
void printEleData(Product data);
void searchID(Product* data, int sizeData);
void searchQuantity(Product* data, int sizeData);
void searchPrice(Product* data, int sizeData);
void searchType(Product* data, int sizeData);
void searchWarehouse(Product* data, int sizeData);
int searchProduct(char fileData[], Product* &data, int sizeData);
void searchNameProduct(Product* data, int sizeData);
void searchNameImporter(Product* data, int sizeData);

char upper(char chr);
char lower(char chr);
void copyProduct(Product* a, Product* b);
int putDataIntoStruct(char fileName[], Product* &data, int &sizeStruct);
int putDataInFile(char fileName[], Product* data, int &sizeStruct);
int inputNewData(char fileNameData[], char fileNameDate[], Product* &data, int &sizeStruct);
bool checkID(Product *data, int sizeData, int idTmp);
int printSaleProduct(char fileName[], char fileDateName[] ,Product* &data, int sizeStruct);
int recordDate(char fileDateName[]);
int printData(char file[], Product* &data, int &sizeData);
int representsOptions(char fileData[], char fileInDate[], char fileOutDate[], Product* &data, Date* &date, int &sizeData, int &sizeDate);