#include "../header/vui_vui.h"
// #include "../src/secondaryFunction.cpp"
#include <string>

void Product::print()
{
    cout << "ID: " << id << "\n";
    cout << "Name of Product: " << nameProducts << "\n";
    cout << "Type: " << type << "\n";
    cout << "Name of Importer: " << nameImporter << "\n";
    cout << "Price: " << price << "\n";
    cout << "Quantity: " << quantity << "\n";
    cout << "Warehouse: " << warehouse << "\n"; 
    
}

/*Hàm chuyển chữ thường thành chữ hoa trong C++*/
char upper(char chr)
{
    if('a' <= chr && chr <= 'z'){
        chr = chr - ((int)('a') - (int)('A'));
    }
    return chr;
}

/*Hàm chuyển chữ hoa thành chữ thường trong chr*/
char lower(char chr)
{
    if('A' <= chr && chr <= 'Z'){
        chr = chr + ((int)('a') - (int)('A'));
    }
    return chr;
}

bool Date::checkDate() 
{
    if (year < 0 || (month > 12 && month < 1) || day < 1) return false;
    if ( month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month ==12)
    {
        if (day > 31) return false;
    }
    if (month == 2 || month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day > 30) return false;
    }
    if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
    {
        if (month == 2 && day > 29) return false;
    }
    else 
    {
        if (month == 2 && day > 28) return false;
    }
    return true;
}

//Hàm dùng để copy khi resize
void copyProduct(Product* a, Product* b)
{
    a->id = b->id;
    strcpy(a->nameImporter,b->nameImporter);
    strcpy(a->nameProducts, b->nameProducts);
    a->price = b->price;
    a->quantity = b->quantity;
    a->type = b->type;
    a->warehouse = b->warehouse;
}

/* Hàm đẩy dữ liệu từ file ra struct */
/* Sau mỗi lần đọc 1 dòng file ra thì sẽ resize lại cái mảng động của struct*/
int putDataIntoStruct(char fileName[], Product* &data, int &sizeStruct) 
{
    fstream file;
    file.open(fileName, ios::in);
    if (!file.is_open())
    {
        cout << "Error open file";
        return -1;
    }
    char line[51];
    sizeStruct = 1;
    while(!file.eof())
    {
        file.getline(line, 51, ' ');
        data[sizeStruct-1].id = atoi(line);
        file.getline(line, 51, ' ');
        strcpy(data[sizeStruct-1].nameProducts, line);
        file.getline(line, 51, ' ');
        data[sizeStruct-1].type = line[0];
        file.getline(line, 51, ' ');
        strcpy(data[sizeStruct-1].nameImporter, line);
        file.getline(line, 51, ' ');
        data[sizeStruct-1].price = atof(line);
        file.getline(line, 51, ' ');
        data[sizeStruct-1].quantity = atoi(line);
        file.getline(line, 51, '\n');
        data[sizeStruct-1].warehouse = line[0];
        //resize struct
        Product* resizeData = new (std::nothrow) Product [sizeStruct + 1];
        if (!resizeData)
        {
            cout << "Error resize";
            file.close();
            return -2;
        }
        sizeStruct++;
        for (int i = 0; i < sizeStruct; i++)
        {
            copyProduct(&resizeData[i], &data[i]);
        }
        delete[] data;
        data = nullptr;
        data = new (std::nothrow) Product [sizeStruct];
        if(!data)
        {
            cout << "Fail Allocation";
            file.close();
            delete[] resizeData;
            resizeData = nullptr;
        }
        for (int i = 0; i < sizeStruct; i++)
        {
            copyProduct(&data[i], &resizeData[i]);
        }
    }
    sizeStruct --;
    file.close();
    return 0;
}

// Hàm đẩy dữ liệu từ struct vào file
int putDataInFile(char fileName[], Product* data, int &sizeStruct)
{
    fstream file;
    file.open(fileName, ios::out);
    if (!file.is_open())
    {
        cout << "Error opening file";
        return -1;
    }

    for (int i = 0; i < sizeStruct; i++)
    {
        if (i == (sizeStruct -1))
        {
            file << data[i].id << ' ' << data[i].nameProducts << ' ';
            file << data[i].type << ' ' << data[i].nameImporter << ' ';
            file << setprecision(10) << data[i].price << ' ' << data[i].quantity << ' ' << data[i].warehouse;
        }
        else
        {
            file << data[i].id << ' ' << data[i].nameProducts << ' ';
            file << data[i].type << ' ' << data[i].nameImporter << ' ';
            file << setprecision(10) << data[i].price << ' ' << data[i].quantity << ' ' << data[i].warehouse << endl;
        }
    }

    file.close();
    return 0;
}

bool checkID(Product *data, int sizeData, int idTmp)
{
    for (int i = 0; i < sizeData; i++)
    {
        if(data[i].id == idTmp)
        {
            return false;
        }
    }
    return true;
}

// Hàm nhập thêm dữ liệu vào file Data và lưu ngày nhập vào file Date
/*Nhập ngày sau đó kiểm tra nếu lỗi thì nhập lại, nhập dữ liệu vào struct tạm thời sau đó đẩy vào file Data*/
int inputNewData(char fileNameData[], char fileNameDate[], Product* &data, int &sizeStruct)
{
    fstream fileData;
    fileData.open(fileNameData, ios::in);
    if(!fileData.is_open())
    {
        cout << "Error opening file";
        return -1;
    }
    putDataIntoStruct(fileNameData, data, sizeStruct);
    while (recordDate(fileNameDate) == -1)
    {
        cout << "\n Day is incorrect, input again\n";
    }

    Product dataTmp;
    int flag = 1;
    while (flag)
    {
        cout << "Please input data: ";
        cout << endl;
        cout << "ID: ";
        cin >> dataTmp.id;
        if (!checkID(data, sizeStruct, dataTmp.id))
        {
            cout << "ID already exists \n";
            cout << "Please re-enter the data \n";
            continue;
        }
        cin.ignore();
        cout << "Name product: ";
        cin.getline(dataTmp.nameProducts, 31);
        cout << "Type: ";
        cin >> dataTmp.type;
        dataTmp.type = lower(dataTmp.type);
        cout << "Importer: ";
        cin.ignore();
        cin.getline(dataTmp.nameImporter, 51);
        cout << "Price: ";
        cin >> dataTmp.price;
        cout << "Quantity: ";
        cin >> dataTmp.quantity;
        cout << "Warehouse: ";
        cin >> dataTmp.warehouse;
        dataTmp.warehouse = upper(dataTmp.warehouse);

        // Save information for input data
        fstream fileDate;
        fileDate.open(fileNameDate, ios::app);
        if(!fileDate)
        {
            cout << "Error opening file";
            return -1;
        }
        
        fileDate << setw(5) << left << dataTmp.id << setw(20) << left << dataTmp.nameImporter << setw(5) << left << dataTmp.quantity << '\n';
        fileDate.close();
        //////////////////////////////////////////////////////////////////////////////

        Product* resizeData = new (std::nothrow) Product [sizeStruct + 1];
        if (!resizeData)
        {
            cout << "Error resize";
            fileData.close();
            fileData.close();
            return -2;
        }
        for (int i = 0; i < sizeStruct; i++)
        {
            copyProduct(&resizeData[i], &data[i]);
        }
        sizeStruct++;
        delete[] data;
        data = nullptr;
        data = new (std::nothrow) Product [sizeStruct];
        if(!data)
        {
            cout << "Fail Allocation";
            fileData.close();
            delete[] resizeData;
            resizeData = nullptr;
        }
        for (int i = 0; i < sizeStruct; i++)
        {
            copyProduct(&data[i], &resizeData[i]);
        }

        copyProduct(&data[sizeStruct - 1], &dataTmp);
        cout << "Do you want to continue input data? \n";
        cout << "1. Yes\n";
        cout << "0. No \n";
        cin >> flag;
    }

    putDataInFile(fileNameData, data, sizeStruct);
    
    fileData.close();
    return 0;
}


//Hàm dùng để hiện giao diện và thực hiện tính toán
int printSaleProduct(char fileName[], char fileDateName[], Product* &data, int sizeStruct)
{
    int idData = 0;
    int number = 0;
    double sum = 0;
    int flag1 = 0;
    while (recordDate(fileDateName) == -1)
    {
        cout << "\n Day is incorrect, input again\n";
    }
    putDataIntoStruct(fileName, data, sizeStruct);
    while(1)
    {
        cout << setw(3) << left << "ID" << setw(20) << left << "Name Product" << setw(10) << left << "Quantity" << setw(10) << left << "Price" << endl;
        for (int i = 0; i < sizeStruct; i++)
        {
            if (data[i].quantity == 0)
            {
                continue;
            }
            cout <<setw(3) << left <<  data[i].id << setw(20) << left << data[i].nameProducts
            << setw(10) << left << data[i].quantity << setw(10) << left << setprecision(10) << data[i].price << endl;
        }
        cout << "0. End\n";
        cout << "What do you want to buy? \n";
        cout << "Please input ID product you want to buy\n";
        cout << "ID: ";
        cin >> idData;
        if (idData == 0) 
        {
            break;
        }
        while(1)
        {
            for (int i = 0; i < sizeStruct; i++)
            {
                if(data[i].id == idData)
                {
                    idData = i;
                    break;
                }
            }
            cout << "Please input quantity product: ";
            cin >> number;
            if(number > data[idData].quantity|| number < 0) 
            {
                cout << "Number incorrect\n";
                cout << "Please input again\n";
                continue;
            }
            break;
        }
        sum = number* data[idData].price;
        cout << "Sum: " << sum << endl;
        cout << "Are you sure? \n";
        cout << "1. Yes \n";
        cout << "0. No \n";
        cout << "Please choose: ";
        cin >> flag1;
        if (flag1 == 0) 
        {
            continue;
        }
        else 
        {
            // Save information output data
                fstream fileDate;
                fileDate.open(fileDateName, ios::app);
                if(!fileDate)
                {
                    cout << "Error opening file";
                    return -1;
                }
                fileDate << setw(5) << left << data[idData].id << setw(10) << left << number << '\n';
                fileDate.close();
                ///////////////////////////////////////////////////////////////////////////////////////////////////////

            data[idData].quantity = data[idData].quantity - number;
            putDataInFile(fileName, data, sizeStruct);
        }
        cout << "Do you want to continue buy? \n";
        cout << "1. Yes \n";
        cout << "0. No \n";
        cout << "Please choose: ";
        cin >> flag1;
        if (flag1) 
        {
            continue;
        }
        else 
        {
            break;
        }
    }  
    return 0;
}

int recordDate(char fileDateName[])
{
    fstream file;
    file.open(fileDateName, ios::app);
    if(!file.is_open())
    {
        cout << "Error opening file";
        return -1;
    }
    Date tmp;
    while(1)
    {
        cout << "Please input date: \n";
        cout << "Day: ";
        cin >> tmp.day;
        cout << "Month: ";
        cin >> tmp.month;
        cout << "Year: ";
        cin >> tmp.year;
        if(tmp.checkDate())
        {
            break;
        }
        else
        {
            cout << "Day is incorrect \n";
            cout << "Please input again\n";
        }
    }

    file << tmp.day << '/' << tmp.month << '/' << tmp.year <<'\n';
    file.close();
    return 0;
}

int printData(char file[], Product* &data, int &sizeData)
{
    fstream fileRead;
    fileRead.open(file, ios::in);
    if (!fileRead)
    {
        cout << "Error opening file";
        return -1;
    }
    putDataIntoStruct(file, data, sizeData);
    fileRead.close();
    cout << "List Products \n";
    cout << setw(3) << left << "ID" << setw(15) << left << "Name product" << setw(5) << left << "Type";
    cout << setw(20) << left << "Name importer" << setw(10) << left << "Price" << setw(10) << left << "Quantity" << setw(10) << left << "WareHouse" << endl;
    for (int i = 0; i < sizeData; i++)
    {
        cout << setw(3) << left << data[i].id << setw(15) << left << data[i].nameProducts << setw(5) << left << data[i].type;
        cout << setw(20) << left << data[i].nameImporter << setw(10) << left << setprecision(10) << data[i].price << setw(10) << left << data[i].quantity << setw(10) << left << data[i].warehouse << endl; 
    }
    return 0;
}

void printProductByStruct(Product* data, int sizeData)
{
    for (int i = 0; i < sizeData; i++)
    {
        cout << setw(3) << left << data[i].id << setw(15) << left << data[i].nameProducts << setw(5) << left << data[i].type;
        cout << setw(20) << left << data[i].nameImporter << setw(10) << left << setprecision(10) << data[i].price 
        << setw(10) << left << data[i].quantity << setw(10) << left << data[i].warehouse << endl; 
    }
    return;
}

void swapProduct(Product &a, Product &b)
{
    Product tmp;
    copyProduct(&tmp, &a);
    copyProduct(&a, &b);
    copyProduct(&b, &tmp);
    return;
}


void sortID(Product* &data, int sizeData)
{
    for (int i = 0 ; i < sizeData; i++)
    {
        for (int j = 0; j < sizeData - 1; j++)
        {
            if(data[j].id > data[j+1].id)
            {
                swapProduct(data[j], data[j+1]);
            }
        }
    }
}

void sortQuantity(Product* &data, int sizeData)
{
    for (int i = 0 ; i < sizeData; i++)
    {
        for (int j = 0; j < sizeData - 1; j++)
        {
            if(data[j].quantity > data[j+1].quantity)
            {
                swapProduct(data[j], data[j+1]);
            }
        }
    }
}

void sortPrice(Product* &data, int sizeData)
{
    for (int i = 0 ; i < sizeData; i++)
    {
        for (int j = 0; j < sizeData - 1; j++)
        {
            if(data[j].price > data[j+1].price)
            {
                swapProduct(data[j], data[j+1]);
            }
        }
    }
}

void sortNameProduct(Product* &data, int sizeData)
{
    for (int i = 0 ; i < sizeData; i++)
    {
        for (int j = 0; j < sizeData - 1; j++)
        {
            if(strcmp(data[j].nameProducts, data[j+1].nameProducts) > 0)
            {
                swapProduct(data[j], data[j+1]);
            }
        }
    }
}
void sortTypeProduct(Product* &data, int sizeData)
{
    for (int i = 0 ; i < sizeData; i++)
    {
        for (int j = 0; j < sizeData - 1; j++)
        {
            if(data[j].type > data[j+1].type)
            {
                swapProduct(data[j], data[j+1]);
            }
        }
    }
}
void sortWarehouse(Product* &data, int sizeData)
{
    for (int i = 0 ; i < sizeData; i++)
    {
        for (int j = 0; j < sizeData - 1; j++)
        {
            if(data[j].warehouse > data[j+1].warehouse)
            {
                swapProduct(data[j], data[j+1]);
            }
        }
    }
}

int sortProduct(char fileData[], Product* &data, int sizeData)
{
    putDataIntoStruct(fileData, data, sizeData);
    int flag  = 1;
    while(flag)
    {
        int n;
        cout << "What is the data you want to sort?\n";
        cout << "1. ID \n";
        cout << "2. Name Products\n";
        cout << "3. Type Products\n";
        cout << "4. Price \n";
        cout << "5. Quantity \n";
        cout << "6. Warehouse \n";
        cout << "0. End\n";
        cout << "Please enter your selection: ";
        cin >> n;
        if (n == 0)
        {
            cout << "End of arrangement \n";
            break;
        }
        if (n == 1)
        {
            sortID(data, sizeData);
            printProductByStruct(data, sizeData);
        }
        if (n == 2)
        {
            sortNameProduct(data, sizeData);
            printProductByStruct(data, sizeData);
        }
        if (n == 3)
        {
            sortTypeProduct(data, sizeData);
            printProductByStruct(data, sizeData);
        }
        if (n == 4)
        {
            sortPrice(data, sizeData);
            printProductByStruct(data, sizeData);
        }
        if (n == 5)
        {
            sortQuantity(data, sizeData);
            printProductByStruct(data, sizeData);
        }
        if (n == 6)
        {
            sortWarehouse(data, sizeData);
            printProductByStruct(data, sizeData);
        }
        if (n < 0 || n > 6)
        {
            cout << "Entered incorrectly, please re-enter \n";
            continue;
        }
        
        int tmp = 1;
        while (tmp)
        {
            cout << "Do you want to include this newly changed data in the file?\n";
            cout << "1. Yes \n";
            cout << "0. No \n";
            cout << "Please input: ";
            cin >> tmp;
            if (tmp == 1)
            {
                putDataInFile(fileData, data, sizeData);
                break;
            }
            if (tmp != 0 && tmp != 1)
            {
                cout << "You entered incorrectly, please re-enter\n";
                continue;
            }
        }

        while (true)
        {
            cout << "Do you want to continue? \n";
            cout << "1. Yes \n";
            cout << "0. No \n";
            cout << "Please input: ";
            cin >> flag;
            if (flag == 0 || flag == 1)
            {
                break;
            }
            else
            {
                cout << "The number entered is incorrect, please re-enter \n";
                continue;
            }
        }
    }
    return 0;
}

void printEleData(Product data)
{
    cout << setw(3) << left << data.id << setw(15) << left << data.nameProducts << setw(5) << left << data.type;
    cout << setw(20) << left << data.nameImporter << setw(10) << left << setprecision(10) << data.price 
    << setw(10) << left << data.quantity << setw(10) << left << data.warehouse << endl; 
}

void searchID(Product* data, int sizeData)
{
    int flag = 1;
    while(flag)
    {
        int search;
        cout << "Please enter the ID you want to search: ";
        cin >> search;
        int n = 0;
        for (int i = 0; i < sizeData; i++)
        {
            if (data[i].id == search)
            {
                printEleData(data[i]);
                n++;
            }
        }
        if (n == 0)
        {
            cout << "There are no satisfactory data \n";
        }
        while (true)
        {
            cout << "Do you want to continue searching for id?\n";
            cout << "1. Yes \n";
            cout << "0. No \n";
            cin >> flag;

            if (flag == 1 || flag == 0)
            {
                break;
            }
            else
            {
                cout << "The input is incorrect, please re-enter\n";
            }
        } 
    } 
}

void searchQuantity(Product* data, int sizeData)
{
    int flag = 1;
    while(flag)
    {
        int search;
        cout << "Please enter the quantity you want to search for: ";
        cin >> search;
        int n = 0;
        for (int i = 0; i < sizeData; i++)
        {
            if (data[i].quantity == search)
            {
                printEleData(data[i]);
                n++;
            }
        }
        if (n == 0)
        {
            cout << "There are no satisfactory data \n";
        }
        while (true)
        {
            cout << "Do you want to continue searching for the quantity?\n";
            cout << "1. Yes \n";
            cout << "0. No \n";
            cin >> flag;

            if (flag == 1 || flag == 0)
            {
                break;
            }
            else
            {
                cout << "The input is incorrect, please re-enter\n";
            }
        } 
    } 
}

void searchPrice(Product* data, int sizeData)
{
    int flag = 1;
    while(flag)
    {
        double search;
        cout << "Please enter the price you want to search for: ";
        cin >> search;
        int n = 0;
        for (int i = 0; i < sizeData; i++)
        {
            if (data[i].price == search)
            {
                printEleData(data[i]);
                n++;
            }
        }
        if (n == 0)
        {
            cout << "There are no satisfactory data \n";
        }
        while (true)
        {
            cout << "Do you want to continue searching for prices??\n";
            cout << "1. Yes \n";
            cout << "0. No \n";
            cin >> flag;

            if (flag == 1 || flag == 0)
            {
                break;
            }
            else
            {
                cout << "The input is incorrect, please re-enter\n";
            }
        } 
    } 
}

void searchType(Product* data, int sizeData)
{
    int flag = 1;
    while(flag)
    {
        char search;
        cout << "Please enter the type you want to search for: ";
        cin >> search;
        search = lower(search);
        int n = 0;
        for (int i = 0; i < sizeData; i++)
        {
            if (data[i].type == search)
            {
                printEleData(data[i]);
                n++;
            }
        }
        if (n == 0)
        {
            cout << "There are no satisfactory data \n";
        }
        while (true)
        {
            cout << "Do you want to continue searching for the type??\n";
            cout << "1. Yes \n";
            cout << "0. No \n";
            cin >> flag;

            if (flag == 1 || flag == 0)
            {
                break;
            }
            else
            {
                cout << "The input is incorrect, please re-enter\n";
            }
        } 
    } 
}

void searchWarehouse(Product* data, int sizeData)
{
    int flag = 1;
    while(flag)
    {
        char search;
        cout << "Please enter the warehouse you want to search: ";
        cin >> search;
        search = upper(search);
        int n = 0;
        for (int i = 0; i < sizeData; i++)
        {
            if (data[i].warehouse == search)
            {
                printEleData(data[i]);
                n++;
            }
        }
        if (n == 0)
        {
            cout << "There are no satisfactory data \n";
        }
        while (true)
        {
            cout << "Do you want to continue searching for the warehouse??\n";
            cout << "1. Yes \n";
            cout << "0. No \n";
            cin >> flag;

            if (flag == 1 || flag == 0)
            {
                break;
            }
            else
            {
                cout << "The input is incorrect, please re-enter\n";
            }
        } 
    } 
}

void searchNameProduct(Product* data, int sizeData)
{
    int flag = 1;
    while(flag)
    {
        char search[31];
        cout << "Please enter the product name you want to search for: ";
        cin.ignore();
        cin.getline(search, 31);
        int n = 0;
        for (int i = 0; i < sizeData; i++)
        {
            if (strcmp(data[i].nameProducts, search) == 0)
            {
                printEleData(data[i]);
                n++;
            }
        }
        if (n == 0)
        {
            cout << "There are no satisfactory data \n";
        }
        while (true)
        {
            cout << "Do you want to continue searching for the product name??\n";
            cout << "1. Yes \n";
            cout << "0. No \n";
            cin >> flag;

            if (flag == 1 || flag == 0)
            {
                break;
            }
            else
            {
                cout << "The input is incorrect, please re-enter\n";
            }
        } 
    } 
}

void searchNameImporter(Product* data, int sizeData)
{
    int flag = 1;
    while(flag)
    {
        char search[51];
        cout << "Please enter the name of the importer you want to search for: ";
        cin.ignore();
        cin.getline(search, 51);
        int n = 0;
        for (int i = 0; i < sizeData; i++)
        {
            if (strcmp(data[i].nameImporter, search) == 0)
            {
                printEleData(data[i]);
                n++;
            }
        }
        if (n == 0)
        {
            cout << "There are no satisfactory data \n";
        }
        while (true)
        {
            cout << "Do you want to continue searching for the importer's name??\n";
            cout << "1. Yes \n";
            cout << "0. No \n";
            cin >> flag;

            if (flag == 1 || flag == 0)
            {
                break;
            }
            else
            {
                cout << "The input is incorrect, please re-enter\n";
            }
        } 
    } 
}

int searchProduct(char fileData[], Product* &data, int sizeData)
{
    putDataIntoStruct(fileData, data, sizeData);
    int flag  = 1;
    while(flag)
    {
        int n;
        cout << "What is the data you want to search?\n";
        cout << "1. ID \n";
        cout << "2. Name Products\n";
        cout << "3. Type Products\n";
        cout << "4. Price \n";
        cout << "5. Quantity \n";
        cout << "6. Warehouse \n";
        cout << "7. Name importer \n";
        cout << "0. End\n";
        cout << "Please enter your selection: ";
        cin >> n;
        if (n == 0)
        {
            cout << "End of search \n";
            break;
        }
        if (n == 1)
        {
            searchID(data, sizeData);
        }
        if (n == 2)
        {
            searchNameProduct(data, sizeData);
        }
        if (n == 3)
        {
            searchType(data, sizeData);
        }
        if (n == 4)
        {
           searchPrice(data, sizeData);
        }
        if (n == 5)
        {
            searchQuantity(data, sizeData);
        }
        if (n == 6)
        {
            searchWarehouse(data, sizeData);
        }
        if (n == 7)
        {
            searchNameImporter(data, sizeData);
        }

        if (n < 0 || n > 7)
        {
            cout << "Entered incorrectly, please re-enter \n";
            continue;
        }
        while (true)
        {
            cout << "Do you want to continue searching? \n";
            cout << "1. Yes \n";
            cout << "0. No \n";
            cout << "Please input: ";
            cin >> flag;
            if (flag == 0 || flag == 1)
            {
                break;
            }
            else
            {
                cout << "The number entered is incorrect, please re-enter \n";
                continue;
            }
        }
    }
    return 0;
}

int representsOptions(char fileData[], char fileInDate[], char fileOutDate[], Product* &data, Date* &date, int &sizeData, int &sizeDate)
{
    cout << "Welcome to the program\n";
    while(true)
    {
        int number;
        cout << "What do you want to do? \n";
        cout << "1. Input new product \n";
        cout << "2. Buy product\n";
        cout << "3. Print all product\n";
        cout << "4. Sort product \n";
        cout << "5. Search product \n";
        cout << "0. End program \n";
        cout << "Please enter your desired action: ";
        cin >> number;
        if (number == 1)
        {
            inputNewData(fileData, fileInDate, data, sizeData);
        }
        else if (number == 2)
        {
            printSaleProduct(fileData, fileOutDate, data, sizeData);
        }
        else if (number == 3)
        {
            printData(fileData, data, sizeData);
        }
        else if (number == 4)
        {
            sortProduct(fileData, data, sizeData);
        }
        else if (number == 5)
        {
            searchProduct(fileData, data, sizeData);
        }
        else if (number == 0)
        {
            cout << "Thank you for your use \n";
            cout << "Program end. \n";
            return 0;
        }
        else
        {
            cout << "You entered it wrong, the program will run again\n";
            continue;
        }
    }
    return 0;
}
