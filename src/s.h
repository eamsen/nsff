#include "g.h"
struct S{
  S(){}
  S(u n,u t,u fc,u x,u y):n(n),fc(fc),t(t),x(x),y(y){}
  // Board dimension, free card, number of targets, x and y positions.
  u n,fc,t,x,y;
  // The board fields.
  vu b;
  // Returns the field value of given index.
  u& operator[](u n){
    assert(n<b.size());
    return b[n];
  }
  // Returns the target value of given index.
  u T(u n){return b[n]>>4;}
  // Returns the card id of given index.
  u C(u n){return b[n]&0xf;}
  // Returns the string representation of the state.
  s Str(){
    ss s;
    s<<"targets: "<<t<<"\ncard: "<<fc<<"\npos: "<<x<<","<<y;
    for(u n=0;n<b.size();++n){
      s<<"\n"<<C(n)<<" "<<T(n);
    }
    return s.str();
  }
};
