#include <stdio.h>
#include <stdlib.h>

typedef struct film{
	char name[100];
	char year[10];
	char country[100];
	char genre[100];
	char grade[5];
}Film;

typedef struct list{
	Film films;
	struct list *next;
	struct list *prev;
}List;

typedef struct users{
	char login[22];
	char password[22];
	char card_num[17];
	int fav_num;
	int is_admin;
}User;

List * init_list();
void add_to_list(List *head, FILE *fp);
List * make_list();
void print(List *head);

int main(){
	List *first;
	//add_to_list(head);
	first = make_list();
	print(first);
}


List * init_list(FILE *fp){
	List *lst;
	lst = (List *)malloc(sizeof(List));

	fgets(lst->films.name, 100, fp);
	fgets(lst->films.year, 10, fp);
	fgets(lst->films.country, 100, fp);
	fgets(lst->films.genre, 100, fp);
	fgets(lst->films.grade, 5, fp);


	lst->next = lst;
	lst->prev = lst;
	return lst;

}

void add_to_list(List *head, FILE *fp){
	List *p, *film;
	film = (List *)malloc(sizeof(List));
	fgets(film->films.name, 100, fp);
	fgets(film->films.year, 10, fp);
	fgets(film->films.country, 100, fp);
	fgets(film->films.genre, 100, fp);
	fgets(film->films.grade, 5, fp);

	p = head->next;
	head->next = film;
	film->next = p;
	film->prev = head;
	p->prev = film;


}

List * make_list(){
	FILE *fp;
	List *head;
	fp = fopen("films.txt", "r");
	head = init_list(fp);
	//for(int i=0; i<29; i++)
	while(!feof(fp))
		add_to_list(head, fp);
	fclose(fp);

	return head;
}

void print(List *head){
	List *p;
	p = head;
	do{
		printf("film name - %s", p->films.name);
		printf("film year - %s", p->films.year);
		printf("film country - %s", p->films.country);
		printf("film genre - %s", p->films.genre);
		printf("film grade - %s", p->films.grade);
		printf("\n________________________________________\n");
		p = p->next;
	}while(p != head);
}