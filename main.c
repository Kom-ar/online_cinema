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


List * init_list(FILE *fp);
void add_to_list(List *head, FILE *fp);
List * make_list(char file_name[50]);
void print(List *head);
void del_el(List * el);
void initTermios(int echo);
void resetTermios(void);
char getch_(int echo);
char getch(void);
char getche(void);
void registration_user();
User * log_in();
List * init_fav_list(User *user, List *film_list);
List * add_to_fav_list(User *user, List *film_list, List *fav_list);
List * film_catalog(List *film, List *fav_film, User *user);
void favorites_catalog(List *fav_film, User *user);
int main_menu();
void fav_to_file(User *user, List *fav_film);
void change_fav_num(User *user);
int file_lines(char file_name[50]);
void personal_account(User *user);
void change_login(User *user);
void change_password(User *user);
void change_card_number(User *user);


int main(){
	// printf("users = %d\n", file_lines("users.txt"));
	// printf("films = %d\n", file_lines("films.txt"));
	// printf("favorites = %d\n", file_lines("favorites/favorites_Virginia"));
	User *user;
	//registration_user();
	// registration_user();
	// registration_user();
	user = log_in();

	List *first, *first_fav;
	FILE *films, *fav_films;	
 	char c;
 	char way[50] = "favorites/favorites_";
 	int i, n;
 	strcat(way, user->login);
	fav_films = fopen(way, "r");
	films = fopen("films.txt", "r");

 	if(user->fav_num != 0)
 		first_fav = make_list(way);

	first = make_list("films.txt");
	fclose(films);
	fclose(fav_films);


	while(1){
		switch(main_menu()){
			case '1':
				first_fav = film_catalog(first, first_fav, user);
				break;
			case '2':
				if(user->fav_num == 0)
					printf("Your favorites list is EMPTY!\n");
				else
					favorites_catalog(first_fav, user);
				break;
			case '3':
				personal_account(user);
				break;
			case '4':
				if(user->fav_num != 0){
					fav_to_file(user, first_fav);
					change_fav_num(user);
				}
				system("rm users.txt");
				system("mv new_users.txt users.txt");
				exit(0);
				break;
			default:
				break;
		}
	}


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

List * make_list(char file_name[50]){
	List *head;
	FILE *fp = fopen(file_name, "r");
	int k = 0;
	head = init_list(fp);
	k += 5;
	while(k < file_lines(file_name)){
		//if(k == file_lines(file_name))
		//	break;
		add_to_list(head, fp);
		k += 5;
	}

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
	char way[50] = "favorites/favorites_";
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

	strcat(way, new_user->login);
	fp = fopen(way, "w");
	fclose(fp);

	printf("User: %s\n", new_user->login);
	printf("Password: %s\n", new_user->password);
	printf("Card number: %s\n", new_user->card_num);
	printf("fav_num: %d\n", new_user->fav_num);
	printf("Is admin: %d\n", new_user->is_admin);

}

User * log_in(){
	FILE *fp;
	User *l_user;
	char log_u[22], password_u[22];
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
	return l_user;
}

List * init_fav_list(User *user, List *film_list){
	List *lst;
	lst = (List *)malloc(sizeof(List));
	strcpy(lst->films.name, film_list->films.name);
	strcpy(lst->films.year, film_list->films.year);
	strcpy(lst->films.country, film_list->films.country);
	strcpy(lst->films.genre, film_list->films.genre);
	strcpy(lst->films.grade ,film_list->films.grade);



	lst->next = lst;
	lst->prev = lst;
	user->fav_num++;
	return lst;
}

List * add_to_fav_list(User *user, List *film_list, List *fav_list){
	List *p, *film;
	film = (List *)malloc(sizeof(List));

	strcpy(film->films.name, film_list->films.name);
	strcpy(film->films.year, film_list->films.year);
	strcpy(film->films.country, film_list->films.country);
	strcpy(film->films.genre, film_list->films.genre);
	strcpy(film->films.grade, film_list->films.grade);

	p = fav_list->next;
	fav_list->next = film;
	film->next = p;
	film->prev = fav_list;
	p->prev = film;

	user->fav_num++;
	return film;
}

List * film_catalog(List *film, List *fav_film, User *user){
	char c;
	print(film);
	int f = 1;
	while(f){
		c = getch();
		switch(c){
			case 'a':
				film = film->prev;
				system("clear");
				print(film);
				break;
			case 'd':
				film = film->next;
				system("clear");
				print(film);
				break;
			case 'l':
				if(user->fav_num == 0){
					fav_film = init_fav_list(user, film);
				}
				else{
					fav_film = add_to_fav_list(user, film, fav_film);
				}
				print(film);
				break;
			case 'e':
				f = 0;
				//favorites_catalog(fav_film, user);
				break;
			default:
				system("clear");
				print(film);
				printf("Error!!!\n");
				printf("Use only 'a' or 'd'!!!");
				break;

		}
	}
	printf("\n\n\n");	
	return fav_film;
}

void favorites_catalog(List *fav_film, User *user){
	char c;
	int f = 1;
	print(fav_film);
	while(f){
		c = getch();
		switch(c){
			case 'a':
				fav_film = fav_film->prev;
				system("clear");
				print(fav_film);
				break;
			case 'd':
				fav_film = fav_film->next;
				system("clear");
				print(fav_film);
				break;
			case 'g':
				fav_film = fav_film->next;
				del_el(fav_film->prev);
				user->fav_num--;
				print(fav_film);
				break;
			case 'e':
				f = 0;
				break;
			default:
				system("clear");
				print(fav_film);
				printf("Error!!!\n");
				printf("Use only 'a' or 'd'!!!");
				break;

		}
	}
	printf("\n\n\n");	

}

int main_menu(){
	char c;
	system("clear");
	printf("\t\t\t\t\t\tMAIN MENU\n\n\n\n");
	printf("1|Film catalog\n");
	printf("2|Your favorites film\n");
	printf("3|Personal account\n");
	printf("4|Exit\n\n\n");

	c = getche();
	return c;
}

void fav_to_file(User *user, List *fav_film){
	char way[50] = "favorites/favorites_";
	strcat(way, user->login);
	FILE *fp = fopen(way, "w");
	List *p = fav_film;

	do{
		fputs(p->films.name, fp);
		fputs(p->films.year, fp);
		fputs(p->films.country, fp);
		fputs(p->films.genre, fp);
		fputs(p->films.grade, fp);

		p = p->next;

	}while(p != fav_film);
	fclose(fp);
}

void change_fav_num(User *user){
	char log[22];
	FILE *f_old_user_list, *f_new_user_list;
	f_old_user_list = fopen("users.txt", "r");
	f_new_user_list = fopen("new_users.txt", "w");
	int fav = user->fav_num;
	int k = 0;

	while(!feof(f_old_user_list)){
		if(k == file_lines("users.txt"))
			break;
		fscanf(f_old_user_list, "%s", log);
		fprintf(f_new_user_list, "%s\n", log);
		k++;


		if(strcmp(log, user->login) == 0){

			fscanf(f_old_user_list, "%s", log);
			fprintf(f_new_user_list, "%s\n", log);

			fscanf(f_old_user_list, "%s", log);
			fprintf(f_new_user_list, "%s\n", log);

			fscanf(f_old_user_list, "%s", log);			
			fprintf(f_new_user_list, "%d\n", fav);
			k += 3;
		}
	}
	fclose(f_old_user_list);
	fclose(f_new_user_list);
}

int file_lines(char file_name[50]){
	FILE *fp = fopen(file_name, "r");
	int k = 0;
	while(!feof(fp))
		if(fgetc(fp) == '\n')
			k++;
	fclose(fp);
	return k;
}

void personal_account(User *user){
	char c;
	int f = 1;
	while(f){
		printf("\n\n\n\n1|Change your login\n");
		printf("2|Change your password\n");
		printf("3|Change your card number\n");
		printf("4|Return to main menu\n\n\n");

		c = getch();
		switch(c){
			case '1':
				change_login(user);
				system("rm users.txt");
				system("mv new_users.txt users.txt");
				break;
			case '2':
				change_password(user);
				system("rm users.txt");
				system("mv new_users.txt users.txt");
				break;
			case '3':
				change_card_number(user);
				system("rm users.txt");
				system("mv new_users.txt users.txt");
				break;				
			case '4':
				f = 0;
				break;
		}
	}
}

void change_login(User *user){
	char log[22];
	FILE *f_old_user_list, *f_new_user_list;
	f_old_user_list = fopen("users.txt", "r");
	f_new_user_list = fopen("new_users.txt", "w");
	int k = 0;
	char old_login[22], old_name[50] = "favorites/favorites_";
	char new_name[50] = "favorites/favorites_";

	strcpy(old_login, user->login);
	strcat(old_name, old_login);

	printf("\n\nEnter new login: ");
	scanf("%s", user->login);
	strcat(new_name, user->login);
	rename(old_name, new_name);

	while(!feof(f_old_user_list)){
		if(k == file_lines("users.txt"))
			break;
		fscanf(f_old_user_list, "%s", log);
		if(strcmp(log, old_login) == 0){
			fprintf(f_new_user_list, "%s\n", user->login);
			k++;
			continue;
		}

		fprintf(f_new_user_list, "%s\n", log);
		k++;
	}
	printf("Your login success changed!\n");
	fclose(f_old_user_list);
	fclose(f_new_user_list);


}

void change_password(User *user){
	char current_pw[22], new_pw[22], log[22];
	int k = 0;
	FILE *f_old_user_list, *f_new_user_list;
	f_old_user_list = fopen("users.txt", "r");
	f_new_user_list = fopen("new_users.txt", "w");

	while(1){
		printf("Enter your current password: ");
		scanf("%s", current_pw);
		if(strcmp(current_pw, user->password) != 0){
			printf("Uncorrect password! Try again!\n");
			continue;
		}
		else
			break;
	}
	printf("Enter your new password: ");
	scanf("%s", new_pw);
	// printf("Confirm your new password: ");
	// scanf("%s", new_pw2)

	strcpy(user->password, new_pw);

	while(!feof(f_old_user_list)){
		if(k == file_lines("users.txt"))
			break;
		fscanf(f_old_user_list, "%s", log);
		fprintf(f_new_user_list, "%s\n", log);
		k++;
		if(strcmp(log, user->login) == 0){
			fscanf(f_old_user_list, "%s", log);
			fprintf(f_new_user_list, "%s\n", user->password);
			k++;
		}
	}
	printf("Your password success changed!\n");
	fclose(f_old_user_list);
	fclose(f_new_user_list);

}

void change_card_number(User *user){
	char new_card_number[17], log[22];
	int k = 0;
	FILE *f_old_user_list, *f_new_user_list;
	f_old_user_list = fopen("users.txt", "r");
	f_new_user_list = fopen("new_users.txt", "w");

	printf("Enter your new card number: ");
	scanf("%s", new_card_number);
	strcpy(user->card_num, new_card_number);

	while(!feof(f_old_user_list)){
		if(k == file_lines("users.txt"))
			break;
		fscanf(f_old_user_list, "%s", log);
		fprintf(f_new_user_list, "%s\n", log);
		k++;
		if(strcmp(log, user->login) == 0){
			fscanf(f_old_user_list, "%s", log);
			fprintf(f_new_user_list, "%s\n", log);

			fscanf(f_old_user_list, "%s", log);
			fprintf(f_new_user_list, "%s\n", user->card_num);
			k += 2;
		}
	}
	printf("Your password success changed!\n");
	fclose(f_old_user_list);
	fclose(f_new_user_list);

}