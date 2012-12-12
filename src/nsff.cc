#include"g.h"
i main(){
  vs c(1);
  gl(ci,c[0]);
  S a;
  ss(c[0])>>a.n;
  u t=a.n*a.n+3;
  c.resize(t+1);
  w(t){gl(ci,c[c.size()-t--]);}
  ss(c[1])>>a.t;
  ss(c[2])>>a.fc;
  ss(c[3])>>a.x>>a.y;
  for_each(c.begin()+4,c.end(),[&a](s& e){
    u c,t;ss(e)>>c>>t;a.b.pb(++t<<4|c);});
  co<<Se(a).B();
}
