//Made by Anirudh & Apeksha
#include<stdio.h>
#include<stdlib.h>
#include "booking.h"
int main()
{
	startBooking();
	int choice_user,choice, ch;
	while(true)
	{
		printf("Enter\n 1. Administrator\n 2. User\n 3. Exit\n");
		scanf("%d", &choice);
		scanf("%*c");

		switch(choice)
		{
		case 1:
			if(!checkPass())
				break;
			printf("Enter\n 1. Add new flight\n 2. Change seat availability\n 3. Update Fare\n");
			scanf("%d",&ch);
			scanf("%*c");
			switch(ch)
			{
			case 1:
				addFlight();
				break;
			case 2:
				change_seatAvail();
				break;
			case 3:
				change_fare();
				break;

			}
			break;
		case 2:

			printf("Enter\n 1. New Booking\n 2. Display Booking\n");
			scanf("%d",&choice_user);
			switch(choice_user)
			{
			case 1:
				newBooking();
				break;
			case 2:
				findBooking();
				break;

			}
			break;

		case 3:
			exit(0);
			break;
		}


	}

}
