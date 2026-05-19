from google import genai

client = genai.Client(api_key="GEMINI_KEY")

def generate_seo(product_name, model_code):
    prompt = f"""
    Write a professional WooCommerce product description for:
    Product: {product_name} ({model_code})

    Include exactly:
    - short_description: 5 key benefits as HTML bullet points using <ul><li> tags
    - description: 350 words minimum, structured with H2 headings in HTML format
    - focus_keyphrase: 4 words specific to this product
    - meta_description: between 145 and 155 characters, starts with the keyphrase

    Return only a JSON object with keys: short_description, description, focus_keyphrase, meta_description.
    No markdown, no backticks, just raw JSON.
    """

    response = client.models.generate_content(
        model="gemini-2.0-flash",
        contents=prompt
    )

    import json
    try:
        return json.loads(response.text)
    except:
        # Fallback if JSON parsing fails
        return {
            "short_description": f"<ul><li>High performance {product_name}</li><li>Plug-and-play installation</li><li>Business grade reliability</li><li>Energy efficient design</li><li>Compatible with all major standards</li></ul>",
            "description": f"<h2>Overview</h2><p>The {product_name} ({model_code}) delivers reliable performance for business environments.</p>",
            "focus_keyphrase": f"{model_code} Kenya",
            "meta_description": f"Buy {product_name} ({model_code}) in Kenya. Available now at Tech Depot."
        }

# Test
if __name__ == "__main__":
    result = generate_seo("D-Link 8-Port Gigabit Unmanaged Desktop Switch", "DGS-108/B")
    for key, value in result.items():
        print(f"\n{key.upper()}:\n{value}")
