#include"g.h"
int main(){
  vs in(1);
  getline(ci,in[0]);
  S st;
  ss(in[0])>>st.n;
  u t=st.n*st.n+3;
  in.resize(t+1);
  while(t){
    getline(ci,in[in.size()-t--]);
  }
  ss(in[1])>>st.t;
  ss(in[2])>>st.fc;
  ss(in[3])>>st.x>>st.y;
  for_each(in.begin()+4,in.end(),[&st](s& e){
    u c,t;
    ss(e)>>c>>t;
    st.b.push_back(++t<<4|c);
  });
  co<<st.Str()<<endl;
  for (S& st2:Expand(st)){
    co<<st2.Str()<<st2.AStr()<<endl;
  }
  co<<endl;
}
