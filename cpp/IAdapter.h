#ifndef IADAPTER_H
#define IADAPTER_H

#include <string>
#include "Product.h"

// Abstract interface — every site adapter must implement these methods
// This is the core of the adapter pattern and polymorphism in SPAS
class IAdapter {
public:

    // Authenticate with the target website
    virtual bool connect() = 0;

    // Upload one product to the site — returns the result
    virtual UploadResult upload(const Product& p) = 0;

    // Clean logout from the site
    virtual void disconnect() = 0;

    // Returns the site name for logging purposes
    virtual std::string siteName() const = 0;

    // Virtual destructor — required for abstract base classes
    virtual ~IAdapter() {}
};

#endif // IADAPTER_H
