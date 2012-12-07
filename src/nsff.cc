#include"g.h"
int main(){
  vs in(1);
  getline(ci,in[0]);
  S state;
  ss(in[0])>>state.n;
  u t=state.n*state.n+3;
  in.resize(t+1);
  while(t){
    getline(ci,in[in.size()-t--]);
  }
  ss(in[1])>>state.t;
  ss(in[2])>>state.fc;
  ss(in[3])>>state.x>>state.y;
  for_each(in.begin()+4,in.end(),[&state](s& e){
    u c,t;
    ss(e)>>c>>t;
    state.b.push_back(++t<<4|c);
  });
  co<<state.Str()<<endl;

}
