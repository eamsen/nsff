#include "g.h"
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
  S():n_(0),fc_(0),t_(0),nt_(1),x_(0),y_(0),c_(0),h_(0){}
  S(S&& o):n_(o.n_),fc_(o.fc_),t_(o.t_),nt_(o.nt_),x_(o.x_),y_(o.y_),c_(o.c_),
           h_(o.h_),b_(move(o.b_)),a_(move(o.a_)){}
  S(const S& o):n_(o.n_),fc_(o.fc_),t_(o.t_),nt_(o.nt_),x_(o.x_),y_(o.y_),
                c_(o.c_),h_(o.h_),b_(o.b_),a_(o.a_){}
  // Moves the player position.
  void M(u x,u y){
    assert(x<n_&&y<n_);
    assert(a_.size());
    x_=x;
    y_=y;
    a_.back().x_=x;
    a_.back().y_=y;
    if(T(x,y)==nt_)++nt_;
  }
  // Returns whether the game is solved.
  bool Sol(){return nt_>t_;}
  // Inserts free card in given column pushing up.
  void CU(u x){
    ++c_;
    a_.push_back(A(1,x,fc_&0xf,x_,y_));
    for(u y=0;y<n_;++y)swap(fc_,F(x,y));
    if(x==x_&&++y_==n_)y_=0;
  }
  // Inserts free card in given column pushing down.
  void CD(u x){
    ++c_;
    a_.push_back(A(0,x,fc_&0xf,x_,y_));
    for(i y=n_-1;y>=0;--y)swap(fc_,F(x,y));
    if(x==x_&&--y_>n_)y_=n_-1;
  }
  // Inserts free card in given row pushing right.
  void RR(u y){
    ++c_;
    a_.push_back(A(2,y,fc_&0xf,x_,y_));
    for(u x=0;x<n_;++x)swap(fc_,F(x,y));
    if(y==y_&&++x_==n_)x_=0;
  }
  // Inserts free card in given row pushing left.
  void RL(u y){
    ++c_;
    a_.push_back(A(3,y,fc_&0xf,x_,y_));
    for(i x=n_-1;x>=0;--x)swap(fc_,F(x,y));
    if(y==y_&&--x_>n_)x_=n_-1;
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
    // Yeah, I know, but this saves characters.
    return i(c_+h_)-i(nt_)>i(r.c_+r.h_)-i(r.nt_);
  }
  // Returns the next target's position.
  uu NTP(){
    for(u y=0;y<n_;++y)
      for(u x=0;x<n_;++x)
        if(T(x,y)==nt_)return{x,y};
    // Next target card is the free card.
    return{0,0};
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
    s<<"n: "<<n_<<"\ntargets: "<<t_<<"\nnext target: "<<nt_
     <<"\ncard: "<<(fc_&0xf)<<"("<<(fc_>>4)<<")"
     <<"\npos: "<<x_<<","<<y_<<"\ncost: "<<c_<<"\n";
    for(i y=n_-1;y>=0;--y){
      for(u x=0;x<n_;++x)s<<C(x,y)<<"("<<T(x,y)<<")\t";
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
  // x and y positions, costs, heuristic costs.
  u n_,fc_,t_,nt_,x_,y_,c_,h_;
  // The board fields.
  vu b_;
  // The recorded actions.
  vA a_;
};
// Returns all rotated card ids for given card id.
vu Rot(u id){
  u t=id&0xfffffff0;
  u c=id&0xf;
  if(c<2)return{t,t|1u};
  else if(c<6)return{t|2u,t|3u,t|4u,t|5u};
  else if(c==6)return{t|6u};
  return{t|7u,t|8u,t|9u,t|10u};
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
vS E1(S& st){
  vS v;
  v.reserve(8*(st.n_-1));
  for(u n=1;n<st.n_;n+=2)
    for(S& st2:In(st,n))
      v.push_back(st2);
  assert(v.size()<=8*(st.n_-1));
  return v;
}
// Returns the manhattan distance for the given positions.
u MD(i x1,i y1,i x2,i y2){return abs(x1-x2)+abs(y1-y2);}
u MD(uu p1,uu p2){return MD(p1.first,p1.second,p2.first,p2.second);}
// Returns at most the given number of reachable positions for given state.
// Results are pair(f-cost,pair(x,y)).
vuuu E2(S& st,u n){
  uu p=st.NTP();
  vuu s={{st.x_,st.y_}};
  suuu r;
  r.insert({MD({st.x_,st.y_},p),{st.x_,st.y_}});
  while(s.size()){
    u x=s.back().first;
    u y=s.back().second;
    s.pop_back();
    for(uu xy:st.Con(x,y)){
      u d=MD(p,xy);
      if(!r.count({d,xy})){
        r.insert({d,xy});
        s.push_back(xy);
      }
    }
  }
  r.erase({MD({st.x_,st.y_},p),{st.x_,st.y_}});
  vuuu v;
  v.reserve(n);
  for(auto& e:r){
    v.push_back(e);
    if(v.size()==n)break;
  }
  return v;
}
// Searches for a solution for given state/game by a hill-climbing/A* hybrid.
// Returns the solution state, or the start state, if no solution was found.
S Se(S& st){
  if(st.T(st.x_,st.y_)==1)++st.nt_;
  qS q;
  st.h_=MD({st.x_,st.y_},st.NTP());
  q.push(st);
  while(q.size()){
    S t=q.top();
    q.pop();
    if(t.Sol())return t;
    for(S& s:E1(t))
      for(uuu& p:E2(s,30)){
        s.M(p.second.first,p.second.second);
        s.h_=p.first;
        q.push(s);
      }
  }
  return st;
}
