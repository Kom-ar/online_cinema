#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>

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

static struct termios old, current;


List * init_list();
void add_to_list(List *head, FILE *fp);
List * make_list();
void print(List *head);
void del_el(List * el);
void initTermios(int echo);
void resetTermios(void);
char getch_(int echo);
char getch(void);
char getche(void);
void registration_user();
void log_in();

int main(){
	log_in();
	//registration_user();
	// List *first, *test;
 // 	char c;
 // 	int i, n;

	// first = make_list();
	// print(first);


	// while(1){
	// 	c = getch();
	// 	switch(c){
	// 		case 'a':
	// 			first = first->prev;
	// 			system("clear");
	// 			print(first);
	// 			break;
	// 		case 'd':
	// 			first = first->next;
	// 			system("clear");
	// 			print(first);
	// 			break;
	// 		default:
	// 			system("clear");
	// 			print(first);
	// 			printf("Error!!!\n");
	// 			printf("Use only 'a' or 'd'!!!");
	// 			break;

	// 	}
	// }
	// printf("\n\n\n");
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
	while(!feof(fp))
		add_to_list(head, fp);
	fclose(fp);

	return head;
}

void del_el(List * el){
	List *prev, *next;
	prev = el->prev;
	next = el->next;
	prev->next = el->next;
	next->prev = el->prev;
	free(el);
}

void print(List *head){
	List *cur, *prev, *nxt;
	cur = head;
	nxt = head->next;
	prev = head->prev;
	system("clear");
	printf("film name(prev) - %s ", prev->films.name);
	printf("film year(prev) - %s ", prev->films.year);
	printf("film country(prev) - %s ", prev->films.country);
	printf("film genre(prev) - %s ", prev->films.genre);
	printf("film grade(prev) - %s ", prev->films.grade);
	printf("\n\n\n");
	printf("film name(cur) - %s ", cur->films.name);
	printf("film year(cur) - %s ", cur->films.year);
	printf("film country(cur) - %s ", cur->films.country);
	printf("film genre(cur) - %s ", cur->films.genre);
	printf("film grade(cur) - %s ", cur->films.grade);
	printf("\n\n\n");
	printf("film name(nxt) - %s ", nxt->films.name);
	printf("film year(nxt) - %s ", nxt->films.year);
	printf("film country(nxt) - %s ", nxt->films.country);
	printf("film genre(nxt) - %s ", nxt->films.genre);
	printf("film grade(nxt) - %s ", nxt->films.grade);



	// do{
	// 	printf("film name - %s", p->films.name);
	// 	printf("film year - %s", p->films.year);
	// 	printf("film country - %s", p->films.country);
	// 	printf("film genre - %s", p->films.genre);
	// 	printf("film grade - %s", p->films.grade);
	// 	printf("\n________________________________________\n");
	// 	p = p->next;
	// }while(p != head);
}

/* Initialize new terminal i/o settings */
void initTermios(int echo){
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	current = old; /* make new settings same as old settings */
	current.c_lflag &= ~ICANON; /* disable buffered i/o */
	if (echo){
	    current.c_lflag |= ECHO; /* set echo mode */
	} 
	else {
	    current.c_lflag &= ~ECHO; /* set no echo mode */
	}
	tcsetattr(0, TCSANOW, &current); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void){

	tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo){
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char getch(void){
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void){
  return getch_(1);
}

void registration_user(){
	FILE *fp;
	User *new_user;
	fp = fopen("users.txt", "a");
	new_user = (User *)malloc(sizeof(User));
	printf("Login: ");
	scanf("%s", new_user->login);
	printf("Password: ");
	scanf("%s", new_user->password);
	printf("Your card number: ");
	scanf("%s", new_user->card_num);
	new_user->fav_num = 0;
	new_user->is_admin = 0;
	fprintf(fp, "%s\n%s\n%s\n%d\n%d\n", new_user->login, new_user->password, new_user->card_num, new_user->fav_num, new_user->is_admin);
	fclose(fp);

	printf("User: %s\n", new_user->login);
	printf("Password: %s\n", new_user->password);
	printf("Card number: %s\n", new_user->card_num);
	printf("fav_num: %d\n", new_user->fav_num);
	printf("Is admin: %d\n", new_user->is_admin);

}

void log_in(){
	FILE *fp;
	User *l_user;
	char log_u[22], log_f[22], password_u[22], password_f[22];
	fp = fopen("users.txt", "r");
	l_user = (User *)malloc(sizeof(User));
	printf("Login: ");
	scanf("%s", log_u);
	while(!feof(fp) && strcmp(log_u, l_user->login) != 0){
		fscanf(fp, "%s", l_user->login);
	}
	if(strcmp(log_u, l_user->login) == 0){
		printf("Password: ");
		scanf("%s", password_u);
		fscanf(fp, "%s", l_user->password);
		if(strcmp(password_u, l_user->password) == 0){
			printf("Password is correct!\n");
			fscanf(fp,"%s", l_user->card_num);
			fscanf(fp, "%d", &(l_user->fav_num));
			fscanf(fp, "%d", &(l_user->is_admin));
		}
		else{
			printf("Password is uncorrect!\n");
		}
	}
	else{
		printf("User not found!\n");
	}
	fclose(fp);
	printf("User: %s\n", l_user->login);
	printf("Password: %s\n", l_user->password);
	printf("Card number: %s\n", l_user->card_num);
	printf("fav_num: %d\n", l_user->fav_num);
	printf("Is admin: %d\n", l_user->is_admin);



}