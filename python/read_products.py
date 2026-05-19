import openpyxl
import re
import csv
from datetime import datetime

def detect_brand(product_name):
    # Clean the product name
    name = product_name.strip()
    
    # Extract first meaningful capitalized word as brand
    words = name.split()
    noise_words = ['the', 'a', 'an', 'port', 'gigabit', 'fast', 'ethernet',
                   'switch', 'router', 'cable', 'patch', 'cord', 'unmanaged',
                   'managed', 'smart', 'desktop', 'indoor', 'outdoor', 
                   'wireless', 'power', 'table', 'top', 'metal', 'black',
                   'white', 'silver', 'to', 'with', 'and', 'for', 'in']
    
    for word in words:
        clean = re.sub(r'[^a-zA-Z\-]', '', word)
        if clean.lower() not in noise_words and len(clean) > 2:
            # Normalize common variations
            normalized = clean.lower()
            if 'dlink' in normalized or 'd-link' in normalized:
                return 'D-Link'
            if 'mikrotik' in normalized:
                return 'MikroTik'
            return clean
    return 'Unknown'

def is_junk_row(product_name, price):
    # Skip rows that are clearly headers or junk
    junk_phrases = ['price', 'distribution unit', 'pop-up', 'converter']
    for phrase in junk_phrases:
        if phrase.lower() in product_name.lower():
            return True
    if str(price) == 'Price':
        return True
    return False

def read_products(filename):
    wb = openpyxl.load_workbook(filename)
    ws = wb.active

    products = []
    current_category = 'Uncategorized'

    for row in ws.iter_rows(values_only=True):
        model_code = row[0]
        product_name = row[1]
        price = row[2]

        # Skip completely empty rows
        if not product_name:
            continue

        # Detect category header rows (no model code, no price)
        if not model_code and not price:
            current_category = str(product_name).strip()
            continue

        # Skip rows with no price
        if not price:
            continue

        product_name_str = str(product_name).strip()

        # Skip junk rows
        if is_junk_row(product_name_str, price):
            continue

        product = {
            'model_code': str(model_code).strip() if model_code else '',
            'product_name': product_name_str,
            'price': price,
            'category': current_category,
            'brand': detect_brand(product_name_str),
            'status': 'Pending'
        }
        products.append(product)

    return products

def save_report(products, filename):
    with open(filename, 'w', newline='', encoding='utf-8') as f:
        writer = csv.DictWriter(f, fieldnames=[
            'model_code', 'product_name', 'price', 
            'category', 'brand', 'status'
        ])
        writer.writeheader()
        writer.writerows(products)
    print(f"Report saved to {filename}")

# Run
products = read_products("products.xlsx")

print("\n===== PRODUCTS READ =====")
for p in products:
    print(f"[{p['category']}] {p['brand']} | {p['model_code']} | {p['product_name']} | KSh {p['price']}")

print(f"\nTotal products found: {len(products)}")

# Save report
timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
report_name = f"upload_report_{timestamp}.csv"
save_report(products, report_name)
