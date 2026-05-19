#ifndef WOOCOMMERCEADAPTER_H
#define WOOCOMMERCEADAPTER_H

#include <string>
#include "IAdapter.h"
#include "Product.h"
#include "Logger.h"

// Concrete adapter for WooCommerce sites
// Inherits from IAdapter and implements all pure virtual methods
// Demonstrates: inheritance, polymorphism, encapsulation
class WooCommerceAdapter : public IAdapter {
private:
    std::string siteUrl;
    std::string consumerKey;
    std::string consumerSecret;
    bool        connected;

public:
    // Constructor — takes site credentials
    WooCommerceAdapter(const std::string& url,
                       const std::string& key,
                       const std::string& secret)
        : siteUrl(url),
          consumerKey(key),
          consumerSecret(secret),
          connected(false) {}

    // Step 1 — Authenticate with WooCommerce REST API
    bool connect() override {
        Logger::instance().log("Connecting to " + siteUrl + "...");
        connected = true;
        Logger::instance().log("Connected to " + siteUrl);
        return true;
    }

    // Step 2 — Upload one product through the 5-step sequence
    UploadResult upload(const Product& p) override {
        UploadResult result;

        if (!connected) {
            result.success = false;
            result.message = "Not connected to " + siteUrl;
            return result;
        }

        Logger::instance().log("Uploading: " + p.name + " to " + siteUrl);

        // Five-step WooCommerce upload sequence
        Logger::instance().log("  Step 1 — Resolving category: " + p.category);
        Logger::instance().log("  Step 2 — Resolving brand: " + p.brand);
        Logger::instance().log("  Step 3 — Uploading product images...");
        Logger::instance().log("  Step 4 — Creating product via REST API...");
        Logger::instance().log("  Step 5 — Injecting Yoast SEO fields...");

        // Simulate successful upload
        result.success  = true;
        result.remoteId = "WC-" + p.sku;
        result.message  = "Product uploaded successfully";

        return result;
    }

    // Step 3 — Disconnect from site
    void disconnect() override {
        connected = false;
        Logger::instance().log("Disconnected from " + siteUrl);
    }

    // Returns site identifier for logging
    std::string siteName() const override {
        return siteUrl;
    }
};

#endif // WOOCOMMERCEADAPTER_H
