#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>
#include <random>
#include <set>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>

using namespace std;

bool calculate(vector<int> &, vector<vector<int>> &, vector<vector<int>> &,
               int &, int &, int &, bool &, bool &, bool &, bool &);
void newQueueStart(unordered_set<int> &, int &, int &, int, int,
                   vector<pair<vector<vector<int>>, pair<int, int>>> &,
                   vector<vector<int>> &,
                   queue<vector<pair<vector<vector<int>>, pair<int, int>>>> &,
                   set<vector<vector<int>>> &);
void initialize(vector<vector<int>>, unordered_map<int, pair<int, int>> &,
                vector<int> &, pair<int, int> &);
void diagnostics(vector<pair<vector<vector<int>>, pair<int, int>>> &);
void managerDiagnostics(vector<pair<vector<vector<int>>, pair<int, int>>> &);
void displayGrid(vector<vector<int>> &);

void randomGrid(std::vector<std::vector<int>> &grid) {
  vector<int> numbers(16);
  for (int i = 0; i < 16; ++i) {
    numbers[i] = i;
  }

  random_device rd;
  mt19937 gen(rd());
  shuffle(numbers.begin(), numbers.end(), gen);

  int k = 0;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      grid[i][j] = numbers[k++];
    }
  }
}

class Manager {
public:
  void (*action)(vector<pair<vector<vector<int>>, pair<int, int>>> &);

  void intro() { cout << "*Mic squeak*\n...\nWelcome to this game!\n\n"; }

  void executeAction(vector<pair<vector<vector<int>>, pair<int, int>>> &path) {
    action(path);
  }
};

static void keygen(char *str) {
  FILE *file = fopen("/dev/urandom", "r");
  fgets(str, 48, file);
  fclose(file);
}

int main() {
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);
  fflush(stdout);

  char managerKey[48];
  char buf[48];

  keygen(managerKey);
  Manager *manager = (Manager *)malloc(sizeof(Manager));
  manager->action = diagnostics;
  manager->intro();

  cout << "Random start instead of input? y/n: ";

  scanf("%48s[^\n]", buf, 48);

  if (strncmp(buf, managerKey, 48) == 0) {
    cout<<"Manager\n";
    manager->action = managerDiagnostics;
  }

  int n = 4;
  vector<vector<int>> grid = {
      {0, 9, 3, 5}, {10, 2, 12, 4}, {1, 11, 6, 8}, {13, 14, 7, 15}};


  if(buf[0]=='y'){
	  cout<<"RANDOM\n";
	  randomGrid(grid);
  }else{
	  cout << "Please commence! And input starting grid!\n";
	  // INPUT
	  for (int i = 0; i < 4; ++i) {
	    for (int j = 0; j < 4; ++j) {
	      cin >> grid[i][j];
	    }
	  }
  }


  set<vector<vector<int>>> s;
  pair<int, int> zeroPosition;
  vector<pair<vector<vector<int>>, pair<int, int>>> finalPath;
  queue<vector<pair<vector<vector<int>>, pair<int, int>>>> q;
  unordered_map<int, pair<int, int>> m;
  unordered_set<int> fixed;
  vector<int> order;

  // Initializing maps, vectors and positions.
  initialize(grid, m, order, zeroPosition);

  bool three, seven, nine, ten;
  three = seven = nine = ten = false;
  int aimNum = 0;
  q.push({{grid, zeroPosition}});

  while (!q.empty()) {
    vector<pair<vector<vector<int>>, pair<int, int>>> fullPath = q.front();
    vector<vector<int>> ft = fullPath[fullPath.size() - 1].first;
    pair<int, int> zeroPos = fullPath[fullPath.size() - 1].second;
    q.pop();
    if (find(s.begin(), s.end(), ft) != s.end()) {
      continue;
    }
    s.insert(ft);

    if (aimNum >= 14) {
      finalPath = fullPath;
      break;
    }

    int r = zeroPos.first;
    int c = zeroPos.second;
    int elem = ft[r][c];
    //____________________________________________
    if (three) {
      if (ft[0][3] == 0) {
        ft[0][2] = 0;
        ft[0][3] = 4;
        s.insert(ft);
        fullPath.push_back({ft, {0, 2}});
        ft[1][2] = 0;
        ft[0][2] = 3;
        three = false;
        newQueueStart(fixed, elem, aimNum, 1, 2, fullPath, ft, q, s);
        continue;
      }
    }
    if (nine) {
      if (ft[3][0] == 0) {
        ft[3][0] = 13;
        ft[2][0] = 0;
        s.insert(ft);
        fullPath.push_back({ft, {2, 0}});
        ft[2][0] = 9;
        ft[2][1] = 0;
        nine = false;
        newQueueStart(fixed, elem, aimNum, 2, 1, fullPath, ft, q, s);
        continue;
      }
    }
    if (seven) {
      if (ft[1][3] == 0) {
        ft[1][3] = 8;
        ft[1][2] = 0;
        s.insert(ft);
        fullPath.push_back({ft, {1, 2}});
        ft[1][2] = 7;
        ft[2][2] = 0;
        seven = false;
        newQueueStart(fixed, elem, aimNum, 2, 2, fullPath, ft, q, s);
        continue;
      }
    }
    if (ten) {
      if (ft[3][1] == 0) {
        ft[3][1] = 14;
        ft[2][1] = 0;
        s.insert(ft);
        fullPath.push_back({ft, {2, 1}});
        ft[2][1] = 10;
        ft[2][2] = 0;
        ten = false;
        newQueueStart(fixed, elem, aimNum, 2, 2, fullPath, ft, q, s);
        continue;
      }
    }

    //____________________________________________

    if (r - 1 >= 0 &&
        find(fixed.begin(), fixed.end(), ft[r - 1][c]) == fixed.end()) {
      vector<vector<int>> f = ft;
      f[r][c] = f[r - 1][c];
      f[r - 1][c] = 0;
      vector<pair<vector<vector<int>>, pair<int, int>>> newPath = fullPath;

      newPath.push_back({f, {r - 1, c}});
      q.push(newPath);
      int elem = f[r][c];
      pair<int, int> comp = {r, c};

      if (elem == order[aimNum] && m[elem] == comp) {
        bool res =
            calculate(order, f, ft, aimNum, r, c, three, seven, nine, ten);
        if (res)
          continue;
        newQueueStart(fixed, elem, aimNum, r - 1, c, fullPath, f, q, s);
        continue;
      }
    }
    if (r + 1 < n &&
        find(fixed.begin(), fixed.end(), ft[r + 1][c]) == fixed.end()) {
      vector<vector<int>> f = ft;
      f[r][c] = f[r + 1][c];
      f[r + 1][c] = 0;
      vector<pair<vector<vector<int>>, pair<int, int>>> newPath = fullPath;

      newPath.push_back({f, {r + 1, c}});
      q.push(newPath);
      int elem = f[r][c];
      pair<int, int> comp = {r, c};

      if (elem == order[aimNum] && m[elem] == comp) {
        bool res =
            calculate(order, f, ft, aimNum, r, c, three, seven, nine, ten);
        if (res)
          continue;
        newQueueStart(fixed, elem, aimNum, r + 1, c, fullPath, f, q, s);
        continue;
      }
    }
    if (c - 1 >= 0 &&
        find(fixed.begin(), fixed.end(), ft[r][c - 1]) == fixed.end()) {
      vector<vector<int>> f = ft;
      f[r][c] = f[r][c - 1];
      f[r][c - 1] = 0;
      vector<pair<vector<vector<int>>, pair<int, int>>> newPath = fullPath;

      newPath.push_back({f, {r, c - 1}});
      q.push(newPath);
      int elem = f[r][c];
      pair<int, int> comp = {r, c};

      if (elem == order[aimNum] && m[elem] == comp) {
        bool res =
            calculate(order, f, ft, aimNum, r, c, three, seven, nine, ten);
        if (res)
          continue;
        newQueueStart(fixed, elem, aimNum, r, c - 1, fullPath, f, q, s);
        continue;
      }
    }
    if (c + 1 < n &&
        find(fixed.begin(), fixed.end(), ft[r][c + 1]) == fixed.end()) {
      vector<vector<int>> f = ft;
      f[r][c] = f[r][c + 1];
      f[r][c + 1] = 0;
      vector<pair<vector<vector<int>>, pair<int, int>>> newPath = fullPath;

      newPath.push_back({f, {r, c + 1}});
      q.push(newPath);
      int elem = f[r][c];
      pair<int, int> comp = {r, c};

      if (elem == order[aimNum] && m[elem] == comp) {

        bool res =
            calculate(order, f, ft, aimNum, r, c, three, seven, nine, ten);
        if (res)
          continue;
        newQueueStart(fixed, elem, aimNum, r, c + 1, fullPath, f, q, s);
        continue;
      }
    }
  }

  manager->action(finalPath);
  free(manager);
  return 0;
}

void newQueueStart(unordered_set<int> &fixed, int &elem, int &aimNum,
                   int firstPos, int secondPos,
                   vector<pair<vector<vector<int>>, pair<int, int>>> &fullPath,
                   vector<vector<int>> &f,
                   queue<vector<pair<vector<vector<int>>, pair<int, int>>>> &q,
                   set<vector<vector<int>>> &s) {
  fixed.insert(elem);
  aimNum++;
  queue<vector<pair<vector<vector<int>>, pair<int, int>>>> tempq;
  vector<pair<vector<vector<int>>, pair<int, int>>> newPath = fullPath;
  newPath.push_back({f, {firstPos, secondPos}});
  tempq.push(newPath);
  q = tempq;
  s.clear();
}

bool calculate(vector<int> &order, vector<vector<int>> &f,
               vector<vector<int>> &ft, int &aimNum, int &r, int &c,
               bool &three, bool &seven, bool &nine, bool &ten) {
  // looking on right side stacking
  if (order[aimNum] == 4) {
    if (f[r][c + 1] == 3 || (f[r][c + 1] == 0 && f[r + 1][c + 1] == 3)) {
      return true;
      ;
    }
  } else if (order[aimNum] == 3) {
    if (f[0][3] == 0) {
      f[0][2] = 3;
      f[0][3] = 4;
      f[1][2] = 0;
    } else {
      three = true;
      aimNum--;
    }
  } else if (order[aimNum] == 8) {
    if (f[r][c + 1] == 7 || (f[r][c + 1] == 0 && f[r + 1][c + 1] == 7)) {
      return true;
      ;
    }
  } else if (order[aimNum] == 7) {
    if (f[1][3] == 0) {
      f[1][3] = 8;
      f[1][2] = 7;
      f[2][2] = 0;
    } else {
      seven = true;
      aimNum--;
    }
  }
  // looking on left side stacking
  else if (order[aimNum] == 13) {
    if (f[r + 1][c] == 9 || (f[r + 1][c] == 0 && f[r + 1][c + 1] == 9)) {
      return true;
    }
  } else if (order[aimNum] == 9) {
    if (ft[3][0] == 0) {
      ft[3][0] = 13;
      ft[2][0] = 9;
      ft[2][1] = 0;
    } else {
      nine = true;
      aimNum--;
    }
  } else if (order[aimNum] == 14) {
    if (f[r + 1][c] == 10 || (f[r + 1][c] == 0 && f[r + 1][c + 1] == 10)) {
      return true;
    }
  } else if (order[aimNum] == 10) {
    if (f[3][1] == 0) {
      f[3][1] = 14;
      f[2][1] = 10;
      f[2][2] = 0;
    } else {
      ten = true;
      aimNum--;
    }
  }
  return false;
}

void initialize(vector<vector<int>> grid, unordered_map<int, pair<int, int>> &m,
                vector<int> &order, pair<int, int> &zeroPosition) {
  int n = 4;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (grid[i][j] == 0) {
        zeroPosition.first = i;
        zeroPosition.second = j;
        break;
      }
    }
  }
  order = {1, 2, 4, 3, 5, 13, 9, 6, 8, 7, 14, 10, 11, 12, 15};
  m[1] = {0, 0};
  m[2] = {0, 1};
  m[3] = {1, 2};
  m[4] = {0, 2};
  m[5] = {1, 0};
  m[6] = {1, 1};
  m[7] = {2, 2};
  m[8] = {1, 2};
  m[9] = {2, 1};
  m[10] = {2, 2};
  m[11] = {2, 2};
  m[12] = {2, 3};
  m[13] = {2, 0};
  m[14] = {2, 1};
  m[15] = {3, 2};
}

pair<int, int> findZero(vector<vector<int>> grid) {
  pair<int, int> curZeroPos = {};
  for (int i = 0; i < grid.size(); ++i) {
    for (int j = 0; j < grid[0].size(); ++j) {
      if (grid[i][j] == 0) {
        curZeroPos.first = i;
        curZeroPos.second = j;
        return curZeroPos;
      }
    }
  }
  return {-1, -1};
}

void diagnostics(vector<pair<vector<vector<int>>, pair<int, int>>> &path) {
  if (path.size() == 0) {
    cout << '\n'
         << "This starting combination is not possible to solve" << '\n';
    return;
  }
  cout << '\n' << "Moves: " << path.size() << '\n';
  cout << "Thanks for playing!\n";
}

void managerDiagnostics(
    vector<pair<vector<vector<int>>, pair<int, int>>> &path) {
  if (path.size() == 0) {
    cout << '\n'
         << "This starting combination is not possible to solve" << '\n';
    return;
  }
  cout << '\n' << "Moves: " << path.size() << '\n';
  if (path.size() == 100) {
    system("/bin/bash");
  }
}

void displayGrid(vector<vector<int>> &grid) {
  for (vector<int> v : grid) {
    for (int x : v) {
      if (to_string(x).size() == 1) {
        cout << x << "  ";
      } else {
        cout << x << " ";
      }
    }
    cout << '\n';
  }
  cout << '\n';
}
