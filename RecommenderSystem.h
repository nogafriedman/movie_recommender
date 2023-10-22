#ifndef SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
#define SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H

#include "RSUser.h"
#include <map>
#include <set>
#include "Movie.h"

typedef std::pair<double, double> data; // movie rate, similarity res
typedef bool (*comp_func)(const sp_movie& m1, const sp_movie& m2);
typedef std::map<sp_movie, std::vector<double>, comp_func> rs_map;

class RecommenderSystem
{
 private:
  rs_map _movies;

  // helper functions:
  static double calc_mean(rank_map ranks_vector);
  static std::vector<double> scalar_multiplication(double scalar,
                                                 std::vector<double>& vector);
  static std::vector<double> vector_addition
  (const std::vector<double>& first_vec,
   const std::vector<double>& second_vec);
  static std::vector<double> calc_preference(const rank_map& ranks_vector,
                                             double mean_ratings,
                                             rs_map& _movies,
                                             unsigned long long num_features);
  static double inner_product(const std::vector<double>& vector_1, const
  std::vector<double>& vector_2);
  static double calc_similarity(const std::vector<double>&
  preference_vector, const std::vector<double>& features_vector);
  static std::set<data> get_k_most_similar(std::vector<data> pairs, int k);
  static bool compare_by_rank(const data& m1, const data& m2);
  static double calc_norm(const std::vector<double>& vector);


  // comparator func:
  static bool comp_map(const sp_movie& m1, const sp_movie& m2);
 public:

	explicit RecommenderSystem(): _movies(comp_map){}

    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name, int year,const
    std::vector<double>& features);

    /**
     * a function that calculates the movie with highest score based on movie
     * features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_content(const RSUser& user);

    /**
     * a function that calculates the movie with highest predicted score
     * based on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const RSUser& user, int k);

    /**
     * Predict a user rating for a movie given argument using item cf
     * procedure with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const RSUser &user, const sp_movie &movie,
                               int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) const;

	friend std::ostream& operator<<(std::ostream& os, const
    RecommenderSystem& rs);
};

#endif //SCHOOL_SOLUTION_RECOMMENDERSYSTEM_H
