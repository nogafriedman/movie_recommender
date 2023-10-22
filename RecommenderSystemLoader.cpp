#include "RecommenderSystemLoader.h"
#include <string>
#include <sstream>
#include <fstream>

#define INVALID_PATH_ERROR "ERROR: given file is invalid."
#define RANGE_ERROR "ERROR: data out of range (1.0 - 10.0)."
#define HYPHEN '-'
#define MAX_LIMIT 10.0
#define MIN_LIMIT 1.0

std::unique_ptr<RecommenderSystem>
    RecommenderSystemLoader::create_rs_from_movies_file
    (const std::string &movies_file_path) noexcept (false)
{
  std::ifstream input_file(movies_file_path);
  if (!(input_file.is_open()))
  {
    throw std::runtime_error((INVALID_PATH_ERROR));
  }
  RecommenderSystem rs; // create a new RecommenderSystem
  std::string line;
  while (std::getline(input_file, line)) // start reading lines from file
  {
    std::string cur_word; // current word read from file
    std::vector<double> features_vector; // a vector to put the features in
    std::string movie_name; // movie name read from file
    int year; // release year for the current movie
    double cur_feature; // feature element for the current movie
    unsigned long long hyphen; // separate data by hyphen <movie_name-year>
    std::stringstream str_stream(line);
    str_stream >> cur_word;
    hyphen = cur_word.find(HYPHEN);
    movie_name = cur_word.substr(0, hyphen); // cut from beg to hyphen
    cur_word.erase(0, hyphen + sizeof(HYPHEN)); // todo
    year = std::stoi(cur_word); // todo
    while (str_stream >> cur_feature)
    { // check data is valid:
      if (cur_feature < MIN_LIMIT || cur_feature > MAX_LIMIT)
      {
        throw std::runtime_error(RANGE_ERROR);
      }
      features_vector.push_back(cur_feature); // add data into features vector
    } // add movie to system:
    rs.add_movie (movie_name, year, features_vector);
  };
  return std::make_unique<RecommenderSystem>(rs);
}