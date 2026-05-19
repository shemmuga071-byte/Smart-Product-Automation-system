import requests
from requests.auth import HTTPBasicAuth
from read_products import read_products
from seo_generator import generate_seo
import urllib3

urllib3.disable_warnings()

# Site credentials
SITE_URL = "https://techdepot.co.ke"
CONSUMER_KEY = "consumer_key"
CONSUMER_SECRET = "consumer_secret key"

auth = HTTPBasicAuth(CONSUMER_KEY, CONSUMER_SECRET)

def get_or_create_category(category_name):
    url = f"{SITE_URL}/wp-json/wc/v3/products/categories"
    response = requests.get(url, auth=auth, params={"search": category_name}, verify=False)
    results = response.json()
    if results:
        return results[0]['id']
    response = requests.post(url, auth=auth, json={"name": category_name}, verify=False)
    return response.json()['id']

def get_or_create_brand(brand_name):
    if not brand_name:
        return None
    url = f"{SITE_URL}/wp-json/wc/v3/products/brands"
    response = requests.get(url, auth=auth, params={"search": brand_name}, verify=False)
    results = response.json()
    if isinstance(results, list) and results:
        return results[0]['id']
    response = requests.post(url, auth=auth, json={"name": brand_name}, verify=False)
    data = response.json()
    return data.get('id')

def upload_product(product):
    print(f"Uploading: {product['product_name']}...")

    # Resolve category and brand
    category_id = get_or_create_category(product['category'])
    brand_id = get_or_create_brand(product.get('brand'))

    # Generate SEO content
    seo = generate_seo(product['product_name'], product['model_code'])

    # Build product title
    title = f"{product['product_name']} ({product['model_code']})" \
        if product['model_code'] else product['product_name']

    data = {
        "name": title,
        "type": "simple",
        "regular_price": str(product['price']),
        "sku": product['model_code'],
        "categories": [{"id": category_id}],
        "description": seo['description'],
        "short_description": seo['short_description'],
        "manage_stock": True,
        "stock_quantity": 10,
        "status": "publish"
    }

    if brand_id:
        data["brands"] = [{"id": brand_id}]

    # Create product
    url = f"{SITE_URL}/wp-json/wc/v3/products"
    response = requests.post(url, auth=auth, json=data, verify=False)

    if response.status_code == 201:
        product_id = response.json()['id']
        print(f"  SUCCESS — Product ID: {product_id}")
        return True, product_id
    else:
        print(f"  FAILED — {response.status_code}: {response.text[:100]}")
        return False, None

# Run — products for testing unlimited
products = read_products("products.xlsx")

print(f"\nUploading {len(products)} products to {SITE_URL}\n")

success_count = 0
fail_count = 0

for product in products:
    success, pid = upload_product(product)
    if success:
        success_count += 1
    else:
        fail_count += 1

print(f"\n===== UPLOAD COMPLETE =====")
print(f"Success: {success_count}")
print(f"Failed:  {fail_count}")
print(f"Total:   {len(products)}")
