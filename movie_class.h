#ifndef C__PROJECTS_MOVIE_CLASS_H
#define C__PROJECTS_MOVIE_CLASS_H

#include <string>
using namespace std;
class Movie {
private:
    int movieId;
    string movieName;
    double rating;

public:
    //Default constructor
    Movie() : movieId(0), movieName(""), rating(0.0) {}
    // Constructor
    Movie(int id, const string& name, double rating)
            : movieId(id), movieName(name), rating(rating) {}

    // Getter for movie ID
    int getMovieId() const {
        return movieId;
    }

    // Setter for movie ID
    void setMovieId(int id) {
        movieId = id;
    }

    // Getter for movie name
    string getMovieName() const {
        return movieName;
    }

    // Setter for movie name
    void setMovieName(const string& name) {
        movieName = name;
    }

    // Getter for rating
    double getRating() const {
        return rating;
    }

    // Setter for rating
    void setRating(double rating) {
        this->rating = rating;
    }
};

#endif //C__PROJECTS_MOVIE
