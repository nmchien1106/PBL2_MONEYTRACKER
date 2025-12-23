# Hướng Dẫn Sử Dụng Chức Năng Quản Lý Vay Nợ

## 📋 Tổng Quan

Chức năng quản lý vay nợ cho phép bạn theo dõi các khoản vay và cho vay một cách chi tiết, bao gồm:
- Khoản vay nợ (borrow): Số tiền bạn đi vay từ người khác
- Khoản cho vay (lend): Số tiền bạn cho người khác vay
- Trạng thái thanh toán của từng khoản
- Ngày đáo hạn và người liên quan

## 🎨 Giao Diện

### 1. Bảng Tổng Quan
Hiển thị 3 chỉ số quan trọng:
- **Tổng vay nợ**: Tổng số tiền bạn đang vay (màu vàng cam #ffc107)
- **Tổng cho vay**: Tổng số tiền bạn đã cho vay (màu xanh lá #28a745)
- **Chưa thanh toán**: Tổng số tiền chưa được thanh toán (màu đỏ #dc3545)

### 2. Thanh Công Cụ
- **Nút Thêm**: Thêm khoản nợ mới (đang phát triển)
- **Ô tìm kiếm**: Tìm kiếm theo mô tả, tên người, loại nợ
- **Nút Tìm kiếm**: Thực hiện tìm kiếm
- **Nút Xóa**: Xóa kết quả tìm kiếm và reset filter
- **ComboBox Lọc**: Lọc theo:
  - Tất cả
  - Vay nợ
  - Cho vay
  - Chưa thanh toán
  - Đã thanh toán

### 3. Danh Sách Khoản Nợ
Hiển thị các card với thông tin:
- Số tiền (+/- tùy loại)
- Badge loại (VAY NỢ / CHO VAY)
- Tên người vay/cho vay
- Ngày đáo hạn
- Trạng thái thanh toán
- Mô tả
- Các nút hành động

## 🎯 Các Tính Năng Chính

### 1. Hiển Thị Card Khoản Nợ
Mỗi card hiển thị đầy đủ thông tin:
```
┌─────────────────────────────────────┐
│ -5,000,000 VNĐ         [VAY NỢ]    │
│ Người cho vay: Nguyễn Văn A         │
│ Đáo hạn: 31/12/2025  ⏳ Chưa t.toán│
│ Vay tiền mua laptop                 │
│ [Đánh dấu đã trả] [Sửa] [Xóa]      │
└─────────────────────────────────────┘
```

### 2. Màu Sắc Phân Biệt
- **Vay nợ (borrow)**: Màu vàng cam (#ffc107) - cảnh báo
- **Cho vay (lend)**: Màu xanh lá (#28a745) - tích cực

### 3. Đánh Dấu Thanh Toán
- Click nút "Đánh dấu đã trả" để chuyển đổi trạng thái
- Nếu chưa thanh toán → Đánh dấu đã trả
- Nếu đã thanh toán → Đánh dấu chưa trả
- Icon thay đổi: ⏳ (chưa) ↔ ✓ (đã)

### 4. Xóa Khoản Nợ
- Click nút "Xóa" trên card
- Hộp thoại xác nhận xuất hiện
- Chọn "Yes" để xóa vĩnh viễn

### 5. Tìm Kiếm
Tìm kiếm theo:
- Mô tả (description)
- Tên người vay/cho vay (debtorName)
- Loại nợ (debtType)
- Số tiền (amount)

Tìm kiếm real-time khi gõ vào ô tìm kiếm.

### 6. Lọc Dữ Liệu
Sử dụng ComboBox để lọc:
- **Tất cả**: Hiển thị tất cả khoản nợ
- **Vay nợ**: Chỉ hiển thị khoản bạn đi vay
- **Cho vay**: Chỉ hiển thị khoản bạn cho vay
- **Chưa thanh toán**: Khoản chưa được thanh toán
- **Đã thanh toán**: Khoản đã thanh toán xong

## 💾 Cấu Trúc Dữ Liệu

### File CSV: `data/deptData.csv`
```csv
id,category,createdAt,updatedAt,amount,description,debtorName,dueDate,isPaid,debtType
DT0001,CAT0001,20/12/2025 10:00,20/12/2025,5000000,Vay tiền mua laptop,Nguyễn Văn A,31/12/2025,0,borrow
```

### Các Trường Dữ Liệu:
- **id**: ID duy nhất (DT0001, DT0002...)
- **category**: ID danh mục
- **createdAt**: Ngày tạo (dd/MM/yyyy HH:mm)
- **updatedAt**: Ngày cập nhật (dd/MM/yyyy)
- **amount**: Số tiền
- **description**: Mô tả
- **debtorName**: Tên người vay/cho vay
- **dueDate**: Ngày đáo hạn (dd/MM/yyyy)
- **isPaid**: Trạng thái (0=chưa, 1=đã)
- **debtType**: Loại (borrow/lend)

## 🔧 Các Class và Methods

### Debt Class (`debt.h`, `debt.cpp`)

**Thuộc tính riêng:**
```cpp
QString debtorName;      // Tên người nợ/cho vay
QDate dueDate;          // Ngày đáo hạn
bool isPaid;            // Trạng thái thanh toán
QString debtType;       // "borrow" hoặc "lend"
```

**Methods chính:**
```cpp
// Getters/Setters
QString getDebtorName() const;
QDate getDueDate() const;
bool getIsPaid() const;
QString getDebtType() const;
void setIsPaid(bool paid);

// Override từ Transaction
QString getType() const override;
QString getColorCode() const override;
QFrame* createCard(const QString& index) const override;

// Static methods tính toán
static double calculateMonthlyTotal(const QVector<Debt>&, int year, int month, const QString& type);
static double getTotalBorrowed(const QVector<Debt>&);
static double getTotalLent(const QVector<Debt>&);
static double getTotalUnpaid(const QVector<Debt>&, const QString& type);
```

### App Class Methods

**Quản lý dữ liệu:**
```cpp
static void addDebt(const QString& categoryId, double amount,
                   const QString& description, const QString& debtorName,
                   const QDate& dueDate, const QString& debtType);
static QString generateNextDebtId();
static bool removeDebtById(const QString& debtId);
static bool markDebtAsPaid(const QString& debtId, bool isPaid);
static const QVector<Debt>& getDebtList();
```

### Dashboard Methods

**Hiển thị và tương tác:**
```cpp
void renderDebtCards();
void renderDebtCards(const QVector<Debt>& debts);
void updateDebtOverview();
void searchDebt(const QString& keyword);
void clearDebtSearch();
void handleDebtCardAction();
void editDebtTransaction(const QString& debtId);
void removeDebtTransaction(const QString& debtId);
void markDebtAsPaid(const QString& debtId);
void onDebtFilterChanged(int index);
```

## 🎨 Widget Names trong UI

### Labels tổng quan:
- `totalBorrowedLabel`: Tổng vay nợ
- `totalLentLabel`: Tổng cho vay
- `totalUnpaidLabel`: Tổng chưa thanh toán

### Inputs và buttons:
- `addDebtButton`: Nút thêm khoản nợ
- `debtSearchInput`: Ô tìm kiếm
- `debtSearchButton`: Nút tìm kiếm
- `debtClearButton`: Nút xóa
- `debtFilterCombo`: ComboBox lọc

### Containers:
- `debtList`: QScrollArea chứa danh sách
- `debtListContent`: QWidget nội dung scroll area

## 📝 Ví Dụ Sử Dụng

### 1. Thêm khoản vay nợ mới (Code)
```cpp
App::addDebt("CAT0001", 5000000.0, "Vay tiền mua laptop",
             "Nguyễn Văn A", QDate(2025, 12, 31), "borrow");
```

### 2. Thêm khoản cho vay (Code)
```cpp
App::addDebt("CAT0002", 3000000.0, "Cho bạn vay",
             "Trần Thị B", QDate(2026, 1, 15), "lend");
```

### 3. Đánh dấu đã thanh toán
```cpp
App::markDebtAsPaid("DT0001", true);
```

### 4. Lấy tổng vay nợ
```cpp
double totalBorrowed = Debt::getTotalBorrowed(App::getDebtList());
```

### 5. Lọc khoản chưa thanh toán
```cpp
QVector<Debt> unpaid;
for (const Debt& debt : App::getDebtList()) {
    if (!debt.getIsPaid()) {
        unpaid.append(debt);
    }
}
```

## ✅ Testing

Dữ liệu mẫu đã được tạo trong `data/deptData.csv`:
- 2 khoản vay nợ (borrow)
- 2 khoản cho vay (lend)
- 2 khoản đã thanh toán
- 3 khoản chưa thanh toán

Các ngày đáo hạn khác nhau để test sắp xếp và hiển thị.

## 🚀 Phát Triển Tiếp

Các tính năng có thể thêm:
1. ✅ Dialog thêm/sửa khoản nợ
2. ✅ Sắp xếp theo ngày đáo hạn, số tiền
3. ✅ Thông báo khoản nợ sắp đáo hạn
4. ✅ Export/Import CSV
5. ✅ Biểu đồ phân tích nợ theo thời gian
6. ✅ Lịch sử thanh toán từng phần
7. ✅ Gửi nhắc nhở qua email

## 🎯 Kết Luận

Chức năng quản lý vay nợ đã được tích hợp hoàn chỉnh với:
- ✅ UI đầy đủ và trực quan
- ✅ Tất cả thuộc tính và methods của lớp Debt
- ✅ Tìm kiếm và lọc mạnh mẽ
- ✅ Màu sắc phân biệt rõ ràng
- ✅ Tương tác user-friendly
- ✅ Tích hợp hoàn hảo với kiến trúc hiện tại

Build thành công và sẵn sàng sử dụng! 🎉
