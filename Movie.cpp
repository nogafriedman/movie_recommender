#include "Movie.h"
#define HASH_START 17
#define RES_MULT 31

/**
 * hash function used for a unordered_map (implemented for you)
 * @param movie shared pointer to movie
 * @return an integer for the hash map
 */
std::size_t sp_movie_hash(const sp_movie& movie){
    std::size_t res = HASH_START;
    res = res * RES_MULT + std::hash<std::string>()(movie->get_name());
    res = res * RES_MULT + std::hash<int>()(movie->get_year());
    return res;
}

/**
 * equal function used for an unordered_map (implemented for you)
 * @param m1
 * @param m2
 * @return true if the year and name are equal else false
 */
bool sp_movie_equal(const sp_movie& m1,const sp_movie& m2){
    return !(*m1 < *m2) && !(*m2 < *m1);
}

Movie::Movie(const std::string& name, int year) // constructor
{
  _name = name;
  _year = year;
}

/**
 * returns the name of the movie.
 * @return std::string - name
 */
std::string Movie::get_name() const
{
  return _name;
}

/**
 * returns the year the movie was released.
 * @return int - year
 */
int Movie::get_year () const
{
  return _year;
}

/**
 * this function compares two movies by the year they were released and
 * returns True if lhs movie was released earlier.
 * if both movies were released in the same year, the function compares
 * their names and returns True if the name of lhs is lexicographically
 * smaller than the rhs name.
 * @param rhs const movie reference
 * @return bool
 */
bool Movie::operator<(const Movie& rhs) const
{
  if (this->_year == rhs.get_year())
  {
    return this->_name < rhs.get_name();
  }
  else
  {
    return this->_year < rhs.get_year();
  }
}

/**
 * passes data to os object by the following format:
 * <movie_name> (<movie_year>), with a newline in the end.
 * @param os std::ostream reference
 * @param movie const Movie reference
 * @return std::ostream
 */
std::ostream& operator<<(std::ostream& os, const Movie& movie)
{
  os << movie.get_name() << " (" << movie.get_year() << ") " << std::endl;
  os << movie.get_name() << " (" << movie.get_year() << ") " << std::endl;
  return os;
}