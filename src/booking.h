/*
 * booking.h
 *
 *  Created on: 14-Oct-2014
 *      Author: anirudh
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#ifndef BOOKING_H_
#define BOOKING_H_

typedef enum { false, true } bool;

int bookedCount=0;
int nTicket=-1;
int capacity=20;
typedef struct
{
	char passenger_name[30];
	char departure_city[20];
	char destination_city[20];
	char airline[20];
	char flight_number[10];
	char date[10];
	char dep_time[10];
	char arr_time[10];
	char booking_reference[20];
	int fare;
	bool active;


}ticket;

ticket *booking;
FILE* fo;
FILE* fi;
void increaseCapacity()
{
	if(nTicket==capacity)
		{
			capacity*=2;
			booking=(ticket*)realloc(booking, capacity*sizeof(ticket));
		}
}
void rewriteFile()
{
	FILE* fb;
	fb=fopen("src/bookings.dat","w+");
	int size=sizeof(ticket);
	int i=0;
	for(i=0;i<=nTicket;i++)
		fwrite(&booking[i], size, 1, fb);
	fclose(fb);
}
void readFile()
{
	FILE *fb;
	fb =  fopen("src/bookings.dat","r");
	int size=sizeof(ticket);

	while(fread(&booking[++nTicket], size, 1, fb)==1)
		increaseCapacity();
	--nTicket;
	//printf("Number read %d",nTicket);
	fclose(fb);

}
void startBooking()
{
	booking = (ticket*)malloc(capacity * sizeof(ticket));
	readFile();
}

void gen_reference(char *s, int len) {
	srand(time(0));
	char temp[5];
	int i=0;
    char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    s[len]='\0';
    sprintf(temp,"%d",++bookedCount);
    strcat(s, temp);
}

void displayFlight(char dep[], char des[])
{
	int seatAvail;
	char depf[20], desf[20], fn[10], dep_t[10], arr_t[10], al[20];
	int faref=0;

	fi=fopen("src/flights.txt", "r");
	fo=fopen("src/availableFlights.txt", "w");
//	if(fi==NULL)
//		printf("Error");
//	else
//		printf("Working");
//scanf("%*d");
	int i=0;
	while( fscanf(fi, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d\n", depf, desf, al, fn, dep_t, arr_t, &faref, &seatAvail)!=EOF)
	{
		if(strcmp(dep, depf)==0&&strcmp(des, desf)==0)
		{
			printf("%d %s %s %s %s %s %s %d\n", ++i, depf, desf, al, fn, dep_t, arr_t, faref);
			fprintf(fo, "%d,%s,%s,%s,%s,%s,%s,%d,%d\n", i, depf, desf, al, fn, dep_t, arr_t, faref, seatAvail);
		}
	}

	fclose(fo);
}
void feedDetails(int choice)
{
	int i=0;
	char depf[20], desf[20], fn[10], dep_t[10], arr_t[10], al[20];
	int faref=0;
	fo=fopen("src/availableFlights.txt", "r");
	while( fscanf(fo, "%d,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%*d\n", &i, depf, desf, al, fn, dep_t, arr_t, &faref)!=EOF)
	{
		//printf("%d %s %s %s %s %s %d\n", i, depf, desf, fn, dep_t, arr_t, faref);
		if(i==choice)
		{
			strcpy(booking[nTicket].flight_number,fn);
			strcpy(booking[nTicket].dep_time,dep_t);
			strcpy(booking[nTicket].arr_time,arr_t);
			strcpy(booking[nTicket].airline,al);
			booking[nTicket].fare=faref;
			//printf("Fare is %d and %d and nTicket is %d", booking[nTicket].fare, faref, nTicket);
			break;
		}
	}

}
int count(int n)
{
	int i=0;
	while(n!=0)
	{
		n/=10;
		i++;
	}
	return i;
}
void remove_newline(char* a)
{
	a[strlen(a)-1]='\0';
}
void updateSeats(char* fn, int new_seatAvail)
{
	char ffn[10];
	int seatAvail,flag=0;
	fi=fopen("src/flights.txt", "r+");
	while(fscanf(fi, "%*[^,],%*[^,],%*[^,],%[^,],%*[^,],%*[^,],%*d,%d", ffn,&seatAvail)!=EOF)
	{
		if(strcmp(ffn,fn)==0)
		{
			flag=1;
			fseek(fi, -10, SEEK_CUR);
			if(new_seatAvail==-1)
				fprintf(fi, "%10d", --seatAvail);
			else
				fprintf(fi, "%10d", new_seatAvail);
			//printf("%d\n",seatAvail);
			fclose(fi);
			return;
		}
	}

	printf("Flight not found\n");
	fclose(fi);
}
bool isSeatAvailable(int n)
{
	int fn=0;
	int seatAvail;
	fo=fopen("src/availableFlights.txt", "r");
	while(fscanf(fo, "%d,%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],%*d,%d", &fn,&seatAvail)!=EOF)
	{


		if(fn==n)
		{
			if(seatAvail==0)
				return false;
			else
				return true;
		}
	}
}

void displayBooking(int x)
{
	//printf("Fare is %d and nTicket is %d", booking[nTicket].fare, nTicket);
	printf("Booking Reference: %s\n", booking[x].booking_reference);
	printf("Passenger Name: %s", booking[x].passenger_name);
	printf("Departure City: %s\n", booking[x].departure_city);
	printf("Destination City: %s\n", booking[x].destination_city);
	printf("Airline: %s\n", booking[x].airline);
	printf("Flight Number: %s\n", booking[x].flight_number);
	printf("Date: %s", booking[x].date);
	printf("Departure Time: %s\n", booking[x].dep_time);
	printf("Arrival Time: %s\n", booking[x].arr_time);
	printf("Fare: %d\n", booking[x].fare);
	fclose(fi);

}

void updateFare(char* fn, int new_fare)
{
	char ffn[10];
	int seatAvail,ffare;
	fi=fopen("src/flights.txt", "r+");
	while(fscanf(fi, "%*[^,],%*[^,],%*[^,],%[^,],%*[^,],%*[^,],%d,%d", ffn,&ffare,&seatAvail)!=EOF)
	{
		if(strcmp(ffn,fn)==0)
		{

			fseek(fi, -21, SEEK_CUR);
			if(new_fare==-1&&seatAvail==10)
				fprintf(fi, "%10d,%10d", 2*ffare,seatAvail);
			else
				fprintf(fi, "%10d,%10d", new_fare,seatAvail);
			//printf("%d\n",seatAvail);
			fclose(fi);
			return;
		}
	}
	printf("Flight not found");
	fclose(fi);
}

void newBooking()
{
	int choice;
	FILE *fb;
	int size=sizeof(ticket);
	fb =  fopen("src/bookings.dat", "a+b");
	increaseCapacity();
	++nTicket;
	printf("Enter departure city: ");
	scanf("%s", booking[nTicket].departure_city);
	printf("Enter destination city: ");
	scanf("%s", booking[nTicket].destination_city);
	displayFlight(booking[nTicket].departure_city, booking[nTicket].destination_city);
	printf("Select flight: ");
	scanf("%d",&choice);
	if(isSeatAvailable(choice)==false)
	{
		printf("Sorry no seat available");
		--nTicket;
		return;
	}

	scanf("%*c");
	feedDetails(choice);
	//printf("Fare is %d and nTicket is %d", booking[nTicket].fare, nTicket);
	printf("Enter passenger name: ");
	fgets(booking[nTicket].passenger_name, sizeof(booking[nTicket].passenger_name), stdin);
	//printf("Fare is %d and nTicket is %d", booking[nTicket].fare, nTicket);
	remove_newline(booking[nTicket].passenger_name);
	printf("Enter date: ");
	fgets(booking[nTicket].date, sizeof(booking[nTicket].date), stdin);
	remove_newline(booking[nTicket].date);
	gen_reference(booking[nTicket].booking_reference,10);
	booking[nTicket].active=true;
	updateSeats(booking[nTicket].flight_number, -1);
	displayBooking(nTicket);
	fwrite(&booking[nTicket], size, 1, fb);
	fclose(fb);

}

void change_fare()
{
	char fn[10];
	int fare;
	printf("Enter flight number: ");
	scanf("%s",fn);
	printf("Enter new fare: ");
	scanf("%d",&fare);
	updateFare(fn, fare);
}

int getIndex(char* br)
{
	int i=0;
	for(i=0;i<=nTicket;i++)
	{
		if(strcmp(booking[i].booking_reference,br)==0 && booking[i].active==true)
		{
			return i;
		}
	}
	return -1;
}
void findBooking()
{
	int index;
	char book_ref[20];
	printf("Enter booking reference: ");
	scanf("%s",book_ref);
	index=getIndex(book_ref);
	if(index==-1)
		printf("Booking not found");
	else
	{
		displayBooking(index);
	}

}

void deleteBooking()
{
	int index;
	char book_ref[20];
	printf("Enter booking reference: ");
	scanf("%s", book_ref);
	index=getIndex(book_ref);
	if(index==-1)
		printf("Booking not found");
	else
		booking[index].active=false;
	rewriteFile();
}
void addFlight()
{

	fi=fopen("src/flights.txt", "a+");
	char depf[20], desf[20], fn[10], dep_t[10], arr_t[10], al[20];
	int faref, seatAvail;
	printf("Enter departure city: ");
	fgets(depf, sizeof(depf), stdin);
	remove_newline(depf);
	printf("Enter destination city: ");
	fgets(desf, sizeof(desf), stdin);
	remove_newline(desf);
	printf("Enter airline: ");
	fgets(al, sizeof(al), stdin);
	remove_newline(al);
	printf("Enter flight number");
	fgets(fn, sizeof(fn), stdin);
	remove_newline(fn);
	printf("Enter departure time");
	fgets(dep_t, sizeof(dep_t), stdin);
	remove_newline(dep_t);
	printf("Enter arrival time: ");
	fgets(arr_t, sizeof(arr_t), stdin);
	remove_newline(arr_t);
	printf("Enter fare");
	scanf("%d",&faref);
	printf("Enter available seats: ");
	scanf("%d",&seatAvail);
	fprintf(fi,"%s,%s,%s,%s,%s,%s,%10d,%10d\n", depf, desf, al, fn, dep_t, arr_t, faref, seatAvail);
	fclose(fi);
}

void change_seatAvail()
{
	char fn[10];
	int seatAvail;
	printf("Enter flight number: ");
	scanf("%s",fn);
	printf("Enter number of available seats: ");
	scanf("%d",&seatAvail);
	updateSeats(fn, seatAvail);
}
char* encrypt(char *array)
{
    int i;
    int array_size=strlen(array);
    char secret[8] = { 22, 53, 44, 71, 66, 177, 253, 122 };
    for(i = 0; i < array_size; i++)
        array[i] = array[i] ^ secret[i];
    //printf("Program : %s",array);
    return array;
}
bool checkPass()
{
	FILE *fp;
	char passf[10],pass[10];
	printf("Enter password: ");
	scanf("%s",pass);

	fp=fopen("src/password.txt","r");
	fscanf(fp,"%s",passf);
	//printf("File: %s",passf);
	if(strcmp(encrypt(pass),passf)==0)
		printf("Authentication successful\n");
	else
	{
		printf("Incorrect password\n");
		return false;
	}
	return true;
	fclose(fp);
}




#endif /* BOOKING_H_ */
