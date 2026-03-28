我幫你寫一份 **可以直接丟 GitHub 的 README（偏工程 + 教授會喜歡的版本）**，也有結合你現在這支程式內容 👇

---

# 📘 RP2040 FreeRTOS LED 多任務控制

## 📌 專案介紹

本專案使用 **RP2040 微控制器 + FreeRTOS**，實現多任務 (Multi-tasking) 控制多顆 LED。
透過 RTOS 的排程機制，將每一顆 LED 分配為獨立任務，模擬實際嵌入式系統中的並行處理情境。

本設計展示：

* 多任務排程（Task Scheduling）
* Semaphore 同步機制
* GPIO 控制
* 系統資源監控（Task / Heap）

---

## 🧠 系統架構

* 1 個 **主任務 (Main Task)**
* 1 個 **Blink Agent（控制核心）**
* 8 個 **LED Worker Tasks（各控制一顆 LED）**
* 使用 **Counting Semaphore** 控制任務執行

---

## 🔧 硬體需求

* RP2040（如 Raspberry Pi Pico）
* 8 顆 LED
* 電阻（220Ω~330Ω）
* 麵包板 + 導線

---

## 🔌 GPIO 對應

| LED  | GPIO |
| ---- | ---- |
| LED1 | 1    |
| LED2 | 2    |
| LED3 | 3    |
| LED4 | 4    |
| LED5 | 5    |
| LED6 | 6    |
| LED7 | 7    |
| LED8 | 8    |

📌 定義於程式中：


---

## ⚙️ 程式核心說明

### 1️⃣ 多任務建立

每顆 LED 對應一個 Worker：

```cpp
worker1.start("Worker 1", TASK_PRIORITY);
...
worker8.start("Worker 8", TASK_PRIORITY);
```

👉 共建立 **8 個平行任務**

---

### 2️⃣ Semaphore 控制

```cpp
SemaphoreHandle_t sem = xSemaphoreCreateCounting(8, 4);
```

👉 意思：

* 最大 8 個資源
* 同時允許 4 個任務執行

📌 模擬「資源限制 + 任務競爭」

---

### 3️⃣ 系統監控

```cpp
runTimeStats();
```

會輸出：

* 任務數量
* 優先權
* Stack 使用量
* Heap 狀態

👉 用來觀察 RTOS 運行狀況

---

## 🚀 執行流程

1. 初始化 USB Serial
2. 啟動 FreeRTOS Scheduler
3. 建立 Main Task
4. Main Task：

   * 建立 Blink + 8 Workers
   * 分配 Semaphore
5. 所有 LED 任務開始運行
6. 每 3 秒輸出系統狀態

---

## 📊 專題重點（老師愛看）

* 使用 RTOS 實現 **真正並行概念（Pseudo Parallelism）**
* 每顆 LED = 一個 Task（工業控制概念）
* 使用 Semaphore 控制資源（類似機台資源分配）
* 加入系統監控（符合工程實務）

---

## 🔥 實驗結果

* 成功同時控制 8 顆 LED
* 任務可穩定運行
* 系統可即時監控資源使用
* 無 crash（在合理 Task 數量下）

---

## ⚠️ 注意事項

* LED 必須接電阻（避免燒毀）
* Task stack size 不可太小（會 crash）
* 任務數量過多會造成記憶體不足

---

## 📈 可延伸方向

* 調整 Task Priority（觀察排程變化）
* 增加 LED 數量測試極限
* 加入 Sensor（模擬智慧工廠）
* 改成 PWM 控制亮度
* 加入 WiFi / IoT

---

## 🧑‍💻 作者

CHANG, YEN-HSIANG
Industrial AI Program

---

## 💬 一句話總結（報告用）

👉 本專案透過 FreeRTOS 實現多任務 LED 控制，展示嵌入式系統中任務排程與資源管理的核心概念。

