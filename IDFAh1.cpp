#include <stdio.h>
#include <iostream>
#include <cmath>
#include <string>
#include <cassert>
#include <random>

using namespace std;
#define N 3
#define N2 9 // N2-1パズル
#define LIMIT 30

// 盤面の移動
static const int dx[4] = {0, -1, 0, 1};
static const int dy[4] = {1, 0, -1, 0};
static const char dir[4] = {'r', 'u', 'l', 'd'};

// 盤面
static const int answer[N2] = {1, 2, 3, 8, 0, 4, 7, 6, 5};

struct Puzzle {
  int f[N2], space;
};

Puzzle state;
int limit;
int path[LIMIT];
long state_count;

// 解の確認

bool isSolved(){
  for(int i = 0; i < N2; i++){
    if(state.f[i] != answer[i]) return false;
  }
  return true;
}

// ヒューリスティック関数h1
// 答えの場所と一致しない駒の数
int h1(Puzzle in){
  int r = 0;
  for(int i = 0; i < N2-1; i++){
    if(in.f[i] != answer[i]) r++;
  }
  return r;
}

// 深さ優先探索

bool dfs(int depth, int prev){
  // 解けてたらtrueを返す
  if(isSolved()) return true;
  // 深さとヒューリスティック関数の合計が制限を超えたら枝刈り
  if((depth + h1(state) ) > limit) return false;

  state_count++;

  int sx = state.space / N;
  int sy = state.space % N;
  Puzzle tmp;

  for(int r = 0; r < 4; r++){
    int tx = sx + dx[r];
    int ty = sy + dy[r];
    if(tx < 0 || ty < 0 || tx >= N || ty >= N) continue;
    // 前のノードと同じ状態に戻らないようにする
    if(max(prev, r) - min(prev, r) == 2) continue;
    tmp = state;
    // ピースの入れ替え
    swap(state.f[tx * N + ty], state.f[sx * N + sy]);
    state.space = tx * N + ty;
    if(dfs(depth + 1, r)) {
      path[depth] = r;
      return true;
    }
    state = tmp;
  }

  return false;
}

// 反復深化で呼び出す

string IDS(Puzzle in){
  for(limit = 5; limit < LIMIT; limit++){
    state = in;
    if(dfs(0, -100)){
      string ans = "";
      for(int i = 0; i < limit ; i++) ans += dir[path[i]];
      return ans;
    }
  }
  return "N";
}

int main(){
  Puzzle in;

  srand(time(NULL));

  bool flag = true;
  int r;
  for(int i = 0; i < N2-1; i++){
    while(true){
      flag = true;
      r = 1 + rand() % (N2 - 1);
      for(int j = 0; j < i; j++){
        if(in.f[j] == r){
          flag = false;
          break;
        }
      }
      if(flag) break;
    }
    in.f[i] = r;
  }

  state_count = 0;

  // call IDS
  string ans = IDS(in);

  cout << "***Iterative Depth First Search(heuristic funcition h_1)***" << endl;
  if(ans != "N"){
    cout << "trouble: " << ans.size() << endl;
    cout << "states: " << state_count << endl;
  }else{
    cout << "NOANSWER" << endl;
  }

  return 0;
}
