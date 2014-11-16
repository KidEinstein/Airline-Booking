/**
 * @mainpage  DSC Project
 *
 * \author Anirudh Singh & Apeksha K.
 *
 * \version 1.1
 *
 * \date 10/11/2014
 */
/**
 * @file
 *
 * \note DSC Project
 *
 * \author Anirudh Singh & Apeksha K.
 *
 * \version 1.1
 *
 * \date 10/11/2014
 *
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#ifndef BOOKING_H_
#define BOOKING_H_
typedef enum { false, true } bool;
///Number of tickets booked
int nTicket=-1;
///Number of bookings that the structure array can store
int capacity=1;
/**
 * \brief Structure to store the bookings done
 *
 * Contains fields to store the details of the passenger and the details of the journey
 */
typedef struct
{
	///Name of the Passenger
	char passenger_name[30];
	///Departure City
	char departure_city[20];
	///Destination City
	char destination_city[20];
	///Name of the airline
	char airline[20];
	///Flight Number
	char flight_number[10];
	///Date of Journey
	char date[10];
	///Reference number of the booking
	char booking_reference[20];
	///Fare of the ticket
	int fare;
	///Status of booking - Valid or Deleted
	bool active;
	///Departure time
	struct tm dep;
	///Arrival time
	struct tm arr;


}ticket;
///Pointer to the structure array containing bookings
ticket *booking;
/// File  variable for flights.txt
FILE* fo;
/// File variable for availableFlights.txt
FILE* fi;
/**
 * \brief Increases the capacity of the structure
 *
 * Increases capacity of the structure if the array of structures is completely filled.
 *
 */
void increaseCapacity()
{
	if(nTicket==capacity-1)
		{
			capacity*=2;
			booking=(ticket*)realloc(booking, capacity*sizeof(ticket));
		}
}
/**
 * \brief Rewrites file containing bookings
 *
 * Rewrites the file containing the bookings
 *  after deleting a booking (active changed to false)
 */
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
/**
 * \brief Reads file containing the bookings
 *
 * Opens bookings.dat and copies the structure variables present into  structure array
 */
void readFile()
{
	FILE *fb;
	fb =  fopen("src/bookings.dat","r");
	int size=sizeof(ticket);

	while(fread(&booking[++nTicket], size, 1, fb)==1)
		increaseCapacity();
	--nTicket;
	fclose(fb);

}
/**
 * \brief Initiates booking
 *
 * Called in the beginning to open the bookings.dat file and allocate memory for a structure object
 *
 */
void startBooking()
{
	booking = (ticket*)malloc(capacity * sizeof(ticket));
	readFile();
}
/**
 * \brief Generates booking reference number
 *
 * Generates a random alphanumeric reference number associated with each booking
 * the last digit representing the number of tickets booked till now
 * @param s String to store the reference number
 * @param len Length of the required reference number
 */
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
    sprintf(temp,"%d",nTicket);
    strcat(s, temp);
}
/**
 * \brief Display flights for the route
 *
 * Displays all the flights available flying from the entered departure city to the destination city
 * @param dep Name of Departure City
 * @param des Name of Destination City
 * @return True if some flights are found False if no flights are found
 */
bool displayFlight(char dep[], char des[])
{
	int seatAvail;
	char depf[20], desf[20], fn[10], dep_t[10], arr_t[10], al[20];
	int faref=0;

	fi=fopen("src/flights.txt", "r");
	fo=fopen("src/availableFlights.txt", "w");
	int i=0,flag=0;
	printf("S.No \t Departure City \t Destination City \t Airline \t\t Flight Number \t Departure Time \t Arrival Time \t Fare\n");
	while( fscanf(fi, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d\n", depf, desf, al, fn, dep_t, arr_t, &faref, &seatAvail)!=EOF)
	{
		if(strcmp(dep, depf)==0&&strcmp(des, desf)==0)
		{
			flag=1;
			printf("%d \t %-10s \t %-10s \t\t %-10s \t\t %s \t\t %s \t\t %s \t\t %d\n", ++i, depf, desf, al, fn, dep_t, arr_t, faref);
			fprintf(fo, "%d,%s,%s,%s,%s,%s,%s,%d,%d\n", i, depf, desf, al, fn, dep_t, arr_t, faref, seatAvail);
		}
	}

	fclose(fo);
	if(flag==0)
		return false;
	else
		return true;
}
/**
 * \brief Extract details of the flight selected
 *
 * Copies details of the booking to the structure variable according to the choice of the flight entered by the user
 * @param choice Index representing the flight selected by the user
 */
void feedDetails(int choice)
{
	int i=0,dep_h,dep_m, arr_h, arr_m;
	char depf[20], desf[20], fn[10], al[20];
	int faref=0;
	fo=fopen("src/availableFlights.txt", "r");
	while( fscanf(fo, "%d,%[^,],%[^,],%[^,],%[^,],%d:%d,%d:%d,%d,%*d\n", &i, depf, desf, al, fn, &dep_h, &dep_m, &arr_h, &arr_m, &faref)!=EOF)
	{

		if(i==choice)
		{
			strcpy(booking[nTicket].flight_number,fn);
			strcpy(booking[nTicket].airline,al);
			booking[nTicket].fare=faref;
			booking[nTicket].dep.tm_hour=dep_h;
			booking[nTicket].dep.tm_min=dep_m;
			booking[nTicket].arr.tm_hour=arr_h;
			booking[nTicket].arr.tm_min=arr_m;

			break;
		}
	}

}
/**
 * \brief Counts the number of digits in a number
 * @param n Number whose digits need to be counted
 * @return Number of digits in the number passed as argument
 */
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
/**
 * \brief Remove trailing new line character
 *
 * Replaces the trailing new line character in the string (added by fgets()) with a null character
 */
void remove_newline(char* a)
{
	a[strlen(a)-1]='\0';
}
/**
 * \brief Modifies the seat availability
 *
 * Modifies the number of seats of the entered flight, in the file flights.txt
 *
 * @param fn Flight Number of the flight whose seat availability needs to be changed
 * @param new_seatAvail New number of seats for the selected flight
 */
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
			fclose(fi);
			return;
		}
	}

	printf("Flight not found\n");
	fclose(fi);
}
/**
 * \brief Checks for seat availability
 *
 * Checks if the number of seats available is 0 for the flight selected by the user
 *
 * @param n Index of flight selected by the user
 * @return True if seats available false otherwise
 */
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
/**
 * \brief Displays all the details of a booking
 *
 * Displays the details of the booking at the specified index in the structure array
 * @param x Index of the booking present in the structure array
 */
void displayBooking(int x)
{
	long num_seconds,minutes,hours,days;
	printf("Booking Reference: %s\n", booking[x].booking_reference);
	printf("Passenger Name: %s\n", booking[x].passenger_name);
	printf("Departure City: %s\n", booking[x].departure_city);
	printf("Destination City: %s\n", booking[x].destination_city);
	printf("Airline: %s\n", booking[x].airline);
	printf("Flight Number: %s\n", booking[x].flight_number);
	printf("Fare: %d\n", booking[x].fare);
	printf("Departure Time: %s", asctime(&booking[x].dep));
	printf("Arrival Time: %s", asctime(&booking[x].arr));
	num_seconds=difftime(mktime(&booking[x].dep),time(NULL));
	days = num_seconds / (60 * 60 * 24);
	num_seconds -= days * (60 * 60 * 24);
	hours = num_seconds / (60 * 60);
	num_seconds -= hours * (60 * 60);
	minutes = num_seconds / 60;
	num_seconds -= minutes*60;
	printf("Time left: %ld days %ld hours %ld minutes %ld seconds\n", days,hours,minutes,num_seconds);


}
/**
 * \brief Modifies the fare
 *
 * Modifies the fare of the entered flight, in the file flights.txt
 *
 * @param fn Flight Number of the flight whose fare needs to be changed
 * @param new_fare New fare for the selected flight
 */
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
			fclose(fi);
			return;
		}
	}
	printf("Flight not found");
	fclose(fi);
}
/**
 * \brief Records a new booking
 *
 * Makes a new booking by taking inputs from the user and adds it to the structure array
 */
void newBooking()
{
	int choice,d,m,y;
	FILE *fb;
	int size=sizeof(ticket);
	fb =  fopen("src/bookings.dat", "a+b");
	increaseCapacity();
	++nTicket;
	printf("Enter departure city: ");
	scanf("%s", booking[nTicket].departure_city);
	printf("Enter destination city: ");
	scanf("%s", booking[nTicket].destination_city);
	if(!displayFlight(booking[nTicket].departure_city, booking[nTicket].destination_city))
	{
		printf("No flight found\n");
		--nTicket;
		return;
	}

	printf("Select flight: ");
	scanf("%d",&choice);
	if(isSeatAvailable(choice)==false)
	{
		printf("Sorry no seat available\n");
		--nTicket;
		return;
	}

	scanf("%*c");
	feedDetails(choice);
	printf("Enter passenger name: ");
	fgets(booking[nTicket].passenger_name, sizeof(booking[nTicket].passenger_name), stdin);
	remove_newline(booking[nTicket].passenger_name);
	printf("Enter date in the format dd:mm:yyyy : ");
	scanf("%d:%d:%d",&d,&m,&y);
	booking[nTicket].dep.tm_mday=d;
	booking[nTicket].dep.tm_mon=m-1;
	booking[nTicket].dep.tm_year=y-1900;
	booking[nTicket].arr.tm_mday=booking[nTicket].dep.tm_mday;
	booking[nTicket].arr.tm_mon=booking[nTicket].dep.tm_mon;
	booking[nTicket].arr.tm_year=booking[nTicket].dep.tm_year;

	gen_reference(booking[nTicket].booking_reference,10);
	booking[nTicket].active=true;
	updateSeats(booking[nTicket].flight_number, -1);
	displayBooking(nTicket);
	fwrite(&booking[nTicket], size, 1, fb);
	fclose(fb);

}
/**
 * \brief Queries about flight fare change
 *
 * Takes input of flight number and the new fare from the administrator
 */
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
/**
 * \brief Finds the index of the booking
 *
 * Traverses through the structure array and locates the index of the booking
 * @param br Booking reference number to search for
 * @return Index of the booking in the array corresponding to the booking reference number
 */
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
/**
 * \brief Queries about the booking
 *
 * Takes input of the booking reference number from the user, if found
 * displays the booking otherwise informs the user that the booking was not found
 */
void findBooking()
{
	int index;
	char book_ref[20];
	printf("Enter booking reference: ");
	scanf("%s",book_ref);
	index=getIndex(book_ref);
	if(index==-1)
		printf("Booking not found\n");
	else
	{
		displayBooking(index);
	}

}
/**
 * \brief Deletes a booking
 *
 * Changes the active attribute to false, rendering the booking deleted
 *
 */
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
/**
 * \brief Adds a new flight
 *
 * Adds a new flight to the database, entering all the details and storing it in the file flights.txt
 */
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
	printf("Enter flight number: ");
	fgets(fn, sizeof(fn), stdin);
	remove_newline(fn);
	printf("Enter departure time: ");
	fgets(dep_t, sizeof(dep_t), stdin);
	remove_newline(dep_t);
	printf("Enter arrival time: ");
	fgets(arr_t, sizeof(arr_t), stdin);
	remove_newline(arr_t);
	printf("Enter fare: ");
	scanf("%d",&faref);
	printf("Enter available seats: ");
	scanf("%d",&seatAvail);
	fprintf(fi,"%s,%s,%s,%s,%s,%s,%10d,%10d\n", depf, desf, al, fn, dep_t, arr_t, faref, seatAvail);
	fclose(fi);
}
/**
 * \brief Queries about the flight seat availability change
 *
 * Takes input of flight number and the new seat availability from the administrator
 */
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
/**
 * \brief Encrypts a string
 *
 * Encrypts the string using XOR operations on its characters
 * @param array String to be encrypted
 * @return Encrypted string
 */
char* encrypt(char *array)
{
    int i;
    int array_size=strlen(array);
    char secret[8] = { 22, 53, 44, 71, 66, 177, 253, 122 };
    for(i = 0; i < array_size; i++)
        array[i] = array[i] ^ secret[i];
    return array;
}
/**
 * \brief Queries for password
 *
 * Matches the password entered with the password stored in file
 * @return True if passwords match, false otherwise
 */
bool checkPass()
{
	FILE *fp;
	char passf[10],pass[10];
	printf("Enter password: ");
	scanf("%s",pass);

	fp=fopen("src/password.txt","r");
	fgets (passf , 10 , fp);
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
/**
 * \brief Changes administrator password
 *
 * Asks the administrator to enter a new password, encrypts it and stores it in the file password.txt
 */
void change_password()
{
	FILE *fp;
	char pass[10];
	printf("Enter password: ");
	scanf("%s",pass);

	fp=fopen("src/password.txt","w");

	fprintf(fp,"%s",encrypt(pass));
	fclose(fp);
}


#endif /* BOOKING_H_ */
