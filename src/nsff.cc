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
  co<<st.Str()<<endl;
  // for (S& st2:Expand(st)){
  //   co<<st2.Str()<<st2.AStr()<<endl;
  // }
  //  for(uu xy:Explore(st,10)){
  //    co<<xy.first<<","<<xy.second<<" ";
  //  }
  S f=Search(st);
  co<<f.Str()<<endl;
  co<<f.AStr()<<endl;
}
