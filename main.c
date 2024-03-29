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
int cheking_login(User *new_user);
int cheking_password(User *new_user);
int cheking_card_num(User *new_user);
User * log_in();
List * init_fav_list(User *user, List *film_list);
List * add_to_fav_list(User *user, List *film_list, List *fav_list);
List * film_catalog(List *film, List *fav_film, User *user);
void favorites_catalog(List *fav_film, User *user);
int main_menu();
void fav_to_file(User *user, List *fav_film);
void change_fav_num(User *user);
int file_lines(char file_name[50]);
void personal_account(User *user, List *head, List *fav_list);
void change_login(User *user);
void change_password(User *user);
void change_card_number(User *user);
int add_film(List *head);
void delete_film(List *film, User *user, List *fav_list);
void out_str_center(char* string, int length_str);
void print_films(List* list);
int is_in_fav_list(List *film, List *fav_list);
int is_film_in_list(List *film, char name[100]);
int check_grade(char grade[5]);
int unique_login(User *user);

int main(){
	User *user;
	int n, f = 1;
	while(f){
		system("clear");
		printf("1|Registration\n");
		printf("2|Log in\n");
		printf("3|Exit\n\n\n\n");
		n = getch();
		switch(n){
			case '1':
				registration_user();
				break;
			case '2':
				if((user = log_in()) != NULL)
					f = 0;
				break;
			case '3':
				exit(0);
				break;
			default:
				break;
		}
	}

	List *first, *first_fav;
	FILE *films, *fav_films;	
 	char c;
 	char way[50] = "favorites/favorites_";
 	int i;
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
				system("clear");
				first_fav = film_catalog(first, first_fav, user);
				break;
			case '2':
				if(user->fav_num == 0){
					printf("Your favorites list is EMPTY!\n");
					sleep(1);
				}
				else
					favorites_catalog(first_fav, user);
				break;
			case '3':
				personal_account(user, first, first_fav);
				break;
			case '4':
				if(user->fav_num != 0){
					fav_to_file(user, first_fav);
					change_fav_num(user);
					system("rm users.txt");
					system("mv new_users.txt users.txt");
				}
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
	while(1){
		printf("Login: ");
		scanf("%s", new_user->login);
		if(cheking_login(new_user) && !unique_login(new_user)){
			break;
		}
		else
			continue;
	}
	while(1){
		printf("Password: ");
		scanf("%s", new_user->password);
		if(cheking_password(new_user))
			break;
		else
			continue;
	}
	while(1){
		printf("Your card number: ");
		scanf("%s", new_user->card_num);
		if(cheking_card_num(new_user))
			break;
		else
			continue;

	}
	new_user->fav_num = 0;
	new_user->is_admin = 0;
	fprintf(fp, "%s\n%s\n%s\n%d\n%d\n", new_user->login, new_user->password, new_user->card_num, new_user->fav_num, new_user->is_admin);
	fclose(fp);

	strcat(way, new_user->login);
	fp = fopen(way, "w");
	fclose(fp);


}

User * log_in(){
	FILE *fp;
	User *l_user;
	char log_u[22], password_u[22], pw_f[22];
	fp = fopen("users.txt", "r");
	l_user = (User *)malloc(sizeof(User));
	printf("Login: ");
	scanf("%s", log_u);
	while(!feof(fp) && strcmp(log_u, l_user->login) != 0){
		fscanf(fp, "%s", l_user->login);
	}
	if(strcmp(log_u, l_user->login) == 0){
		fscanf(fp, "%s", pw_f);
		while(1){
			printf("Password: ");
			scanf("%s", password_u);
			if(strcmp(password_u, pw_f) == 0){
				printf("Password is correct!\n");
				strcpy(l_user->password, pw_f);
				fscanf(fp,"%s", l_user->card_num);
				fscanf(fp, "%d", &(l_user->fav_num));
				fscanf(fp, "%d", &(l_user->is_admin));
				sleep(1);
				break;
			}
			else{
				printf("Password is uncorrect!Try again!\n");
				continue;
			}
		}
	}
	else{
		printf("User not found!\n");
		sleep(1);
		return NULL;
	}
	fclose(fp);
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
					if(is_in_fav_list(film, fav_film)){
						printf("This film already in your favorites list!\n");
						sleep(1);
						printf("Choose another one!\n");
						sleep(1);
					}
					else{
						fav_film = add_to_fav_list(user, film, fav_film);
						printf("Film added to your favorite list!\n");
						sleep(1);
					}
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

void personal_account(User *user, List *head, List *fav_list){
	char c;
	int f = 1;
	if(user->is_admin == 1){
		while(f){
			system("clear");
			printf("\n\n\n\n1|Change your login\n");
			printf("2|Change your password\n");
			printf("3|Change your card number\n");
			printf("4|Add new film(A)\n");
			printf("5|Delete film(A)\n");
			printf("6|Return to main menu\n\n\n");

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
					add_film(head);
					break;
				case '5':
					delete_film(head, user, fav_list);
				case '6':
					f = 0;
					break;
			}
		}
	}
	else{
		while(f){
			system("clear");
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
	while(1){
		printf("\n\nEnter new login: ");
		scanf("%s", user->login);
		if(cheking_login(user))
			break;
		else
			continue;
	}
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
	char current_pw[22], new_pw[22], con_pw[22], log[22];
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
	while(1){
		printf("Enter your new password: ");
		scanf("%s", user->password);
		if(cheking_password(user))
			break;
		else
			continue;
	}
	while(1){
		printf("Confirm your password: ");
		scanf("%s", con_pw);
		if(strcmp(user->password, con_pw) == 0)
			break;
		else{
			printf("Passwords don't match!\nTry again!\n");
			continue;
		}
	}
	//strcpy(user->password, new_pw);

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

	while(1){
		printf("Enter your new card number: ");
		scanf("%s", user->card_num);
		if(cheking_card_num(user))
			break;
		else
			continue;
	}
	//strcpy(user->card_num, new_card_number);

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

int add_film(List *head){
	char name[100], year[10], country[100], genre[100], grade[5];
	FILE *fp = fopen("films.txt", "r");
	printf("Enter the name of film: ");
	fgets(name, 100, stdin);
	if(is_film_in_list(head, name)){
		printf("That film is already in the catalog!\n");
		sleep(1);
		fclose(fp);
		return 0;
	}
	printf("Enter year: ");
	fgets(year, 10, stdin);
	printf("Enter the country or countries: ");
	fgets(country, 100, stdin);
	printf("Enter genre or genres: ");
	fgets(genre, 100, stdin);
	while(1){
		printf("Enter the grade of film: ");
		fgets(grade, 5, stdin);
		if(check_grade(grade)){
			printf("Wrong format!(The grade must be of the form '7.8', '6,5', '3.7', '9.2', etc.)\n");
			sleep(2);
			continue;
		}
		else
			break;
	}

	fclose(fp);
	List *p, *film;
	fp = fopen("films.txt", "a+");
	film = (List *)malloc(sizeof(List));
	strcpy(film->films.name, name);
	strcpy(film->films.year, year);
	strcpy(film->films.country, country);
	strcpy(film->films.genre, genre);
	strcpy(film->films.grade, grade);

	p = head->next;
	head->next = film;
	film->next = p;
	film->prev = head;
	p->prev = film;

	fprintf(fp, "%s", name);
	fprintf(fp, "%s", year);
	fprintf(fp, "%s", country);
	fprintf(fp, "%s", genre);
	fprintf(fp, "%s", grade);

	fclose(fp);
}

void delete_film(List *film, User *user, List *fav_list){
	char c, n;
	int f = 1;
	char name[100], log[100];
	int k = 0, m = 0, x = 0;

	while(f){
		system("clear");
		print(film);		
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
			case 'g':
				printf("Are you sure that you want to delete '%s' from film catalog?\n\n\n", film->films.name);
				printf("1|Yes\t\t\t\t\t\t\t2|No\n\n");
				n = getch();
				if(n == '1'){
					// char name[100], log[100];
					// int k = 0, m = 0;
					FILE *film_list, *new_film_list;
					film_list = fopen("films.txt", "r");
					new_film_list = fopen("new_film_list.txt", "w");
					strcpy(name, film->films.name);

					List *p = fav_list;
					do{
						if(strcmp(p->films.name, name) == 0){
							x = 1;
							break;
						}
						p = p->next;
					}while(p != fav_list);
					if(x){
						fav_list = p;
						fav_list = fav_list->next;
						del_el(fav_list->prev);
						user->fav_num--;
					}


					while(!feof(film_list)){
						if(k == file_lines("films.txt"))
							break;
						fgets(log, 100, film_list);
						if(strcmp(log, name) == 0){
							for(int i=0; i<4; i++)
								fgets(log, 100, film_list);
							k += 5;
							continue;
						}
						fprintf(new_film_list, "%s", log);
						k++;
					}
					fclose(film_list);
					fclose(new_film_list);
					k = 0;

					char login[22], way_old[50], way_new[50];
					FILE *old_users_list = fopen("users.txt", "r");
					FILE *old_fav, *new_fav;
					FILE *new_users_list = fopen("new_users.txt", "w");
					char rm_command[65];
					char mv_command[65];
					int num_users, q, l = 0;
					while(k < file_lines("users.txt")){
						fscanf(old_users_list, "%s", login);
						fprintf(new_users_list, "%s\n", login);
						strcpy(way_old, "");
						strcpy(way_new, "");						
						strcat(way_old, "favorites/favorites_");
						strcat(way_new, "favorites/new_favorites_");
						strcat(way_old, login);
						strcat(way_new, login);
						old_fav = fopen(way_old, "r");
						new_fav = fopen(way_new, "w");
						while(m < file_lines(way_old)){
							fgets(log, 100, old_fav);
							if(strcmp(log, name) == 0){
								for(int i=0; i<4; i++){
									 l = 1;
									fgets(log, 100, old_fav);
								}
								m += 5;
								continue;
							}
							fprintf(new_fav, "%s", log);
							m++;
						}
						fclose(old_fav);
						fclose(new_fav);
						strcpy(rm_command, "");
						strcpy(mv_command, "");
						strcat(rm_command, "rm ");
						strcat(mv_command, "mv ");
						strcat(rm_command, way_old);
						strcat(mv_command, way_new);
						strcat(mv_command, " ");
						strcat(mv_command, way_old);
						system(rm_command);
						system(mv_command);
						m = 0;
						for(int i=0; i<2; i++){
							fscanf(old_users_list, "%s", login);
							fprintf(new_users_list, "%s\n", login);
						}
						fscanf(old_users_list, "%d", &q);
						if(l)
							q--;
						fprintf(new_users_list, "%d\n", f);
						fscanf(old_users_list, "%s", login);
						fprintf(new_users_list, "%s\n", login);

						k += 5;
						l = 0;
						q = 0;
					}
					fclose(old_users_list);
					fclose(new_users_list);
					system("rm films.txt");
					system("mv new_film_list.txt films.txt");
					system("rm users.txt");
					system("mv new_users.txt users.txt");
					system("clear");
					printf("'%s' deleted!\n");
					film = film->next;
					del_el(film->prev);
					print(film);
				}
				else if(n == '2')
					print(film);
				else{
					print(film);
				}

				break;
			case'e':
				f = 0;
				break;
		}
	}
}


int cheking_password(User *new_user){
	int ts=0,bukV=0,bukN=0;
	for(int i=0;i<strlen(new_user->password);i++){
		if(strlen(new_user->password)>=6&&strlen(new_user->password)<=20){
			if(('0' <= new_user->password[i]) && (new_user->password[i] <= '9')){
				ts++;
			}
			if(('A' <= new_user->password[i]) && (new_user->password[i] <= 'Z')){
				bukV++;
			}
			if(('a' <= new_user->password[i]) && (new_user->password[i] <= 'z')){
				bukN++;
			}
		}
	}
	if((ts > 0) && (bukV > 0) && (bukN > 0))
		return 1;
	else{
		printf("Wrong format!(The minimum length of the password must be 6 characters(maximum 20),\nand must contain at least 1 number, an uppercase letter and a lower letter!)\n");
		return 0;
	}

}

int cheking_login(User* new_user){
	int ts=0,bukV=0,bukN=0;
	for(int i=0;i<strlen(new_user->login);i++){
	    if(strlen(new_user->login)>=3&&strlen(new_user->login)<=20){
			if(('0' <= new_user->login[i]) && (new_user->login[i] <= '9')){
				ts++;
			}
			if(('A' <= new_user->login[i]) && (new_user->login[i] <= 'Z')){
				bukV++;
			}
			if(('a' <= new_user->login[i]) && (new_user->login[i] <= 'z')){
				bukN++;
			}
		}
    }
	if((ts > 0) && ((bukV > 0) || (bukN > 0)))
		return 1;
    else{
      printf("Wrong format!(The minimum length of the login must be 3(maximum 20),\nand must contain at least 1 number!)\n");
      return 0;
    }
}

int cheking_card_num(User* new_user){
	int ts=0;
	for(int i=0;i<strlen(new_user->card_num);i++){
		if(('0' <= new_user->card_num[i]) && (new_user->card_num[i] <= '9')){
			ts++;
		}
	}
	if(ts==16)
		return 1;
	else{
		printf("Wrong format!(The card number must contain exactly 16 digits!)\n");
		return 0;
	}
}

void out_str_center(char* string, int length_str){
   int m=(length_str-strlen(string))/2;
   for(int i=0;i<m;i++){
       printf(" ");
   }
   printf("%s",string);
   for(int i=0;i<m;i++){
       printf(" ");
    }
}
void print_films(List* list){
    printf("                              Меню\n");
    // printf("                                                                                            %c\n",(char)24);
    printf("                                    ");                    printf("\t\t ====================================\n");
    printf("                                    ");                    printf("\t\t//                                  \\\\");printf("\n");
    printf("                                    ");                    printf("\t\t||                                  ||\n");
    printf("//---------------------------------\\\\");                   printf("\t\t||                                  ||");                    printf("\t\t//---------------------------------\\\\");printf("\n");
    printf("|                                   |");                   printf("\t\t||                                  ||");                    printf("\t\t|                                   |\n");
    printf("|                                   |");                   printf("\t\t||                                  ||");                    printf("\t\t|                                   |\n");
    printf("|                                   |");                   printf("\t\t||                                  ||");                    printf("\t\t|                                   |\n");
    printf("|                                   |");                   printf("\t\t||                                  ||");                    printf("\t\t|                                   |\n");
    printf("|                                   |");                   printf("\t\t||                                  ||");                    printf("\t\t|                                   |\n");
    printf("|                                   |");                   printf("\t\t||                                  ||");                    printf("\t\t|                                   |\n");
    printf("|");out_str_center(list->prev->films.name,35);printf("|");                   printf("\t\t||");out_str_center(list->films.name,40);printf("\t\t||");                    printf("|");out_str_center(list->next->films.name,35);printf("|\n");                    
    printf("|                                   |");                   printf("\t\t||                                  ||");                    printf("\t\t|                                   |\n");
    printf("|                                   |");                   printf("\t\t||                                  ||");                    printf("\t\t|                                   |\n");
    printf("|                                   |");                   printf("\t\t||                                  ||");                    printf("\t\t|                                   |\n");
    printf("|                                   |");                   printf("\t\t||                                  ||");                    printf("\t\t|                                   |\n");
    printf("|                                   |");                   printf("\t\t||                                  ||");                    printf("\t\t|                                   |\n");
    printf("|                                   |");                   printf("\t\t||                                  ||");                    printf("\t\t|                                   |\n");
    printf("\\\\_________________________________//");                   printf("\t\t||                                  ||");                    printf("\t\t\\\\_________________________________//\n");
    printf("                                     ");                   printf("\t\t\\\\                                  //\n");
    printf("                                     ");                   printf("\t\t ==================================== \n");
    printf("                                  Добавить фильм в избранное - Q\n");
    printf("                                  Переход между фильмами на клавиши A и D\n");
    printf("                                  Подробная информацию о фильме - E");

    // char F;
    // if(scanf("%c",&F)=='E'){
    //     system("clear");
    //     printf("                                    ");                    printf(" =========================================\n");
    //     printf("                                    ");                    printf("//                                       \\\\");printf("\n");
    //     printf("                                    ");                    printf("||                                       ||\n");
    //     printf("//---------------------------------\\\\");                   printf("||                                       ||");                    printf("//---------------------------------\\\\");printf("\n");
    //     printf("|                                   |");                   printf("||                                       ||");                    printf("|                                   |\n");
    //     printf("|                                   |");                   printf("||                                       ||");                    printf("|                                   |\n");
    //     printf("|                                   |");                   printf("||                                       ||");                    printf("|                                   |\n");
    //     printf("|                                   |");                   printf("||");printf("||");out_str_center(list->films.name,40);printf("||");                   printf("|                                   |\n");
    //     printf("|                                   |");                   printf("||");printf("||");out_str_center(list->films.genre,40);printf("||");                   printf("|                                   |\n");
    //     printf("|                                   |");                   printf("||");out_str_center(list->films.country,40);printf("||");                   printf("|                                   |\n");
    //     printf("|");out_str_center(list->prev->films.name,35);printf("|");                   printf("||");printf("||");out_str_center(list->films.grade,40);printf("||");                   printf("|");out_str_center(list->next->films.name,35);printf("|\n");                                  
    //     printf("|                                   |");                   printf("||");printf("||");out_str_center(list->films.year,40);printf("||");                   printf("|                                   |\n");
    //     printf("|                                   |");                   printf("||                                       ||");                    printf("|                                   |\n");
    //     printf("|                                   |");                   printf("||                                       ||");                    printf("|                                   |\n");
    //     printf("|                                   |");                   printf("||                                       ||");                    printf("|                                   |\n");
    //     printf("|                                   |");                   printf("||                                       ||");                    printf("|                                   |\n");
    //     printf("|                                   |");                   printf("||                                       ||");                    printf("|                                   |\n");
    //     printf("\\\\_________________________________//");                   printf("||                                       ||");                    printf("\\\\_________________________________//\n");
    //     printf("                                     ");                   printf("\\\\                                       //\n");
    //     printf("                                     ");                   printf(" ========================================= \n");
    // }
}

int is_in_fav_list(List *film, List *fav_list){
	List *p = fav_list;
	do{
		if(strcmp(film->films.name, p->films.name) == 0)
			return 1;
		p = p->next;
	}while(p != fav_list);

	return 0;
}

int is_film_in_list(List *film, char name[100]){
	List *p = film;
	do{
		if(strcmp(p->films.name, name) == 0)
			return 1;
		p = p->next;
	}while(p != film);

	return 0;
}

int check_grade(char grade[5]){
	if('0' > grade[0] || grade[0] > '9')
		return 1;
	if(grade[1] != '.')
		return 1;
	if('0' > grade[2] || grade[2] > '9')
		return 1;

	return 0;

}

int unique_login(User *user){
	FILE *fp = fopen("users.txt", "r");
	char log[22];
	while(!feof(fp)){
		fscanf(fp, "%s", log);
		if(strcmp(user->login, log) == 0){
			fclose(fp);
			printf("This login is already taken!\n");
			sleep(1);

			return 1;
		}
	}
	fclose(fp);
	return 0;
}