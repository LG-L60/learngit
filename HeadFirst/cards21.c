#include<stdio.h>
#include<stdlib.h>

int main()
{
	char card_name[3];
	puts("input cards name: ");
	scanf("%2s", card_name);
	int val = 0;
	if (card_name[0] == 'K')
{	val = 10;	}
	else if (card_name == 'Q')
{	val = 10;	}
	else if (card_name == 'J')
{	val = 10;	}
	else if (card_name == 'A')
{	val = 11;	}
	else{	val = atoi(card_name);	}
	if ((val > 2)&&(val < 7))
	puts("month add");
	else if (val == 10)
	puts("month readuce");
	return 0;	


}
