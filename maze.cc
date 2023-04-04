//Nicolás Van Olmen Giménez, 74383864R
#include <fstream>
#include <string>
#include <limits>
#include <iostream>
#include <vector>
using namespace std;

const int SENTINEL = -1;
const int KMAXINT = std::numeric_limits<int>::max() - 10000000;//para que no de overflow al sumar 1

void show_usage(){
  cerr << "Usage:" << endl << "maze [-p] [-t] [--ignore-naive] -f file" << endl;
}

int maze_naive(const vector<vector<int>> &maze, int posx, int posy){
  if(maze[posy][posx] == 0) return KMAXINT;
  if(posx == 0 && posy == 0) return 1;
  else{
    int S1, S2, S3;
    S1 = S2 = S3 = KMAXINT;
    if(posy > 0){
      S1 = 1 + maze_naive(maze,posx,posy-1);
    }
    if(posx > 0){
      S2 = 1 + maze_naive(maze,posx-1,posy);
    }
    if(posx > 0 && posy > 0){
      S3 = 1 + maze_naive(maze,posx-1,posy-1);
    }

    return std::min(std::min(S1,S2),S3);
  }
}

int maze_memo(const vector<vector<int>> &maze, int posx, int posy, vector<vector<int>> &memo_table){
  if(memo_table[posy][posx] != SENTINEL) return memo_table[posy][posx];//ya estaba calculado
  if(maze[posy][posx] == 0) return memo_table[posy][posx] = KMAXINT;//caso base 1
  if(posx == 0 && posy == 0) return memo_table[posy][posx] = 1;//caso base 2
  else{
    int S1, S2, S3;
    S1 = S2 = S3 = KMAXINT;
    if(posy > 0){
      S1 = 1 + maze_memo(maze,posx, posy-1, memo_table);
    }
    if(posx > 0){
      S2 = 1 + maze_memo(maze,posx-1, posy, memo_table);
    }
    if(posx > 0 && posy > 0){
      S3 = 1 + maze_memo(maze,posx-1, posy-1, memo_table);
    }
    return memo_table[posy][posx] = std::min(std::min(S1,S2),S3);
  }
}

int maze_it_matrix(const vector<vector<int>> &maze, vector<vector<int>> &it_table){
  for(long unsigned int posy=0;posy<it_table.size();posy++){
    for(long unsigned int posx=0;posx<it_table[posy].size();posx++){
      if(maze[posy][posx] == 0){
        it_table[posy][posx] = KMAXINT;//caso base 1
        continue;
      }
      if(posx == 0 && posy == 0){
        it_table[posy][posx] = 1;//caso base 2
        continue;
      }
      else{
        int S1, S2, S3;
        S1 = S2 = S3 = KMAXINT;
        if(posy > 0){
        S1 = 1 + it_table[posy-1][posx];
        }
        if(posx > 0){
          S2 = 1 + it_table[posy][posx-1]; 
        }
        if(posx > 0 && posy > 0){
          S3 = 1 + it_table[posy-1][posx-1];
        }
        it_table[posy][posx] = std::min(std::min(S1,S2),S3);
      }
    }
  }
  return it_table[it_table.size()-1][it_table[0].size()-1];//el valor de la ultima casilla
}

int maze_it_vector(){
  return -1;
}

void maze_parser(vector<vector<int>> &path_solu, const vector<vector<int>> &memo_table){

}

void print_matrix(vector<vector<int>> matrix, int rows, int cols, bool blanks){
  for(int i=0;i<rows;i++){
    for(int j=0;j<cols;j++){
      if(matrix[i][j] >= KMAXINT) cout << "X";
      else if(matrix[i][j] == -1) cout << "-";
      else{
        cout << matrix[i][j];
      }
      if(blanks) cout << " ";
    }
    cout << endl;
  }
}

int main(int argc, char *argv[]){
  //comprobacion de argumentos
  bool path = false;
  bool tables = false;
  bool ignore_naive = false;
  bool file = false;
  string filename = "";

  if(argc == 1){
    show_usage();
    return -1;
  }

  for(int i=1;i<argc;i++){
    string arg = argv[i];

    if(arg == "-p"){
      path = true;
    }
    else if(arg == "-t"){
      tables = true;
    }
    else if(arg == "--ignore-naive"){
      ignore_naive = true;
    }
    else if(arg == "-f"){
      if(!file){
        i++;
        if( i>= argc ){
          cerr << "ERROR: missing filename." << endl;
          show_usage();
          return -1;
        }
        filename = argv[i];
        file = true;
      }
    }
    else{
      cerr << "ERROR: unknown option " << argv[i] << "." << endl;
      show_usage();
      return -1;
    }
  }

  //leo el fichero
  ifstream is(filename);
  if(!is){
    cerr << "ERROR: can't open file: " << filename << "."  << endl;
    show_usage();
    return -1;
  }

  int rows , cols;
  is >> rows >> cols ;
  vector<vector<int>> maze(rows, vector<int>(cols));
  for(int i=0;i<rows;i++)
    for(int j=0;j<cols;j++)
      is >> maze[i][j];

  //ejecucion algoritmos
  int naive = -1;
  vector<vector<int>> path_solu(maze);
  vector<vector<int>> memo_table(rows, vector<int>(cols));
  for(int i=0;i<rows;i++)
    for(int j=0;j<cols;j++)
      memo_table[i][j] = SENTINEL;

  vector<vector<int>> it_table(memo_table);
  int maxPathValue = rows*cols;

  if(!ignore_naive){
    naive = maze_naive(maze, cols-1, rows-1);
    if(naive > maxPathValue) naive = 0;
  }
  int memo = maze_memo(maze, cols-1, rows-1, memo_table);
  if(memo > maxPathValue) memo = 0;

  int it_m = maze_it_matrix(maze, it_table);
  if(it_m > maxPathValue) it_m = 0;

  int it_v = maze_it_vector();
  if(it_v > maxPathValue) it_v = 0;

  if(path){
    if(memo != 0) maze_parser(path_solu, memo_table);
  }

  //salida
  if(!ignore_naive){
    if(naive>=0) cout << naive << " "; 
    else cout << "? ";
  }
  else cout << "- ";

  if(memo>=0) cout << memo << " ";
  else cout << "? ";
  
  if(it_m>=0) cout << it_m << " ";
  else cout << "? ";

  if(it_v>=0) cout << it_v << " ";
  else cout << "? ";

  cout << endl;

  if(path){
    //if(memo==0) cout << "NO EXIT" << endl;
    //else
      //print_matrix(path_solu,rows,cols,false);
    cout << "?" << endl;
  }

  if(tables){
    cout << "Memoization table:" << endl;
    print_matrix(memo_table,rows,cols,true);

    cout << "Iterative table:" << endl;
    print_matrix(it_table,rows,cols,true);
  }

  return 0;
}
