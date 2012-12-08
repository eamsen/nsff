#include"g.h"
int main(){
  vs in(1);
  getline(ci,in[0]);
  S st;
  ss(in[0])>>st.n_;
  u t=st.n_*st.n_+3;
  in.resize(t+1);
  while(t){
    getline(ci,in[in.size()-t--]);
  }
  ss(in[1])>>st.t_;
  ss(in[2])>>st.fc_;
  ss(in[3])>>st.x_>>st.y_;
  for_each(in.begin()+4,in.end(),[&st](s& e){
    u c,t;
    ss(e)>>c>>t;
    st.b_.push_back(++t<<4|c);
  });
  S f=Se(st);
  co<<f.AStr();
}
