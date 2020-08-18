#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//INPUT:
//   ./main
//  dd.mm.yyyy-dd.mm.yyyy
//      ili
//  ./main < data.txt
//
//OUTPUT:
//  broj dana izmedju dva datuma
typedef struct date{
    int day;
    int month;
    int year;
}dateStruct;

int argValid(char input_date[]){

    if(strlen(input_date) != 21){
        fprintf(stderr, "strlen != 21");
        return -1;
    }

    int invld_sign = 0; 
    int comma_or_dash = 0;
    for(int i=0; i<21; i++)
    {
        if((input_date[i] < '-' || input_date[i] > '9') || input_date[i] == '/')
            invld_sign ++;           
        if(input_date[i] == '.' || input_date[i] == '-')
            comma_or_dash ++;
    }

    if(invld_sign != 0){
        fprintf(stderr, "Invalid symbols found.");
        return -1;
    }
    if(comma_or_dash == 5){
        if(input_date[2] != '.' || input_date[5] != '.' || input_date[10] != '-' || input_date[13] != '.' || input_date[16] != '.'){
            fprintf(stderr, "comma_or_dash position incorrect.");
            return -1;
        }
    }else{
        fprintf(stderr, "comma_or_dash != 5");
        return -1;  
        }

    return 0;
}
    
int formatDate(char date[], int format){

    char day[3], month[3], year[5];
    
    date[2] = '\0';
    date[5] = '\0';

    for(int i=0; i<11; i++)
    {
        if(i <= 2)
            day[i] = date[i];
        else if(i > 2 && i <= 5)
            month[i-3] = date[i];
        else if(i > 5)
            year[i-6] = date[i];
    } 
             
    int iDay, iMnt, iYr;
    sscanf(day, "%d", &iDay);
    sscanf(month, "%d", &iMnt);
    sscanf(year, "%d", &iYr);

    if(format == 0){     //day
        return iDay;}
    if(format == 1){     //month
        return iMnt;}
    if(format == 2){     //year
        return iYr;}

    return 0;
}
   
int prestup(int year){

    if(year % 4 == 0){
        if(year % 100 == 0){
            if(year % 400 == 0){
                return 1;
            }else{
                return 0;
            }
        }
        return 1;
    }

    return 0;
}
   
int days_in_month(int month, int year){

    int month31[8] = {1,3,5,7,8,10,12};
    int month30[5] = {4,6,9,11};

    for(int i=0; i<12; i++)
    {
        if(month == month31[i])
            return 31;
        else if(month == month30[i])
            return 30;
        else if(month == 2){
            if (prestup(year) == 1)
                return 29;
            else 
                return 28;
        }
    }

    return 0;

}

int dateValid(dateStruct d){
    
    if(d.day == 0 || d.day > 31)
        return -1;
    if(d.month == 0 || d.month > 12)
        return -1;
    if(d.year == 0)
        return -1;

    if (d.day > days_in_month(d.month, d.year))
        return -1;   
    
    return 1;
}

int daysSum(dateStruct date1, dateStruct date2){
    
    int sum = 0, i;
    for(i=date1.year; i<date2.year; i++)
    {
        if(prestup(i) == 1)
            sum+=366;
        else
            sum+=365;
    }

    int daysFirstDate = 0, daysSecondDate = 0;

    for(i=1; i<date1.month; i++)
    {
        daysFirstDate += days_in_month(i, date1.year);
    }

    daysFirstDate += date1.day;
    sum -= daysFirstDate;
    
    for(i=1; i<date2.month; i++)
    {
        daysSecondDate += days_in_month(i, date2.year);
    } 

    daysSecondDate += date2.day;
    sum += daysSecondDate;

    return sum;
}

int main(){
    
    char input_date[21];
    scanf("%s", input_date);
    
    if (argValid(input_date) == -1){
        fprintf(stderr, "Input is not valid.");
        return -1;
    }

    char date1[11], date2[11];

    for(int i=0; i<21; i++)
    {
        if(i < 10){
            date1[i] = input_date[i];
        }else{
            if(i > 10)
                date2[i-11] = input_date[i];
        }
    }
        
    dateStruct d1, d2;  

    d1.day = formatDate(date1, 0);
    d1.month = formatDate(date1, 1);
    d1.year = formatDate(date1, 2);
    
    d2.day = formatDate(date2, 0);
    d2.month = formatDate(date2, 1);
    d2.year = formatDate(date2, 2);

    if(dateValid(d1) != 1 || dateValid(d2) != 1){
        fprintf(stderr, "Date is not valid.");
        return -1;
    }

    //Prvi datum ne mora da bude veci od drugog
    if(d2.year > d1.year)    
        fprintf(stdout, "%d\n", daysSum(d1, d2));
    else if(d2.year < d1.year) 
        fprintf(stdout, "%d\n", daysSum(d2, d1));
    else{
        if(d2.month > d1.month)
            fprintf(stdout, "%d\n", daysSum(d1, d2));
        else if (d2.month < d1.month)
            fprintf(stdout, "%d\n", daysSum(d2, d1));
        else{
            if(d2.day > d1.day)   
                fprintf(stdout, "%d\n", daysSum(d1, d2));
            else if(d2.day < d1.day)
                fprintf(stdout, "%d\n", daysSum(d2, d1));
            else         //isti datumi - stampa nulu 
                fprintf(stdout, "%d\n", daysSum(d1, d2));
        }
    }

    return 0;
}
