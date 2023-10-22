#include "RSUsersLoader.h"
#include "RecommenderSystem.h"
#include "RSUser.h"
#include <fstream>
#include <sstream>

#define INVALID_PATH_ERROR "ERROR: given file is invalid."
#define HYPHEN '-'
#define NA "NA"
#define ZERO 0.0

void RSUsersLoader::get_users(const std::string& str,
                              const std::vector<Movie>& movies_vector,
                              std::vector<RSUser>& users_vector,
                              std::shared_ptr<RecommenderSystem> rs)
{
  int i = 0;
  double cur_ranking;
 std::string cur_user, cur_word;
 rank_map user_rankings(0, sp_movie_hash, sp_movie_equal);
 sp_movie cur_movie_ptr;
 std::stringstream str_stream(str);
 str_stream >> cur_user;
 while (str_stream >> cur_word)
 {
   if (cur_word != NA)
   {
     cur_ranking = std::stoi(cur_word);
   }
   else
   {
     cur_ranking = ZERO;
   }
   cur_movie_ptr = rs->get_movie(movies_vector[i].get_name(),
                                 movies_vector[i].get_year());
   user_rankings[cur_movie_ptr] = cur_ranking;
   i++;
 }
 users_vector.push_back((RSUser) {cur_user, user_rankings, rs});
}

void RSUsersLoader::get_movies(std::string& str,
                               std::vector<Movie>& movies_vector)
{
  unsigned long long hyphen;
  int cur_year;
  sp_movie cur_movie_ptr;
  std::stringstream str_stream(str);
  std::string cur_movie_name, cur_word;
  while (str_stream >> cur_word)
  {
    hyphen = cur_word.find(HYPHEN);
    cur_movie_name = cur_word.substr(0, hyphen);
    cur_word.erase(0, hyphen + sizeof(HYPHEN));
    cur_year = std::stoi(cur_word);
    movies_vector.push_back((Movie){cur_movie_name, cur_year});
  }
}

std::vector<RSUser> RSUsersLoader::create_users_from_file(const std::string&
users_file_path, std::shared_ptr<RecommenderSystem> rs) noexcept(false)
{
  std::ifstream user_file (users_file_path);
  if (!user_file)
  {
    throw std::runtime_error (INVALID_PATH_ERROR);
  }
  std::string line;
  std::getline (user_file, line);
  std::vector<Movie> movies_vector;
  std::vector<RSUser> users_vector;
  get_movies (line, movies_vector);
  while (std::getline (user_file, line))
  {
    get_users (line, movies_vector, users_vector, rs);
  }
  return users_vector;
}
