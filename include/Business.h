#ifndef BUSINESS_H
#define BUSINESS_H

#include <string>
#include <vector>


class Business {
private:

    int m_total_ratings;
    double m_rating;

    std::string m_name;
    std::string m_address;
    std::string m_phone_number;
    std::string m_email;
    std::string m_website;
    std::vector<std::string> m_additional_numbers;
    std::vector<std::string> m_additional_emails;
    std::vector<std::string> m_social_media_links;

public:

    Business();
    ~Business();

    // Accessors/Mutators
    void set_name(const std::string& name) { m_name = name; }
    std::string name() const { return m_name; }

    void set_address(const std::string& address) { m_address = address; }
    std::string address() const { return m_address; }

    void set_phone_number(const std::string& phone_number) { m_phone_number = phone_number; }
    std::string phone_number() const { return m_phone_number; }

    void set_email(const std::string& email) { m_email = email; }
    std::string email() const { return m_email; }

    void set_website(const std::string& website) { m_website = website; }
    std::string website() const { return m_website; }

    void set_additional_numbers(const std::vector<std::string>& numbers) { m_additional_numbers = numbers; }
    std::vector<std::string> additional_numbers() const { return m_additional_numbers; }

    void set_additional_emails(const std::vector<std::string>& emails) { m_additional_emails = emails; }
    std::vector<std::string> additional_emails() const { return m_additional_emails; }

    void set_social_media_links(const std::vector<std::string>& links) { m_social_media_links = links; }
    std::vector<std::string> social_media_links() const { return m_social_media_links; }

    void set_total_ratings(int total_ratings) { m_total_ratings = total_ratings; }
    int total_ratings() const { return m_total_ratings; }

    void set_rating(double rating) { m_rating = rating; }
    double rating() const { return m_rating; }
    
    // Utilities
    void add_additional_number(std::string& phone_number) { m_additional_numbers.push_back(phone_number); }
    void add_additional_email (std::string& email) { m_additional_emails.push_back(email); }
    void add_social_link (std::string& social_link) { m_social_media_links.push_back(social_link); }
};

#endif