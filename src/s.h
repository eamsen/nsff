#include "g.h"
u ASCII(u c){
  return c;
  switch(c){
    case 0:return 196;
    case 1:return 179;
    case 2:return 218;
    case 3:return 191;
    case 4:return 217;
    case 5:return 192;
    case 6:return 197;
    case 7:return 193;
    case 8:return 194;
    case 9:return 180;
    case 10:return 195;
    default:assert(false);
  }
}
// Action
struct A{
  A(u t,u p,u c,u x,u y):v(t|(p<<3)),c(c),x(x),y(y){}
  // Returns the action type. 0:CD, 1:CU, 2:RR, 3:RL
  u T(){return v&3u;} 
  // Returns the position (row/column) of the action.
  i P(){return v>>3;}
  // Returns the string representation in output format.
  s Str(){
    ss s;
    s<<c<<" "<<(T()/2?"r ":"c ")<<(P()+1)*(T()%2?-1:1)<<" "<<x<<" "<<y;
    return s.str();
  }
  u v,c,x,y;
};
// State
struct S{
  S():n(0),fc(0),t(0),x(0),y(0),c(0){}
  S(u n,u t,u fc,u x,u y):n(n),fc(fc),t(t),x(x),y(y){}
  // Inserts free card in given column pushing up.
  void CU(u _x){
    a.push_back(A(1,_x,fc,x,y));
    for(u _y=0;_y<n;++_y)swap(fc,F(_x,_y));
  }
  // Inserts free card in given column pushing down.
  void CD(u _x){
    a.push_back(A(0,_x,fc,x,y));
    for(i _y=n-1;_y>=0;--_y)swap(fc,F(_x,_y));
  }
  // Inserts free card in given row pushing right.
  void RR(u _y){
    a.push_back(A(2,_y,fc,x,y));
    for(u _x=0;_x<n;++_x)swap(fc,F(_x,_y));
  }
  // Inserts free card in given row pushing left.
  void RL(u _y){
    a.push_back(A(3,_y,fc,x,y));
    for(i _x=n-1;_x>=0;--_x)swap(fc,F(_x,_y));
  }
  // Returns the field value reference for given position.
  u& F(u x,u y){return F(x+y*n);}
  // Returns the target value for given position.
  u T(u x,u y){return T(x+y*n);}
  // Returns the card id for given position.
  u C(u x,u y){return C(x+y*n);}
  // Returns the field value reference for given index.
  u& F(u n){
    assert(n<b.size());
    return b[n];
  }
  // Returns the target value of given index.
  u T(u n){return F(n)>>4;}
  // Returns the card id of given index.
  u C(u n){return F(n)&0xf;}
  // Returns the string representation of the state.
  s Str(){
    ss s;
    s<<"n: "<<n<<"\ntargets: "<<t<<"\ncard: "<<fc<<"\npos: "<<x<<","<<y
     <<"\ncost: "<<c<<"\n";
    for(i y=n-1;y>=0;--y){
      for(u x=0;x<n;++x)
        s<<ASCII(C(x,y))<<"("<<T(x,y)<<")\t";
      s<<"\n";
    }
    return s.str();
  }
  // Returns the solution string.
  s AStr(){
    ss s;
    for(A& e:a)s<<e.Str()<<"\n";
    return s.str();
  }
  // Board dimension, free card, number of targets, x and y positions, costs.
  u n,fc,t,x,y,c;
  // The board fields.
  vu b;
  // The recorded actions.
  vA a;
};
// Returns all rotated card ids for given card id.
vu Rot(u id){
  if(id<2)return{0,1};
  else if(id<6)return{2,3,4,5};
  else if(id==6)return{6};
  return{7,8,9,10};
}
// Returns all states resulting in inserting at given row and column.
vS In(S& st,u n){
  vu fc=Rot(st.fc);
  vS v;
  v.reserve(4*fc.size());
  for(u c:fc){
    v.push_back(st);
    v.back().fc=c;
    v.back().CU(n);
    v.push_back(st);
    v.back().fc=c;
    v.back().CD(n);
    v.push_back(st);
    v.back().fc=c;
    v.back().RR(n);
    v.push_back(st);
    v.back().fc=c;
    v.back().RL(n);
  }
  assert(v.size()==4*fc.size());
  return v;
}
// Returns all states created by inserting the free card at any valid location.
vS Expand(S& st){
  vS v;
  v.reserve(8*(st.n-1));
  for(u n=1;n<st.n;n+=2)
    for(S& st2:In(st,n))
      v.push_back(st2);
  assert(v.size()<=8*(st.n-1));
  return v;
}
