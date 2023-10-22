
#ifndef SCHOOL_SOLUTION_USERFACTORY_H
#define SCHOOL_SOLUTION_USERFACTORY_H

#include "RecommenderSystem.h"

class RSUsersLoader
{
private:
  static void get_users(const std::string& str,
            const std::vector<Movie>& movies_vector,
            std::vector<RSUser>& users_vector,
            std::shared_ptr<RecommenderSystem> rs);
  static void get_movies(std::string& str, std::vector<Movie>& movies_vector);

public:
    RSUsersLoader() = delete;
    /**
     *
     * loads users by the given format with their movie's ranks
     * @param users_file_path a path to the file of the users and their movie
     * ranks
     * @param rs RecommendingSystem for the Users
     * @return vector of the users created according to the file
     */
    static std::vector<RSUser> create_users_from_file
    (const std::string& users_file_path,
     std::shared_ptr<RecommenderSystem> rs) noexcept(false);
};

#endif //SCHOOL_SOLUTION_USERFACTORY_H
