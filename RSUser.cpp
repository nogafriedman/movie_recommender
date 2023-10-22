
// don't change those includes
#include "RSUser.h"
#include "RecommenderSystem.h"

RSUser::RSUser(std::string username, rank_map ranks,
               std::shared_ptr<RecommenderSystem> rs)
{
  _username = username;
  _ranks = ranks;
  _rs = rs;
}

std::string RSUser::get_name() const
{
  return _username;
}

void RSUser::add_movie_to_rs(const std::string &name, int year,
                     const std::vector<double> &features,
                     double rate)
{
  std::shared_ptr<Movie> new_movie = _rs->add_movie(name, year, features);
  _ranks[new_movie] = rate;
}

sp_movie RSUser::get_recommendation_by_content () const
{
  return this->_rs->recommend_by_content(*this);
}

double RSUser::get_prediction_score_for_movie(const std::string& name, int
year, int k) const
{
  sp_movie new_movie = _rs->get_movie(name, year);
  return _rs->predict_movie_score(*this, new_movie, k);
}

sp_movie RSUser::get_recommendation_by_cf(int k) const
{
  return this->_rs->recommend_by_cf(*this, k);
}

std::ostream& operator<<(std::ostream& os, RSUser& user) // todo
{
 os << "name: " << user.get_name() << std::endl;
 os << *(user._rs) << std::endl;
 return os;
}

rank_map RSUser::get_ranks () const
{
  return _ranks;
}