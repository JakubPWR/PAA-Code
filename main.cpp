#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "movie_class.h"
#include <ctime>
#include <algorithm>
#include <numeric>
#include <map>
using namespace std;

void calculateMeanAndMedian(const std::vector<Movie>& movies) {
    // Calculate mean
    double mean = 0.0;
    for (const auto& movie : movies)
    {
        double rating = movie.getRating();
        if (std::isnan(rating))
        {
            continue;
        }
        else
        {
            mean += movie.getRating();
        }
    }
    mean /= movies.size();
    std::cout << "Mean rating: " << mean << std::endl;

    // Calculate median
    std::vector<double> ratings;
    for (const auto& movie : movies) {
        ratings.push_back(movie.getRating());
    }
    std::sort(ratings.begin(), ratings.end());
    double median = 0.0;
    if (movies.size() % 2 == 0) {
        median = (ratings[movies.size() / 2 - 1] + ratings[movies.size() / 2]) / 2.0;
    } else {
        median = ratings[movies.size() / 2];
    }
    std::cout << "Median rating: " << median << std::endl;
}

//template<typename Func, typename Arg>
//static void time_measurement(Func func, Arg arg, int start, int end) {
//    clock_t start_time = clock();
//    func(arg, start, end); // Call func with the argument and range
//    clock_t end_time = clock();
//    double elapsed_secs = double(end_time - start_time) / CLOCKS_PER_SEC;
//    std::cout << std::fixed << std::setprecision(9);
//    std::cout << "Time taken to sort elements: " << elapsed_secs << " seconds" << std::endl;
//}
void bucketSort(vector<Movie>& movies) {
    // Define a vector of vectors to hold the buckets
    vector<vector<Movie>> buckets(11); // Assuming ratings are in the range [0, 10]

    // Distribute movies into buckets based on their ratings
    for (const auto& movie : movies) {
        double rating = movie.getRating();
        int bucketIndex = static_cast<int>(rating);
        if (bucketIndex < 0 || bucketIndex >= 11) {
            cerr << "Error: Invalid bucket index for rating " << rating << endl;
            continue;
        }
        buckets[bucketIndex].push_back(movie);
    }

    // Sort individual buckets
    for (auto& bucket : buckets) {
        sort(bucket.begin(), bucket.end(), [](const Movie& a, const Movie& b) {
            return a.getRating() < b.getRating();
        });
    }

    // Concatenate sorted buckets back into the original vector
    movies.clear();
    for (const auto& bucket : buckets) {
        movies.insert(movies.end(), bucket.begin(), bucket.end());
    }

}

void merge(std::vector<Movie>& movies, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<Movie> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = movies[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = movies[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].getRating() <= R[j].getRating()) {
            movies[k] = L[i];
            ++i;
        } else {
            movies[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        movies[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        movies[k] = R[j];
        ++j;
        ++k;
    }
}

void mergeSort(std::vector<Movie>& movies, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(movies, left, mid);
        mergeSort(movies, mid + 1, right);
        merge(movies, left, mid, right);
    }
}
int partition(std::vector<Movie>& movies, int low, int high) {
    double pivot = movies[low + (high - low) / 2].getRating(); // Choosing pivot as middle element
    int i = low - 1;
    int j = high + 1;
    while (true) {
        do {
            ++i;
        } while (movies[i].getRating() < pivot);

        do {
            --j;
        } while (movies[j].getRating() > pivot);

        if (i >= j)
            return j;

        std::swap(movies[i], movies[j]);
    }
}

void quickSort(std::vector<Movie>& movies, int low, int high) {
    if (low < high) {
        int pi = partition(movies, low, high);
        quickSort(movies, low, pi);
        quickSort(movies, pi + 1, high);
    }
}

void quickSort(std::vector<Movie>& movies) {
    quickSort(movies, 0, movies.size() - 1);
}


void removeMoviesWithNaN(std::vector<Movie>& movies) {
    movies.erase(std::remove_if(movies.begin(), movies.end(), [](const Movie& movie) {
        return std::isnan(movie.getRating());
    }), movies.end());
}

int main() {
    string data_path = "/Users/jakubgodlewski/Desktop/C++/c++projects/PAA-dane.csv";

    ifstream allMovies(data_path);

    if(allMovies.fail())
    {
        cout<<"Failed to open file"<<endl;
        return 1;
    }
    vector<Movie> movies;

    string line;
    clock_t start = clock();
    while (getline(allMovies, line)) {
        stringstream ss(line);
        string token, movieIdStr, movieName, ratingStr;

        // Read movie information from CSV
        getline(ss, movieIdStr, ','); // Read movie ID
        int movieId;
        try {
            movieId = stoi(movieIdStr);
        } catch (const std::invalid_argument& e) {
            cout << "Invalid movie ID: " << movieIdStr << endl;
            continue; // Skip this line and continue with the next one
        }

        getline(ss, token, ','); // Read movie name (including commas)
        movieName = token;

        // Read the rest of the line as the rating
        getline(ss, ratingStr);
        double rating = 0.0;
        if (!ratingStr.empty()) {
            try {
                rating = stod(ratingStr);
            } catch (const std::invalid_argument& e) {
                cout << "Invalid rating for movie: " << movieName << endl;
                continue; // Skip this line and continue with the next one
            }
        }

        // Check if the movie has a valid name and rating
        if (!movieName.empty()) {
            // Create a Movie object and add it to the vector
            Movie movie(movieId, movieName, rating);
            movies.push_back(movie);
        }
    }
    clock_t end = clock();
    double elapsed_secs = double(end - start) / CLOCKS_PER_SEC;
    cout << fixed << setprecision(9);
    cout << "Time taken to select data" << elapsed_secs << " seconds" << endl;


    std::vector<Movie> movies_10000(movies.begin(), movies.begin() + 10000);
    std::vector<Movie> movies_100000(movies.begin(), movies.begin() + 100000);
    std::vector<Movie> movies_500000(movies.begin(), movies.begin() + 500000);
    removeMoviesWithNaN(movies_10000);
    removeMoviesWithNaN(movies_100000);
    removeMoviesWithNaN(movies_500000);
    removeMoviesWithNaN(movies);

    clock_t start1 = clock();
    quickSort(movies_10000,0,movies_10000.size()-1);
    clock_t end1 = clock();
    double elapsed_secs1 = double(end1 - start1) / CLOCKS_PER_SEC;
    cout << fixed << setprecision(9);
    cout << "Time1 taken to sort elements " << elapsed_secs1 << " seconds" << endl;
    clock_t start2 = clock();
    quickSort(movies_100000,0,movies_100000.size()-1);
    clock_t end2 = clock();
    double elapsed_secs2 = double(end2 - start2) / CLOCKS_PER_SEC;
    cout << fixed << setprecision(9);
    cout << "Time2 taken to sort elements " << elapsed_secs2 << " seconds" << endl;
    clock_t start3 = clock();
    quickSort(movies_500000,0,movies_500000.size()-1);
    clock_t end3 = clock();
    double elapsed_secs3 = double(end3 - start3) / CLOCKS_PER_SEC;
    cout << fixed << setprecision(9);
    cout << "Time3 taken to sort elements " << elapsed_secs3 << " seconds" << endl;
    clock_t start4 = clock();
    quickSort(movies,0,movies.size()-1);
    clock_t end4 = clock();
    double elapsed_secs4 = double(end4 - start4) / CLOCKS_PER_SEC;
    cout << fixed << setprecision(9);
    cout << "Time4 taken to sort elements " << elapsed_secs4 << " seconds" << endl;
    //bucketsort
//    Time1 taken to sort elements 0.003450000 seconds
//    Time2 taken to sort elements 0.032982000 seconds
//    Time3 taken to sort elements 0.148429000 seconds
//    Time4 taken to sort elements 0.354872000 seconds
//    calculateMeanAndMedian(movies_10000);
//    calculateMeanAndMedian(movies_100000);
//    calculateMeanAndMedian(movies_500000);
//    calculateMeanAndMedian(movies);
    //QuickSort
//    Time1 taken to sort elements 0.008395000 seconds
//    Time2 taken to sort elements 0.104662000 seconds
//    Time3 taken to sort elements 0.597033000 seconds
//    Time4 taken to sort elements 1.145342000 seconds
    //MergeSort
//    Time1 taken to sort elements 0.034149000 seconds
//    Time2 taken to sort elements 0.390409000 seconds
//    Time3 taken to sort elements 2.265066000 seconds
//    Time4 taken to sort elements 4.411145000 seconds
    return 0;
}
