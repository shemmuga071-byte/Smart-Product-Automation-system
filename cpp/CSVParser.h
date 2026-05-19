#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Product.h"
#include "Logger.h"

// Reads product data from a CSV file
// Demonstrates: file handling, abstraction, single responsibility
class CSVParser {
private:
    // Handles quoted fields containing commas
    std::vector<std::string> parseLine(const std::string& line) {
        std::vector<std::string> fields;
        std::string field;
        bool inQuotes = false;

        for (size_t i = 0; i < line.size(); i++) {
            char c = line[i];
            if (c == '"') {
                inQuotes = !inQuotes;
            } else if (c == ',' && !inQuotes) {
                fields.push_back(field);
                field.clear();
            } else {
                field += c;
            }
        }
        fields.push_back(field);
        return fields;
    }

public:
    // Parses the CSV file and returns a vector of Product objects
    std::vector<Product> parse(const std::string& filename) {
        std::vector<Product> products;
        std::ifstream file(filename);

        if (!file.is_open()) {
            Logger::instance().log("ERROR: Could not open file: " + filename);
            return products;
        }

        std::string line;
        bool firstLine = true;

        while (std::getline(file, line)) {
            // Skip header row
            if (firstLine) {
                firstLine = false;
                continue;
            }

            // Skip empty lines
            if (line.empty()) continue;

            std::vector<std::string> fields = parseLine(line);

            // Need at least 5 fields to be a valid product
            if (fields.size() < 5) continue;

            Product p;
            p.sku             = fields[0];
            p.name            = fields[1];
            p.price           = fields[2].empty() ? 0.0 : std::stod(fields[2]);
            p.category        = fields[3];
            p.brand           = fields[4];
            p.description     = "";
            p.shortDescription  = "";
            p.focusKeyphrase  = "";
            p.metaDescription = "";

            products.push_back(p);
        }

        file.close();
        Logger::instance().log("Parsed " + std::to_string(products.size()) +
                               " products from " + filename);
        return products;
    }
};

#endif // CSVPARSER_H
