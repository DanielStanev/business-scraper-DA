#include <iostream>
#include <string>
#include "MapScraper.h"
#include "Business.h"

int main(int argc, char** argv) {
    std::string api_key, keyword, location;
    int max_radius, max_results;

    // Get user input
    std::cout << "Enter Google Maps API key: ";
    std::getline(std::cin, api_key);

    std::cout << "Enter search keyword (e.g., 'restaurants', 'coffee shops'): ";
    std::getline(std::cin, keyword);

    std::cout << "Enter location (e.g., 'New York, NY'): ";
    std::getline(std::cin, location);

    std::cout << "Enter max radius in meters (default 5000): ";
    std::string radius_input;
    std::getline(std::cin, radius_input);
    max_radius = radius_input.empty() ? 5000 : std::stoi(radius_input);

    std::cout << "Enter max number of results (default 20): ";
    std::string results_input;
    std::getline(std::cin, results_input);
    max_results = results_input.empty() ? 20 : std::stoi(results_input);

    // Create and configure MapScraper
    MapScraper scraper(api_key);
    scraper.set_keyword(keyword);
    scraper.set_location(location);
    scraper.set_max_radius(max_radius);
    scraper.set_max_results(max_results);

    std::cout << "\nSearching for businesses..." << std::endl;

    // Search for businesses
    std::vector<Business> businesses = scraper.search_businesses();

    // Display results
    if (businesses.empty()) {
        std::cout << "No businesses found." << std::endl;
    } else {
        std::cout << "\nFound " << businesses.size() << " businesses:\n" << std::endl;

        for (size_t i = 0; i < businesses.size(); ++i) {
            const Business& business = businesses[i];
            std::cout << "--- Business " << (i + 1) << " ---" << std::endl;
            std::cout << "Name: " << business.name() << std::endl;
            std::cout << "Address: " << business.address() << std::endl;
            std::cout << "Phone: " << business.phone_number() << std::endl;
            std::cout << "Website: " << business.website() << std::endl;
            std::cout << "Rating: " << business.rating() << " (" << business.total_ratings() << " reviews)" << std::endl;
            std::cout << std::endl;
        }
    }

    return 0;
}