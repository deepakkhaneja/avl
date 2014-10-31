/* DEEPAK KHANEJA 
   10CS10011 
*/
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# define LEFT -1
# define RIGHT 1
# define VOWEL -2
# define CONSONANT 2

typedef struct _treenode {
  char *word;
  struct _treenode *left;
  struct _treenode *right;
  struct _treenode *parent;
  int balancefactor;
} AVLtreenode;

typedef AVLtreenode *AVLtree;

typedef AVLtree *dictionary;

//functions
dictionary init();
int h(int len);
int mystrcmp(const char* s1, const char* s2);
int search(dictionary D, char *w);
void clockrot(AVLtreenode *u, AVLtreenode *v, AVLtreenode *p, int side, dictionary D, int len);
void anticlockrot(AVLtreenode *u, AVLtreenode *v, AVLtreenode *p, int side, dictionary D, int len);
dictionary insert(dictionary D, char *w);
dictionary delete(dictionary D, char *w);
void printTree(AVLtreenode *root);
void printDictionary(dictionary D);
int isBST(AVLtree T, char *minStr, char *maxStr);
void freetree(AVLtree T);
int height(AVLtree T);

dictionary init() {
  dictionary D = (AVLtree *)malloc(7 * sizeof(AVLtree));
  int i = 0;
  for(i = 0; i < 7; i++)
    D[i] = NULL;
  return D;
}		

int h(int len) {
  return len - 2;
}

int mystrcmp(const char* s1, const char* s2) {
	int i = 0;
	while(s1[i] != '\0' && s2[i] != '\0') {
		if(s1[i] != s2[i]) return (int)s1[i]-(int)s2[i];
		i++;
	}
	if(s1[i] == s2[i]) return 0;
	else if(s1[i] == '\0') return -1;
	else return 1;
}

//search word *w in dictionary D  
int search(dictionary D, char *w) {
  int len, cond;
  AVLtree T;
  AVLtreenode *u;
  
  len = strlen(w);
  T = D[h(len)];
  u = T;
  
  while(u != NULL) {
    cond = mystrcmp(w, u->word);
    if(cond == 0) return 1;
    if(cond > 0) u = u->right; else u = u->left;
  }
  return 0;
}

//clockwise rotation : v left child of u, p is parent of u
void clockrot(AVLtreenode *u, AVLtreenode *v, AVLtreenode *p, int side, dictionary D, int len) {
  v->parent = p;
  if(p != NULL) if(side == LEFT) p->left = v; else p->right = v;
  else D[h(len)] = v;

  if(v->right != NULL) v->right->parent = u;
  u->left = v->right;
  
  u->parent = v;
  v->right = u;
}

//anticlockwise rotation : v right child of u, u is the left/right(side) child of p 	
void anticlockrot(AVLtreenode *u, AVLtreenode *v, AVLtreenode *p, int side, dictionary D, int len) {
  v->parent = p;
  if(p != NULL) if(side == LEFT) p->left = v; else p->right = v;
  else D[h(len)] = v;
  
  if(v->left != NULL) v->left->parent = u;
  u->right = v->left;
					
  u->parent = v;
  v->left = u;
}

//inserts str word in the dictionary D
dictionary insert(dictionary D, char *str) {
  int len, cond, side, i;
  char *w;
  AVLtree T;
  AVLtreenode *p, *u, *v, *t;
  
  len =  strlen(str);
  cond = 0;
  side = 0;
  w = (char *)malloc(10 * sizeof(char));
  if(w == NULL) {
    printf("\nNot enough dynamic memory");
    return D;
  }	
  strcpy(w, str);
  T = D[h(len)];
  p = NULL; 
  u = T;
  
  while(u != NULL) {
    cond = mystrcmp(w, u->word);
    if(cond == 0) return D;
    p = u;
    if(cond > 0) u = u->right; else u = u->left;
  }
  
  u = (AVLtreenode *)malloc(sizeof(AVLtreenode));
  if(u == NULL) {
    printf("\nNot enough dynamic memory");
    return D;
  }	
  u->word = w;
  u->balancefactor = 0;
  u->left = u->right = NULL;
  u->parent = p;
  
  if(p == NULL) {
    D[h(len)] = u; //insertion in empty tree and root's parent is NULL
    return D;
  }	
  
  if(cond > 0) {
    p->right = u;
    side = RIGHT;
  } else {	 
    p->left = u;
    side = LEFT;
  }		
  
  u = p;
  //height balancing : in each iteration balancing is done for the node u	
  while(u != NULL) {	
    //if side = LEFT, left subtree of u has grown in height else vice versa 
    if(side == LEFT) u->balancefactor--; else u->balancefactor++;
    
    if(u->balancefactor == 0) break;
    
    p = u->parent;
    if(p != NULL) if(mystrcmp(u->word, p->word) > 0) side = RIGHT; else side = LEFT; 

    if(u->balancefactor == -2) {
      v = u->left;
      
      if(v->balancefactor == -1) { //do one rotation
				clockrot(u, v, p, side, D, len);					
				u->balancefactor = v->balancefactor = 0;					
	
      } else if(v->balancefactor == 1) { //do two rotations
				t = v->right;
				anticlockrot(v, t, u, LEFT, D, len);
				clockrot(u, t, p, side, D, len);

				if(t->balancefactor == -1) {
	  			u->balancefactor = 1;
	  			v->balancefactor = 0;
				} else if(t->balancefactor == 1) {	
				  u->balancefactor = 0;
				  v->balancefactor = -1;
				} else if(t->balancefactor == 0) {
				  u->balancefactor = v->balancefactor = 0;											  
				}	
				t->balancefactor = 0;       		
      }
      break;  

    } else if(u->balancefactor == 2) { 
      v = u->right;

      if(v->balancefactor == 1) { //do one rotation
				anticlockrot(u, v, p, side, D, len);	
				u->balancefactor = v->balancefactor = 0;
					
      }	else if(v->balancefactor == -1) { //do two rotations
				t = v->left;
				clockrot(v, t, u, RIGHT, D, len);
				anticlockrot(u, t, p, side, D, len);
	
				if(t->balancefactor == 1) {
	  			u->balancefactor = -1;
	  			v->balancefactor = 0;
				} else if(t->balancefactor == -1) {	
	  			u->balancefactor = 0;
	  			v->balancefactor = 1;
				} else if(t->balancefactor == 0) {
	  			u->balancefactor = v->balancefactor = 0;										
				}			
				t->balancefactor = 0;
      }
      break;

    }
 
  	u = p;
  }		
  //height balancing ends		
  return D;		
}

//deletes str word in the dictionary D
dictionary delete(dictionary D, char *str) {
  int len, cond, side, i;
  AVLtree T;
  AVLtreenode *p, *u, *v, *t;
  
  len =  strlen(str);
  cond = 0;
  side = 0;
  T = D[h(len)];
  p = NULL; 
  u = T;
  
  while(u != NULL) {
    cond = mystrcmp(str, u->word);
    if(cond == 0) break;
    p = u;
    if(cond > 0) u = u->right; else u = u->left;
  }
  
	//printf("checking if u == NULL...\n");
	if(u == NULL) return D;
	//printf("u != NULL\n");
  
  if(p != NULL) { //deletion in a single node tree
	  if(u == p->right) side = RIGHT;
	  else if(u == p->left) side = LEFT;
		else { printf("\nError in delete(D, str): p is not a parent of u\n"); return D;  }
  }	
  
	if(u->right == NULL || u->left == NULL) {//zero or one children
		if(u->right == NULL) v = u->left;
		else v = u->right;
		free(u->word);
		free(u);
		if(p == NULL) {
			D[h(len)] = v;
			if(v != NULL) v->parent = NULL;
		} else {
			if(side == RIGHT) p->right = v;
			else p->left = v;
			if(v != NULL) v->parent = p;
		}
	} else { // u->right != NULL && u->left != NULL //two children
		v = u;
		if(rand()%2 == 0) {	//inorder predecessor
			t = v->left;
			side = LEFT;
			while(t->right != NULL) {
				v = t;
				t = t->right;
				side = RIGHT;
			}
			free(u->word);
			u->word = t->word;
			p = v;
			v = t->left;
		} else { //inorder successor
			t = v->right;
			side = RIGHT;
			while(t->left != NULL) {
				v = t;
				t = t->left;
				side = LEFT;
			}
			free(u->word);
			u->word = t->word;
			p = v;
			v = t->right;
		}
		free(t);
		if(side == RIGHT) p->right = v;
		else p->left = v;
		if(v != NULL) v->parent = p;
	}

  u = p;
  //height balancing : in each iteration balancing is done for the node u	
  while(u != NULL) {	
    //if side = LEFT, left subtree of u has shrunk in height else vice versa 
    if(side == LEFT) u->balancefactor++; else u->balancefactor--;
    
    if(u->balancefactor == 1 || u->balancefactor == -1) break;
    
    p = u->parent;
    if(p != NULL) if(mystrcmp(u->word, p->word) > 0) side = RIGHT; else side = LEFT; 

    if(u->balancefactor == -2) {
      v = u->left;
      
      if(v->balancefactor == 0) { //do one rotation
				clockrot(u, v, p, side, D, len);					
				u->balancefactor = -1;
				v->balancefactor = 1;	
				break;				
	
      } else if(v->balancefactor == -1) { //do one rotation
				clockrot(u, v, p, side, D, len);					
				u->balancefactor = v->balancefactor = 0;	
	
      } else if(v->balancefactor == 1) { //do two rotations
				t = v->right;
				anticlockrot(v, t, u, LEFT, D, len);
				clockrot(u, t, p, side, D, len);

				if(t->balancefactor == -1) {
	  			u->balancefactor = 1;
	  			v->balancefactor = 0;
				} else if(t->balancefactor == 1) {	
				  u->balancefactor = 0;
				  v->balancefactor = -1;
				} else if(t->balancefactor == 0) {
				  u->balancefactor = v->balancefactor = 0;											  
				}	
				t->balancefactor = 0;       		
      }

    } else if(u->balancefactor == 2) { 
      v = u->right;

      if(v->balancefactor == 0) { //do one rotation
				anticlockrot(u, v, p, side, D, len);	
				u->balancefactor = 1;
				v->balancefactor = -1;
				break;
					
      } else if(v->balancefactor == 1) { //do one rotation
				anticlockrot(u, v, p, side, D, len);	
				u->balancefactor = v->balancefactor = 0;
					
      }	else if(v->balancefactor == -1) { //do two rotations
				t = v->left;
				clockrot(v, t, u, RIGHT, D, len);
				anticlockrot(u, t, p, side, D, len);
	
				if(t->balancefactor == 1) {
	  			u->balancefactor = -1;
	  			v->balancefactor = 0;
				} else if(t->balancefactor == -1) {	
	  			u->balancefactor = 0;
	  			v->balancefactor = 1;
				} else if(t->balancefactor == 0) {
	  			u->balancefactor = v->balancefactor = 0;										
				}			
				t->balancefactor = 0;
      }

    }
 
  	u = p;
  }		
  //height balancing ends		
  return D;		
}

void printTree(AVLtreenode *root) {
	if(root == NULL) { printf("printTree:NULL\n"); return; }
	printf("%s--%s(%d)--%s, %s\n", (root->parent)?root->parent->word:"O", root->word, root->balancefactor, (root->left)?root->left->word:"O", (root->right)?root->right->word:"O");
	if(root->left) printTree(root->left);
	if(root->right) printTree(root->right);
}

void printDictionary(dictionary D) {
	printf("\n");
	int i;
	for(i = 5; i < 7; i++) {
		printf("\nLength %d:\n", i+2);
		printTree(D[i]);
	}
}

int isBST(AVLtree T, char *minStr, char *maxStr) {
	AVLtreenode *u = T;
	if(u == NULL) return 1;
	if(mystrcmp(u->word, minStr) <= 0 || mystrcmp(u->word, maxStr) >= 0)
		return 0;
	return isBST(u->left, minStr, u->word) && isBST(u->right, u->word, maxStr);
}

//find height of an AVLtree  
int height(AVLtree T) {
  if(T == NULL) return -1;
  if(T->balancefactor == 1) return 1 + height(T->right);
  else return 1 + height(T->left);
} 

//assuming T != NULL, free the binary tree T
void freetree(AVLtree T) {
  //free the left and right subtrees
  if(T->left != NULL)  freetree(T->left);
  if(T->right != NULL) freetree(T->right);
  //free the root node : first its associated word and then the root node itself
  free(T->word);
  free(T);
}

int main() {
  srand((unsigned int)time(NULL));

  dictionary D = init();
  char str[10], suggest[10], tmp;
  FILE *ifp = fopen("wordlist.txt", "r");
  int i, j, k, H[7]; //H : height array
  int len, r;  //length of the random word 
  char letter; //random letter choosen 
  int P[26] = {  9, 2, 2, 6, 11, 4, 3, 2, 9, 1, 1, 4, 2, 4, 8, 2, 1, 6, 5, 6, 4, 2, 2, 1, 2, 1 }; //probability distribution of letters
  int type1 = 0, type2 = 0; //VOWEL or, CONSONANT

  //read all words and insert
  while(!feof(ifp)) {
    fscanf(ifp, "%s", str);		
    D = insert(D, str);
    if(fscanf(ifp, "%c", &tmp) == EOF) break;
  }	
  fclose(ifp);

  //computing the heights of all 7 trees
  for(i = 0; i < 7; i++) {
    H[i] = height(D[i]);
    printf("\nHeight of Tree of word length %d = %d",i + 2,H[i]);
  }

  //making partial sums of probabilities
  for(i = 1; i < 26; i++) P[i] += P[i - 1];
  //generating 10 random words as per probabilities and two VOWELs or, CONSONANTs occur together less frequently
  for(i = 0; i < 10; i++) {    
    len = 2 + rand()%7; //length of random word
    j = 0;
    while(j < len) {
      r = 1 + rand()%100;
      for(k = 0; k < 26; k++)  if(r <= P[k]) { letter = 'A' + k; break; }
      
      if(letter == 'A' || letter == 'E' || letter == 'I' || letter == 'O' || letter == 'U') type1 = VOWEL; else type1 = CONSONANT;
      if(type1 == type2 && rand()%100 < 90) continue; //two VOWELs or, CONSONANTs occur together less frequently
      type2 = type1;
      
      str[j] = letter; j++;
    }
    str[len] = '\0';
    
    //searching the word in the dictionary
    printf("\n\nSearching for the word %s : ", str);
    if(search(D, str)) { printf("found"); continue; }
    printf("not found");

    //generating spell checking suggestions for random word
    printf("\nSpell checking suggestions : ");
    printf("\nGroup 1 : ");
    suggest[len - 1] = '\0';
    for(j = 0; j < len; j++) {
      if(j < len - 1) if(str[j] == str[j + 1]) continue;
      for(k = 0; k < j; k++) suggest[k] = str[k];
      for(k = j + 1; k < len; k++) suggest[k - 1] = str[k];
      if(search(D,suggest)) 
	printf("%s ",suggest);
    }

    printf("\nGroup 2 : ");
    suggest[len + 1] = '\0';
    if(len <= 7) {
      for(j = 0; j < len + 1; j++) {
	for(k = 0; k < j; k++) suggest[k] = str[k];
	for(k = j; k < len; k++) suggest[k + 1] = str[k];
	for(k = 0; k < 26; k++) {
	  suggest[j] = 'A' + k;
	  if(j < len) if(suggest[j] == suggest[j + 1]) continue;
	  if(search(D, suggest)) 
	    printf("%s ", suggest);
	}
      }
    }
    
    printf("\nGroup 3 : ");
    strcpy(suggest, str);
    for(j = 0; j < len; j++) {
      for(k = 0; k < 26; k++) {
	suggest[j] = 'A' + k;
	if(suggest[j] == str[j]) continue;
	if(search(D, suggest)) 
	  printf("%s ", suggest);
      }
      suggest[j] = str[j];
    }

    printf("\nGroup 4 : ");
    for(j = 0; j < len - 1; j++)
      for(k = j + 1; k < len; k++) {
	if(suggest[k] == suggest[j]) continue;
	tmp = suggest[k]; suggest[k] = suggest[j]; suggest[j] = tmp;
	if(search(D, suggest)) 
	  printf("%s ", suggest);
	tmp = suggest[k]; suggest[k] = suggest[j]; suggest[j] = tmp;
      }          
  }

	//Determine whether our Dictionary AVLtrees are BSTs
  printf("\n\nDetermining whether our Dictionary AVLtrees are BSTs...\n");		
  for(i = 0; i < 7; i++) 
    printf("isBST(D[%d], \"A\", \"ZZZZZZZZZ\"): %d\n", i, isBST(D[i], "A", "ZZZZZZZZZ"));

  //open file, read half of all words and delete
	//printDictionary(D);
  ifp = fopen("wordlist.txt", "r");
	i = 0;
  printf("\n\nDeleting half of all words...\n");		
  while(!feof(ifp) && i < 507700) {
    fscanf(ifp, "%s", str);
		//printf("Deleting %s...\n", str);		
    D = delete(D, str);
		//printDictionary(D);
		i++;
    if(fscanf(ifp, "%c", &tmp) == EOF) break;
  }	
	printf("i = %d\n", i);
	
  //computing the heights of all 7 trees
  for(i = 0; i < 7; i++) {
    H[i] = height(D[i]);
    printf("\nHeight of Tree of word length %d = %d",i + 2,H[i]);
  }

	//Determine whether our Dictionary AVLtress are BSTs
  printf("\n\nDetermining whether our Dictionary AVLtress are BSTs...\n");		
  for(i = 0; i < 7; i++) 
    printf("isBST(D[%d], \"A\", \"ZZZZZZZZZ\"): %d\n", i, isBST(D[i], "A", "ZZZZZZZZZ"));

  //read rest of the words and delete
  printf("\n\nDeleting rest of the words...\n");		
  while(!feof(ifp)) {
    fscanf(ifp, "%s", str);		
    D = delete(D, str);
    if(fscanf(ifp, "%c", &tmp) == EOF) break;
  }	
  fclose(ifp);

  //computing the heights of all 7 trees
  for(i = 0; i < 7; i++) {
    H[i] = height(D[i]);
    printf("\nHeight of Tree of word length %d = %d",i + 2,H[i]);
  }

  //Destroy the dictionary
  printf("\n\nDestroying the dictionary...\n");		
  for(i = 0; i < 7; i++) 
    if(D[i] != NULL) freetree(D[i]);
  free(D);

  return 0;
}
