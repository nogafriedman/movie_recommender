#include "RecommenderSystem.h"
#include "RSUser.h"
#include <cstdlib>
#include <set>
#include <algorithm>
#include <cmath>

#define SIMILARITY_LIM -2.0

/**
 * Calculates the normalized vector of the vector given as a parameter,
 * according to the requested algorithm (subtracts the mean from each index).
 * @param ranks rank_map
 * @return normalized ranks vector
 */
double RecommenderSystem::calc_mean(rank_map ranks_vector) // todo const?
{
  double sum_ratings = 0;
  int num_rated = 0;
  for (auto& movie_data : ranks_vector) // calculate mean:
  {
    if (movie_data.second != 0) // if ratings is not NA (0)
    {
      sum_ratings += movie_data.second;
      num_rated++;
    }
  }
  double mean_ratings = sum_ratings / num_rated;
  return mean_ratings;
}

/**
 * Calculates scalar multiplication.
 * @param scalar - int
 * @param vector - std::vector<double> reference
 * @return new vector which is the product of the multiplication.
 */
std::vector<double> RecommenderSystem::scalar_multiplication
(double scalar, std::vector<double>& vector)
{
  std::vector<double> new_vector;
  for (auto elem : vector)
  {
    new_vector.push_back(scalar * elem);
  }
  return new_vector;
}

/**
 * calculates the vector which is the sum of the two vectors given as
 * arguments and returns it by value.
 * @param first_vec std::vector<double> reference
 * @param second_vec std::vector<double> reference
 * @return std::vector<double> by value
 */
std::vector<double> RecommenderSystem::vector_addition
(const std::vector<double>& first_vec, const std::vector<double>& second_vec)
{
  std::vector<double> sum_vector = first_vec; // copy
  size_t size = first_vec.size();
  for (size_t i = 0; i < size; i++)
  {
    sum_vector[i] += second_vec[i];
  }
  return sum_vector;
}

/**
 * Calculate the preference vector according to the requested algorithm:
 * run through all movies watched by user
 * for each movie, multiply it's features vector by the scalar of the rating
 * (after normalization)
 * sum all of the calculated results to one vector.
 * @param normalized_ranks - rank_map const reference
 * @return std::vector<double> vector which is the sum of all calculations
 */
std::vector<double> RecommenderSystem::calc_preference(const rank_map&
ranks, double mean_ratings, rs_map& _movies, unsigned long long
num_features)
{
  std::vector<double> preference_vector (num_features, 0.0);
  for (const auto& elem : ranks) // was normalized_ranks
  {
    if (elem.second != 0)
    {
      std::vector<double> features = _movies[elem.first];
      std::vector<double> vec = scalar_multiplication(elem.second - mean_ratings,
                                                      features);
      preference_vector = vector_addition (preference_vector,vec);
    }
  }
  return preference_vector;
}

/**
 * calculates the norm of a vector and returns a double value of it.
 * @param vector std::vector<double>
 * @return double
 */
double RecommenderSystem::calc_norm(const std::vector<double>& vector)
{
  double powered_norm = 0;
  for (size_t i = 0; i < vector.size(); i++)
  {
    powered_norm += vector[i] * vector[i];
  }
  double norm = std::sqrt(powered_norm);
  return norm;
}

/**
 * calculates the inner product of two given vectors and returns a double
 * value of the result.
 * @param vector_1 const std::vector<double> reference
 * @param vector_2 const std::vector<double> reference
 * @return double - inner product result
 */
double RecommenderSystem::inner_product
(const std::vector<double>& vector_1, const std::vector<double>& vector_2)
{
  double res = 0.0;
  for (size_t i = 0; i < vector_1.size(); i++)
  {
    res += vector_1[i] * vector_2[i];
  }
  return res;
}

/**
 * calculates the similarity of a given movie to the features preferred by
 * the user.
 * @param preference_vector const std::vector<double>&
 * @param features_vector const std::vector<double>&
 * @return double representing the similarity of the movie to the user's taste.
 */
double RecommenderSystem::calc_similarity(const std::vector<double>&
    preference_vector, const std::vector<double>& features_vector)
{
  double numerator = inner_product(preference_vector,
                                   features_vector);
  double denominator = calc_norm(preference_vector) * calc_norm
      (features_vector);
  double res = numerator / denominator;
  return res;
}

/**
 * calculates a recommendation of a movie to the user based on other movies
 * the user rated high.
 * @param user const RSUser&
 * @return sp_movie movie recommendation
 */
sp_movie RecommenderSystem::recommend_by_content(const RSUser& user)
{
  rank_map ranks_copy = user.get_ranks(); // calls copy constructor
  double mean_ratings = calc_mean (ranks_copy);
  std::vector<double> preference_vector;
  double max = SIMILARITY_LIM; // similarity is a value between -1 and 1
  sp_movie most_similar;
  int count = 0;
  std::vector<double> features;
  for (auto& movie : user.get_ranks())
  {
    if (movie.second == 0) // if the movie has no rating - NA
    {
      features = _movies[movie.first];
      if (count == 0) // calc the preference vector only on the first iteration
      {               // - then use it for the rest (it's the same vector)
        preference_vector = calc_preference(user.get_ranks(), mean_ratings,
                                            _movies, features.size());
      }
      double similarity = calc_similarity(preference_vector, features);
      if (similarity > max)
      {
        max = similarity;
        most_similar = movie.first;
      }
      count++;
    }
  }
  return most_similar;
}

/**
 * compares two movies by their rating and returns True if the first movie's
 * rating is higher or equals to the second movie.
 * @param m1 const data& - whereas data is an std::pair of two doubles
 * @param m2 const data&
 * @return bool - True if the rating of the first movie is higher or equals
 */
bool RecommenderSystem::compare_by_rank(const data& m1, const data& m2)
{
  return m1.second < m2.second; // compare the rates of both movies
}

/**
 * calculates the k most similar movies to a given movie and returns them in
 * a vector that contains their data, organized as a pair object.
 * @param pairs std::vector<data> (whereas data is a pair of two doubles)
 * @param k int representing the amount of movies to fetch from the map
 * @return std::set<data> of the k most similar movies, as pair objects
 */
std::set<data> RecommenderSystem::get_k_most_similar
(std::vector<data> pairs, int k)
{
  std::sort(pairs.begin(), pairs.end(), compare_by_rank);
  std::set<data> k_most_similar;
  unsigned long long n = pairs.size();
  for (int i = 0; i <= k; i++)
  {
    k_most_similar.insert(pairs[n-i]);
  }
  return k_most_similar;
}

double RecommenderSystem::predict_movie_score(const RSUser &user, const
sp_movie &movie, int k)
{
  std::vector<data> pairs;
  for (auto& elem : user.get_ranks())
  {
    if (elem.second != 0) // if the elem has a rating - not 0/NA
    { // calc it's similarity to the cur movie and add to a pair vector:
      std::vector<double> cur_features = _movies[elem.first];
      std::vector<double> na_movie_features = _movies[movie];
      double similarity = calc_similarity(na_movie_features, cur_features);
      auto cur_data = std::make_pair(elem.second, similarity);
      pairs.push_back(cur_data);
    }
  }
  std::set<data> k_most_similar = get_k_most_similar(pairs, k);
  double numerator = 0;
  double denominator = 0; // sum similarities
  for (auto elem : k_most_similar)
  { // calcs (rate_m1 * similarity_m1) + ... + (rate_mk * similarity_mk)
    numerator += elem.first * elem.second;
    denominator += elem.second;
  }
  return (numerator / denominator);
}

sp_movie RecommenderSystem::recommend_by_cf(const RSUser& user, int k)
{
  sp_movie most_similar = nullptr;
  double max = SIMILARITY_LIM;
  // loop through all movies the user has not watched yet - NA movies:
  for (auto& elem : user.get_ranks())
  {
    if (elem.second == 0)
    {
      double prediction_rate = predict_movie_score (user, elem.first, k);
      if (prediction_rate > max)
      {
        max = prediction_rate;
        most_similar = elem.first;
      }
    }
  }
  return most_similar;
}

sp_movie RecommenderSystem::add_movie(const std::string& name, int year, const
std::vector<double>& features)
{
  sp_movie new_movie = std::make_shared<Movie>(name, year);
  _movies[new_movie] = features;
  return new_movie;
}

sp_movie RecommenderSystem::get_movie(const std::string &name, int year) const
{
  sp_movie cur_movie = std::make_shared<Movie>(name, year);
  for (const auto& movie : _movies)
  {
    if (sp_movie_equal (movie.first, cur_movie))
    {
      return movie.first; // return the smart pointer to the movie
    }
  }
  return nullptr;
}

std::ostream& operator<<(std::ostream& os, const
RecommenderSystem& rs)
{
  if (!os)
  {
    return os;
  }
  for (const auto& movies : rs._movies)
  {
    os << *(movies.first);
  }
  return os;
}

bool RecommenderSystem::comp_map(const sp_movie& m1, const sp_movie& m2)
{
  return *m1 < *m2;
}