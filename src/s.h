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
  A(u t,u p,u c,u x,u y):v_(t|(p<<3)),c_(c),x_(x),y_(y){}
  // Returns the action type. 0:CD, 1:CU, 2:RR, 3:RL
  u T(){return v_&3u;} 
  // Returns the position (row/column) of the action.
  i P(){return v_>>3;}
  // Returns the string representation in output format.
  s Str(){
    ss s;
    s<<c_<<" "<<(T()/2?"r ":"c ")<<(P()+1)*(T()%2?-1:1)<<" "<<x_<<" "<<y_;
    return s.str();
  }
  u v_,c_,x_,y_;
};
// State
struct S{
  S():n_(0),fc_(0),t_(0),nt_(1),x_(0),y_(0),c_(0){}
  S(u n,u t,u nt,u fc,u x,u y):n_(n),fc_(fc),t_(t),nt_(nt),x_(x),y_(y){}
  // Inserts free card in given column pushing up.
  void CU(u x){
    ++c_;
    a_.push_back(A(1,x,fc_,x_,y_));
    for(u y=0;y<n_;++y)swap(fc_,F(x,y));
  }
  // Inserts free card in given column pushing down.
  void CD(u x){
    ++c_;
    a_.push_back(A(0,x,fc_,x_,y_));
    for(i y=n_-1;y>=0;--y)swap(fc_,F(x,y));
  }
  // Inserts free card in given row pushing right.
  void RR(u y){
    ++c_;
    a_.push_back(A(2,y,fc_,x_,y_));
    for(u x=0;x<n_;++x)swap(fc_,F(x,y));
  }
  // Inserts free card in given row pushing left.
  void RL(u y){
    ++c_;
    a_.push_back(A(3,y,fc_,x_,y_));
    for(i x=n_-1;x>=0;--x)swap(fc_,F(x,y));
  }
  // Returns all reachable positions from the given postion.
  vuu Con(u x,u y){
    assert(x<n_&&y<n_);
    vuu v;
    u c=C(x,y);
    if(x>0&&c!=1&&c!=2&&c!=5&&c!=10){
      // Left.
      u l=C(x-1,y);
      if(l!=1&&l!=3&&l!=4&&l!=9)
        v.push_back({x-1,y});
    }
    if(x+1<n_&&c!=1&&c!=3&&c!=4&&c!=9){
      // Right.
      u r=C(x+1,y);
      if(r!=1&&r!=2&&r!=5&&r!=10)
        v.push_back({x+1,y});
    }
    if(y>0&&c!=0&&c!=4&&c!=5&&c!=7){
      // Down.
      u d=C(x,y-1);
      if(d!=0&&d!=2&&d!=3&&d!=8)
        v.push_back({x,y-1});
    }
    if(y+1<n_&&c!=0&&c!=2&&c!=3&&c!=8){
      // Up.
      u u=C(x,y+1);
      if(u!=0&&u!=4&&u!=5&&u!=7)
        v.push_back({x,y+1});
    }
    return v;
  }
  bool operator<(const S& r)const{
    return c_<r.c_;
  }
  // Returns the next target's position.
  uu NTP(){
    for(u y=0;y<n_;++y)
      for(u x=0;x<n_;++x)
        if(T(x,y)==nt_)return {x,y};
    assert(false);
  }
  // Returns the field value reference for given position.
  u& F(u x,u y){return F(x+y*n_);}
  // Returns the target value for given position.
  u T(u x,u y){return T(x+y*n_);}
  // Returns the card id for given position.
  u C(u x,u y){return C(x+y*n_);}
  // Returns the field value reference for given index.
  u& F(u n){
    assert(n<b_.size());
    return b_[n];
  }
  // Returns the target value of given index.
  u T(u n){return F(n)>>4;}
  // Returns the card id of given index.
  u C(u n){return F(n)&0xf;}
  // Returns the string representation of the state.
  s Str(){
    ss s;
    s<<"n: "<<n_<<"\ntargets: "<<t_<<"\ncard: "<<fc_<<"\npos: "<<x_<<","<<y_
     <<"\ncost: "<<c_<<"\n";
    for(i y=n_-1;y>=0;--y){
      for(u x=0;x<n_;++x)
        s<<ASCII(C(x,y))<<"("<<T(x,y)<<")\t";
      s<<"\n";
    }
    return s.str();
  }
  // Returns the solution string.
  s AStr(){
    ss s;
    for(A& e:a_)s<<e.Str()<<"\n";
    return s.str();
  }
  // Board dimension, free card, number of targets, next target id,
  // x and y positions, costs.
  u n_,fc_,t_,nt_,x_,y_,c_;
  // The board fields.
  vu b_;
  // The recorded actions.
  vA a_;
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
  vu fc=Rot(st.fc_);
  vS v;
  v.reserve(4*fc.size());
  for(u c:fc){
    v.push_back(st);
    v.back().fc_=c;
    v.back().CU(n);
    v.push_back(st);
    v.back().fc_=c;
    v.back().CD(n);
    v.push_back(st);
    v.back().fc_=c;
    v.back().RR(n);
    v.push_back(st);
    v.back().fc_=c;
    v.back().RL(n);
  }
  assert(v.size()==4*fc.size());
  return v;
}
// Returns all states created by inserting the free card at any sane location.
vS Expand(S& st){
  vS v;
  v.reserve(8*(st.n_-1));
  for(u n=1;n<st.n_;n+=2)
    for(S& st2:In(st,n))
      v.push_back(st2);
  assert(v.size()<=8*(st.n_-1));
  return v;
}
u MD(i x1,i y1,i x2,i y2){
  return abs(x1-x2)+abs(y1-y2);
}
// Returns all reachable positions for given state.
vuu Explore(S& st){
  vuu s={{st.x_,st.y_}};
  suu r;
  r.insert(s.back());
  while(s.size()){
    u x=s.back().first;
    u y=s.back().second;
    s.pop_back();
    for(uu xy:st.Con(x,y)){
      if(!r.count(xy)){
        r.insert(xy);
        s.push_back(xy);
      }
    }
  }
  return vuu(r.begin(), r.end());
}
  
S Search(S& st){
  qS q;
  q.push(st);
  while(q.size()){
    S t=q.top();
    q.pop();
    if(!t.t_)return t;
    for(S& s:Expand(t)){
      q.push(s);
    }
  }
  return st;
}
