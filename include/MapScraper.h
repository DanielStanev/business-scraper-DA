#ifndef MAPSCRAPER_H
#define MAPSCRAPER_H

#include <string>
#include <vector>
#include "Business.h"

class MapScraper {
private:
    std::string m_api_key;
    std::string m_keyword;
    std::string m_location;
    int m_max_radius;
    int m_max_results;

    // Helper methods
    std::string build_search_url() const;
    std::string make_http_request(const std::string& url) const;
    std::vector<Business> parse_response(const std::string& json_response) const;
    Business parse_business_details(const std::string& place_id) const;

public:
    MapScraper();
    MapScraper(const std::string& api_key);
    ~MapScraper();

    // Setters
    void set_api_key(const std::string& api_key) { m_api_key = api_key; }
    void set_keyword(const std::string& keyword) { m_keyword = keyword; }
    void set_location(const std::string& location) { m_location = location; }
    void set_max_radius(int radius) { m_max_radius = radius; }
    void set_max_results(int results) { m_max_results = results; }

    // Getters
    std::string api_key() const { return m_api_key; }
    std::string keyword() const { return m_keyword; }
    std::string location() const { return m_location; }
    int max_radius() const { return m_max_radius; }
    int max_results() const { return m_max_results; }

    // Main functionality
    std::vector<Business> search_businesses();
};

#endif