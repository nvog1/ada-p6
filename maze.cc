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
    if(posx == 0 && posy > 0){
      return 1 + maze_naive(maze,posx,posy-1);
    }
    else if(posx > 0 && posy == 0){
      return 1 + maze_naive(maze,posx-1,posy);
    }
    else{
      return 1 + std::min(std::min(maze_naive(maze,posx-1,posy-1), maze_naive(maze,posx,posy-1)), maze_naive(maze,posx-1,posy));
    }
  }
}

int maze_memo(const vector<vector<int>> maze, int posx, int posy, vector<vector<int>> &memo_table){
  if(memo_table[posy][posx] != SENTINEL) return memo_table[posy][posx];
  if(maze[posy][posx] == 0) return memo_table[posy][pox] = KMAXINT;
  if(posx == 0 && posy == 0) return memo_table[posy][posx] = 1;
  else{
    return memo_table[posy][posx] = maze_naive(maze,posx,posy);
  }
}

int maze_it_matrix(const vector<vector<int>> &it_table){
  return -1;
}

int maze_it_vector(){
  return -1;
}

void maze_parser(vector<vector<int>> &path_solu, const vector<vector<int>> &memo_table){

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

  int it_m = maze_it_matrix(it_table);
  if(it_m > maxPathValue) it_m = 0;

  int it_v = maze_it_vector();
  if(it_v > maxPathValue) it_v = 0;

  if(path){
    maze_parser(path_solu, memo_table);
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
    /*for(int i=0;i<rows;i++){
      for(int j=0;j<cols;j++)
        cout << path_solu[i][j];
      cout << endl;
    }
    cout << endl;*/
    cout << "?" << endl;
  }

  if(tables){
    cout << "Memoization table:" << endl;
    cout << "?" << endl;
    cout << "Iterative table:" << endl;
    cout << "?" << endl;
  }

  return 0;
}
