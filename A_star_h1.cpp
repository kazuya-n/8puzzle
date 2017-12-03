#include <stdio.h>
#include <iostream>
#include <cmath>
#include <string>
#include <cassert>
#include <random>
#include <map>
#include <queue>

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
// 1~9(スペース)の駒が答えではどこにあるか
static const int answer_dist[N2] = {0, 1, 2, 5, 8, 7, 6, 3, 4};

struct Puzzle {
  int f[N2], space, MD;
  int cost;

  bool operator < (const Puzzle &p) const {
    for(int i = 0; i < N2; i++){
      if(f[i] == p.f[i]) continue;
      return f[i] < p.f[i];
    }
    return false;
  }
};

Puzzle state;
int limit;
int path[LIMIT];
long state_count;
int count;

struct State{
  Puzzle puzzle;
  int estimated;
  bool operator < (const State &s) const{
    return estimated > s.estimated;
  }
};

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

// ヒューリスティック関数h2
// 答えの駒の位置とのマンハッタン距離
int h2(Puzzle in){
  int r = 0;
  for(int i = 0; i < N2-1; i++){
    if(in.f[i] != answer[i]){
      r += abs((answer_dist[in.f[i] - 1] % 3) - i % 3) + abs((answer_dist[in.f[i] - 1] / 3) - i / 3);
    }
  }
  return r;
}

int astar(Puzzle s){
  priority_queue<State> PQ;
  s.MD = h1(s);
  s.cost = 0;
  map<Puzzle, bool> V;
  Puzzle u, v;
  State initial;
  initial.puzzle = s;
  initial.estimated = h1(s);
  PQ.push(initial);
  count = 0;

  while(!PQ.empty()){
    State st = PQ.top(); PQ.pop();
    u = st.puzzle;
    count++;

    if(u.MD == 0) return u.cost;
    V[u] = true;

    int sx = u.space / N;
    int sy = u.space % N;
    for(int r = 0; r < 4; r++){
      int tx = sx + dx[r];
      int ty = sy + dy[r];
      if(tx < 0 || ty < 0 || tx >= N || ty >= N) continue;
      v = u;
      v.MD = h1(v);
      swap(v.f[sx * N + sy], v.f[tx * N + ty]);
      v.space = tx * N + ty;

      if( !V[v] ){
        v.cost++;
        State news;
        news.puzzle = v;
        news.estimated = v.cost + v.MD;
        PQ.push(news);
      }
    }
  }
  return -1;
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

  int ans = astar(in);
  cout << ans << endl;
  cout << count << endl;

  return 0;
}
