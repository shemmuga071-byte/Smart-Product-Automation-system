# SPAS — Smart Product Automation System

A C++ programming project demonstrating object-oriented design principles 
through a real-world product upload automation system.

## Project Structure
c++ core architecture
python implementation layer

## C++ Layer

Implements the system architecture using core OOP concepts:

- **Adapter Pattern** — `IAdapter` defines the interface; `WooCommerceAdapter` implements it
- **Encapsulation** — `Product` struct bundles all product data
- **Inheritance** — `WooCommerceAdapter` extends `IAdapter`
- **Polymorphism** — engine operates uniformly across all site adapters
- **Singleton Pattern** — `Logger` maintains one shared instance
- **File Handling** — `CSVParser` reads and parses product CSV files

## Python Layer

Operational implementation that runs against live WooCommerce sites:

- Reads supplier Excel files automatically
- Detects product categories and brands from file structure
- Generates SEO-optimised product descriptions
- Uploads products to WooCommerce via REST API
- Saves a timestamped upload report after each run

## How to Run

**C++ layer** — open `cpp/` project in Code Blocks and run

**Python layer:**
```bash
pip install openpyxl requests google-genai
py read_products.py
py seo_generator.py
py upload_products.py
```

## Language

C++17 · Python 3.13
