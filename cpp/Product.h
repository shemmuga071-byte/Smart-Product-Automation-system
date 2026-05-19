#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <map>

// Stores the result of uploading one product to one site
struct UploadResult {
    bool        success;
    std::string remoteId;
    std::string message;
};

// Represents one product from the Excel/CSV file
struct Product {
    // Identity
    std::string sku;
    std::string name;
    std::string brand;
    std::string category;

    // Pricing
    double price;

    // SEO content (AI generated)
    std::string description;
    std::string shortDescription;
    std::string focusKeyphrase;
    std::string metaDescription;

    // Upload results per site — populated after upload
    std::map<std::string, UploadResult> results;
};

#endif // PRODUCT_H
