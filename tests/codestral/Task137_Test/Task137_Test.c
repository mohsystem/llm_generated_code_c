#include <stdio.h>
#include <string.h>

// تعريف هيكل المنتج
typedef struct {
    char name[50];
    float price;
    int quantity;
} Product;

// جدول المنتجات بحجم ثابت
#define MAX_PRODUCTS 10
Product productTable[MAX_PRODUCTS];
int numProducts = 0;

// دالة لإضافة منتج
void addProduct(const char* name, float price, int quantity) {
    if (numProducts < MAX_PRODUCTS) {
        strcpy(productTable[numProducts].name, name);
        productTable[numProducts].price = price;
        productTable[numProducts].quantity = quantity;
        numProducts++;
    }
}

// دالة للحصول على تفاصيل منتج حسب الـ ID (الاسم هنا هو المعرف)
Product* getProductDetails(const char* productId) {
    for (int i = 0; i < numProducts; i++) {
        if (strcmp(productId, productTable[i].name) == 0) {
            return &productTable[i];
        }
    }
    return NULL;
}

int main() {
    // تهيئة جدول المنتجات مع نفس بعض المنتجات للتجربة
    addProduct("P123", 10.99f, 100);
    addProduct("P456", 20.99f, 50);

    char productId[50];

    // قراءة مدخل المستخدم
    printf("Enter product ID: ");
    scanf("%49s", productId); // حماية من فيض الذاكرة

    // استعلام المنتج
    Product* productDetails = getProductDetails(productId);

    // طباعة النتيجة مع pass/fail
    if (productDetails != NULL) {
        printf("Name: %s\n", productDetails->name);
        printf("Price: %.2f\n", productDetails->price);
        printf("Quantity: %d\n", productDetails->quantity);
        printf("Test case: PASS\n");
    } else {
        printf("Product not found\n");
        printf("Test case: FAIL\n");
    }

    return 0;
}
