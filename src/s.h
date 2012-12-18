// #ifndef S_H
// #define S_H
// #include "g.h"
// Action
struct A{
  A(u t,u p,u c,u x,u y):v(t|(p<<3)),c(c),x(x),y(y){}
  // Returns the action type. 0:CD, 1:CU, 2:RR, 3:RL
  u T(){r v&3u;} 
  // Returns the position (row/column) of the action.
  i P(){r v>>3;}
  // Returns the string representation in output format.
  s B(){
    ss s;
    s<<c<<" "<<(T()/2?"r ":"c ")<<(P()+1)*(T()%2?-1:1)<<" "<<x<<" "<<y;
    r s.str();
  }
  u v,c,x,y;
};
// State
struct S{
  S():n(0),fc(0),t(0),nt(1),x(0),y(0),c(0),h(0){}
  S(S&& o):n(o.n),fc(o.fc),t(o.t),nt(o.nt),x(o.x),y(o.y),c(o.c),
           h(o.h),bo(move(o.bo)),a(move(o.a)){}
  S(const S& o):n(o.n),fc(o.fc),t(o.t),nt(o.nt),x(o.x),y(o.y),
                c(o.c),h(o.h),bo(o.bo),a(o.a){}
  // Moves the player position.
  void M(u o,u k){x=o;y=k;a.b().x=o;a.b().y=k;if(T(o,k)==nt)++nt;}
  // Returns whether the game is solved.
  i O(){r nt>t;}
  // Inserts free card in given column pushing up.
  void CU(u o){
    ++c;a.push_back(A(1,o,fc&0xf,x,y));
    for(u k=0;k<n;++k)swap(fc,F(o,k));
    if(o==x&&++y==n)y=0;
  }
  // Inserts free card in given column pushing down.
  void CD(u o){
    ++c;a.push_back(A(0,o,fc&0xf,x,y));
    for(i k=n-1;k>=0;--k)swap(fc,F(o,k));
    if(o==x&&--y>n)y=n-1;
  }
  // Inserts free card in given row pushing right.
  void RR(u k){
    ++c;a.push_back(A(2,k,fc&0xf,x,y));
    for(u o=0;o<n;++o)swap(fc,F(o,k));
    if(k==y&&++x==n)x=0;
  }
  // Inserts free card in given row pushing left.
  void RL(u k){
    ++c;a.push_back(A(3,k,fc&0xf,x,y));
    for(i o=n-1;o>=0;--o)swap(fc,F(o,k));
    if(k==y&&--x>n)x=n-1;
  }
  // Returns all reachable positions from the given postion.
  vuu Con(u o,u k){
    vuu v;u c=C(o,k);
    if(o>0&&c!=1&&c!=2&&c!=5&&c!=10){
      // Left.
      u l=C(o-1,k);
      if(l!=1&&l!=3&&l!=4&&l!=9)v.push_back({o-1,k});
    }
    if(o+1<n&&c!=1&&c!=3&&c!=4&&c!=9){
      // Right.
      u b=C(o+1,k);
      if(b!=1&&b!=2&&b!=5&&b!=10)v.push_back({o+1,k});
    }
    if(k>0&&c!=0&&c!=4&&c!=5&&c!=7){
      // Down.
      u d=C(o,k-1);
      if(d!=0&&d!=2&&d!=3&&d!=8)v.push_back({o,k-1});
    }
    if(k+1<n&&c!=0&&c!=2&&c!=3&&c!=8){
      // Up.
      u u=C(o,k+1);
      if(u!=0&&u!=4&&u!=5&&u!=7)v.push_back({o,k+1});
    }
    r v;
  }
  // Yeah, I know, but this saves characters.
  i operator<(const S& g)const{r i(c+h)-i(nt)>i(g.c+g.h)-i(g.nt);}
  // Returns the next target's position or the current position if the target is
  // on the free card.
  uu NTP(){for(u k=0;k<n;++k)for(u o=0;o<n;++o)if(T(o,k)==nt)r{o,k};r{x,y};}
  // Returns the field value reference for given position.
  u& F(u o,u k){r F(o+k*n);}
  // Returns the target value for given position.
  u T(u o,u k){r T(o+k*n);}
  // Returns the card id for given position.
  u C(u o,u k){r C(o+k*n);}
  // Returns the field value reference for given index.
  u& F(u g){r bo[g];}
  // Returns the target value of given index.
  u T(u g){r F(g)>>4;}
  // Returns the card id of given index.
  u C(u g){r F(g)&0xf;}
  // Returns the solution string.
  s B(){ss s;for(A& e:a)s<<e.B()<<"\n";r s.str();}
  // Board dimension, free card, number of targets, next target id,
  // x and y positions, costs, heuristic costs.
  u n,fc,t,nt,x,y,c,h;
  // The board fields.
  vu bo;
  // The recorded actions.
  vA a;
};
// Returns all rotated card ids for given card id.
vu Rot(u id){
  u t=id&0xfffffff0;u c=id&0xf;
  if(c<2)r{t,t|1u};else if(c<6)r{t|2u,t|3u,t|4u,t|5u};else if(c==6)r{id};
  r{t|7u,t|8u,t|9u,t|10u};
}
// Returns all states resulting in inserting at given row and column.
vS In(S& st,u n){
  vu fc=Rot(st.fc);vS v;v.reserve(4*fc.size());
  for(u c:fc){
    v.pb(st);v.b().fc=c;v.b().CU(n);
    v.pb(st);v.b().fc=c;v.b().CD(n);
    v.pb(st);v.b().fc=c;v.b().RR(n);
    v.pb(st);v.b().fc=c;v.b().RL(n);
  }
  r v;
}
// Returns all states created by inserting the free card at any sane location.
vS E1(S& st){
  vS v;v.reserve(8*(st.n-1));
  for(u n=1;n<st.n;n+=2)for(S& st2:In(st,n))v.pb(st2);
  r v;
}
// Returns the manhattan distance for the given positions.
u MD(i x1,i y1,i x2,i y2){r abs(x1-x2)+abs(y1-y2);}
u MD(uu p1,uu p2){r MD(p1.fi,p1.se,p2.fi,p2.se);}
// Returns at most the given number of reachable positions for given state.
// Results are pair(f-cost,pair(x,y)).
vuuu E2(S& st,u n){
  uu p=st.NTP();vuu s={{st.x,st.y}};suuu b;
  b.insert({MD({st.x,st.y},p),{st.x,st.y}});
  w(s.size()){
    u x=s.b().fi;u y=s.b().se;s.pop_back();
    for(uu xy:st.Con(x,y)){
      u d=MD(p,xy);
      if(!b.count({d,xy})){b.insert({d,xy});s.pb(xy);}
    }
  }
  b.erase({MD({st.x,st.y},p),{st.x,st.y}});vuuu v;v.reserve(n);
  for(auto& e:b){
    v.pb(e);
    if(v.size()==n)break;
  }
  r v;
}
// Searches for a solution for given state/game by a hill-climbing/A* hybrid.
// Returns the solution state, or the start state, if no solution was found.
S Se(S& st){
  if(st.T(st.x,st.y)==1)++st.nt;
  qS q;st.h=MD({st.x,st.y},st.NTP());q.push(st);
  w(q.size()){
    S t=q.top();q.pop();
    if(t.O())r t;
    for(S& s:E1(t))
      for(uuu& p:E2(s,5)){S s2=s;s2.M(p.se.fi,p.se.se);s2.h=p.fi;q.push(s2);}
  }
  r st;
}
// #endif  // S_H
