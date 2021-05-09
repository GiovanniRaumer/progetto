#ifndef ORBIT_HPP 
#define ORBIT_HPP 

#include <cmath>
#include <vector>

struct State {
  
  double x;
  double y;
  double v_x;
  double v_y;
  double a_x;
  double a_y;
};

class Orbit 
{
 State m_initial_state;
 double m_duration;

 public:
  Orbit(State const& initial_state, double duration_in_seconds)
     : m_initial_state{initial_state}, m_duration{duration_in_seconds}
  {
  }

  std::vector<State> generate_points(int N) const
  {
    std::vector<State> result{m_initial_state};

    double const k=6.674e-11*1.989e30;
    double const e=m_duration/N;
    
    for(int i=1; i!=N; ++i) {
      State next{};
      State const& prev=result.back(); 
      next.a_x=-k*(prev.x/pow((pow(prev.x,2)+pow(prev.y,2)),1.5));
      next.a_y=-k*(prev.y/pow((pow(prev.x,2)+pow(prev.y,2)),1.5));
      next.v_x=prev.x+(e*next.a_x);
      next.v_y=prev.y+(e*next.a_y);
      next.x=prev.x+e*next.v_x;
      next.y=prev.y+e*next.v_y;
      
      

      result.push_back(next);
    }
    return result;
  }  
};


#endif

