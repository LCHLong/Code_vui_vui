#include "../header/vui_vui.h"

int main()
{
    char inputData[100] = "data.txt";
    char inputDate[100] = "date_input_data.txt";
    char outputDate[100] = "date_output_data.txt";
    Product* data = new (std::nothrow) Product [1];
    if (!data)
    {
        cout << "Error opening file";
    }
    int sizeData = 1;
    Date* date = new (std::nothrow) Date[1];
    if (!date)
    {
        cout << "Error opening file";
    }
    int sizeDate = 1;
    representsOptions(inputData, inputDate, outputDate, data, date, sizeData, sizeDate);
    delete[] data;
    data = nullptr;
    delete[] date;
    date = nullptr;
}