#include <iostream>
#include <vector>
#include <thread>
#include "Product.h"
#include "Logger.h"
#include "IAdapter.h"
#include "WooCommerceAdapter.h"
#include "CSVParser.h"

int main() {

    Logger::instance().log("========================================");
    Logger::instance().log("   SPAS - Smart Product Automation System");
    Logger::instance().log("========================================");
    Logger::instance().log("System starting...");

    // Step 1: Load products from CSV file
    CSVParser parser;
    std::vector<Product> products = parser.parse("C:\\Users\\samiela\\Desktop\\SPAS Project\\bin\\Debug\\products.csv");

    if (products.empty()) {
        Logger::instance().log("No products found. Exiting.");
        return 1;
    }

    Logger::instance().log("Products loaded: " + std::to_string(products.size()));

    // Step 2: Defining target websites
    std::vector<WooCommerceAdapter> adapters = {
        WooCommerceAdapter("https://builderschoice.co.ke",
                           "ck_demo_key_1", "cs_demo_secret_1"),
        WooCommerceAdapter("https://plugtech.co.ke",
                           "ck_demo_key_2", "cs_demo_secret_2"),
        WooCommerceAdapter("https://novatech.co.ke",
                           "ck_demo_key_3", "cs_demo_secret_3")
    };

    Logger::instance().log("Target sites: " +
                           std::to_string(adapters.size()));

    // Step 3: Uploading to each site
    for (auto& adapter : adapters) {

        adapter.connect();

        for (const auto& product : products) {
            UploadResult result = adapter.upload(product);

            if (result.success) {
                Logger::instance().log(
                    "SUCCESS | " + adapter.siteName() +
                    " | " + product.name +
                    " | ID: " + result.remoteId);
            } else {
                Logger::instance().log(
                    "FAILED  | " + adapter.siteName() +
                    " | " + product.name +
                    " | " + result.message);
            }
        }

        adapter.disconnect();
    }

    Logger::instance().log("========================================");
    Logger::instance().log("All uploads complete.");
    Logger::instance().log("========================================");

    return 0;
}
