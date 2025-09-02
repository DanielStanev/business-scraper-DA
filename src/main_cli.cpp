#include <iostream>
#include <string>
#include <fstream>
#include <getopt.h>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <filesystem>

#include "MapScraper.h"
#include "Business.h"
#include "Formatter.h"

// Structure to hold program options
struct ProgramOptions {
    std::string keyword;
    std::string location;
    int max_radius = 5000;
    int max_results = 20;
    OutputFormat output_format = OutputFormat::CSV;
    bool show_help = false;
};

// Function declarations
void print_usage(const char* program_name);
std::string read_api_key_from_config();
bool parse_command_line(int argc, char** argv, ProgramOptions& options);
std::string get_timestamp_string();
std::string get_file_extension(OutputFormat format);
bool write_output_to_file(const std::string& content, const std::string& filename);

int main(int argc, char** argv) {
    ProgramOptions options;

    // Parse command line arguments
    if (!parse_command_line(argc, argv, options)) {
        return 1;
    }

    // Show help if requested
    if (options.show_help) {
        return 0;
    }

    // Read API key from config file
    std::string api_key = read_api_key_from_config();
    if (api_key.empty()) {
        return 1;
    }

    // Create and configure MapScraper
    MapScraper scraper(api_key, options.keyword, options.location, options.max_radius, options.max_results);

    std::cout << "Searching for '" << options.keyword << "' in '" << options.location << "'..." << std::endl;
    std::cout << "Max radius: " << options.max_radius << " meters" << std::endl;
    std::cout << "Max results: " << options.max_results << std::endl << std::endl;

    // Search for businesses
    std::vector<Business> businesses = scraper.search_businesses();

    // Format and save results
    Formatter formatter(options.output_format);

    if (businesses.empty()) {
        std::cout << "No businesses found." << std::endl;
        return 0;
    }

    // Generate output
    std::string formatted_output = formatter.format_businesses(businesses);

    // Create output filename with timestamp
    std::string timestamp = get_timestamp_string();
    std::string extension = get_file_extension(options.output_format);
    std::string filename = "output/" + timestamp + "." + extension;

    // Write to file
    if (write_output_to_file(formatted_output, filename)) {
        std::cout << "Found " << businesses.size() << " businesses." << std::endl;
        std::cout << "Results saved to: " << filename << std::endl;
    } else {
        std::cout << "Error saving results to file. Printing to console:" << std::endl;
        std::cout << formatted_output << std::endl;
    }    return 0;
}

// Function definitions
void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [OPTIONS]\n\n"
              << "Required options:\n"
              << "  -k, --keyword KEYWORD     Search keyword (e.g., 'restaurants', 'coffee shops')\n"
              << "  -l, --location LOCATION   Location to search (e.g., 'New York, NY')\n\n"
              << "Optional options:\n"
              << "  -d, --distance METERS     Max search radius in meters (default: 5000)\n"
              << "  -r, --results NUMBER      Max number of results (default: 20)\n"
              << "  -f, --format FORMAT       Output format: csv, json, yaml, xml (default: csv)\n"
              << "  -h, --help               Show this help message\n\n"
              << "The Google Maps API key should be configured in config.ini file.\n"
              << std::endl;
}

std::string read_api_key_from_config() {
    std::ifstream config_file("config.ini");
    if (!config_file.is_open()) {
        std::cerr << "Error: Could not open config.ini file. Please create it from config.template.ini" << std::endl;
        return "";
    }

    std::string line;
    while (std::getline(config_file, line)) {
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#' || line[0] == ';' || line[0] == '[') {
            continue;
        }

        // Look for the API key line
        size_t pos = line.find("google_maps_api_key=");
        if (pos != std::string::npos) {
            std::string api_key = line.substr(pos + 20); // Length of "google_maps_api_key="
            // Trim whitespace
            api_key.erase(0, api_key.find_first_not_of(" \t"));
            api_key.erase(api_key.find_last_not_of(" \t") + 1);
            return api_key;
        }
    }

    std::cerr << "Error: google_maps_api_key not found in config.ini" << std::endl;
    return "";
}

bool parse_command_line(int argc, char** argv, ProgramOptions& options) {
    // Define command line options
    static struct option long_options[] = {
        {"keyword",   required_argument, 0, 'k'},
        {"location",  required_argument, 0, 'l'},
        {"distance",  required_argument, 0, 'd'},
        {"results",   required_argument, 0, 'r'},
        {"format",    required_argument, 0, 'f'},
        {"help",      no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    int option_index = 0;
    int c;

    // Parse command line arguments
    while ((c = getopt_long(argc, argv, "k:l:d:r:f:h", long_options, &option_index)) != -1) {
        switch (c) {
            case 'k':
                options.keyword = optarg;
                break;
            case 'l':
                options.location = optarg;
                break;
            case 'd':
                options.max_radius = std::atoi(optarg);
                if (options.max_radius <= 0) {
                    std::cerr << "Error: Distance must be a positive number" << std::endl;
                    return false;
                }
                break;
            case 'r':
                options.max_results = std::atoi(optarg);
                if (options.max_results <= 0) {
                    std::cerr << "Error: Results must be a positive number" << std::endl;
                    return false;
                }
                break;
            case 'f': {
                std::string format_str = optarg;
                std::transform(format_str.begin(), format_str.end(), format_str.begin(), ::tolower);

                if (format_str == "csv") {
                    options.output_format = OutputFormat::CSV;
                } else if (format_str == "json") {
                    options.output_format = OutputFormat::JSON;
                } else if (format_str == "yaml") {
                    options.output_format = OutputFormat::YAML;
                } else if (format_str == "xml") {
                    options.output_format = OutputFormat::XML;
                } else {
                    std::cerr << "Error: Invalid format '" << optarg << "'. Valid formats are: csv, json, yaml, xml" << std::endl;
                    return false;
                }
                break;
            }
            case 'h':
                print_usage(argv[0]);
                options.show_help = true;
                return true;
            case '?':
                print_usage(argv[0]);
                return false;
            default:
                break;
        }
    }

    // Check required arguments
    if (options.keyword.empty() || options.location.empty()) {
        std::cerr << "Error: Both --keyword and --location are required\n" << std::endl;
        print_usage(argv[0]);
        return false;
    }

    return true;
}

std::string get_timestamp_string() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y%m%d_%H%M%S");
    return ss.str();
}

std::string get_file_extension(OutputFormat format) {
    switch (format) {
        case OutputFormat::CSV:
            return "csv";
        case OutputFormat::JSON:
            return "json";
        case OutputFormat::YAML:
            return "yml";
        case OutputFormat::XML:
            return "xml";
        default:
            return "csv";
    }
}

bool write_output_to_file(const std::string& content, const std::string& filename) {
    try {
        // Create output directory if it doesn't exist
        std::filesystem::create_directories("output");

        // Write content to file
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not create output file: " << filename << std::endl;
            return false;
        }

        file << content;
        file.close();

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error writing to file: " << e.what() << std::endl;
        return false;
    }
}