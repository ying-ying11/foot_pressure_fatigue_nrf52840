# Sarcopenia MCU
本專案透過韌體控制晶片開發板（nRF5340-DK）的12位元類比數位轉換器功能採樣肌肉信號傳感器（MyoWare）處理過後的EMG訊號，以及使用I2C功能與加速度陀螺儀模組（MPU6050）進行通訊，收集患者腳部的加速度與角加速度數據，資料採集完畢後利用nRF5340-DK內建的低功耗藍芽（Bluetooth Low Energy, BLE）將數據發送至手機的應用程式上。

# 安裝
## [教學影片](https://www.youtube.com/playlist?list=PLx_tBuQ_KSqEt7NK-H7Lu78lT2OijwIMl)
## 需要安裝的工具
* nRF Command Line Tools - [下載連結](https://bit.ly/2YgBGC5)
* nRF Connect SDK - [下載連結](https://bit.ly/39Tm3my)
* Visual Studio Code - [下載連結](https://code.visualstudio.com/download)

# ADC
### 匯入的程式庫：
* drivers/adc.h
* hal/nrf_saadc.h
### 主要使用的function：
* **adc_channel_setup**  
利用adc_channel_cfg，設定使用的adc功能

* **adc_read**  
利用adc_sequence，讀取adc資料

### adc_channel_cfg物件：
* **gain**  
設定ADC增益

* **reference**  
設定參考電壓

* **acquisition_time**  
設定採集時間(並不是採樣的頻率)

* **channel_id**  
設定採樣使用的channel

### adc_sequence物件：
* **options**  
採樣時使用的設定adc_sequence_options

* **channels**  
採樣時使用的通道

* **buffer**  
採樣時的儲存位址

* **buffer_size**  
儲存位置的大小

* **resolution**  
採樣的解析度

詳細資料可以參考[Zyphyr ADC](https://developer.nordicsemi.com/nRF_Connect_SDK/doc/latest/zephyr/hardware/peripherals/adc.html)

# I2C
### 匯入的程式庫：
* drivers/i2c.h
### 主要使用的function：
* **device_is_ready**  
確認裝置可以使用

* **i2c_reg_read_byte**  
讀取i2c的byte資料

* **i2c_reg_write_byte**  
寫入i2c的byte資料

# BLE
### 匯入的程式庫：
* settings/settings.h
* bluetooth/bluetooth.h
* bluetooth/conn.h
* bluetooth/gatt.h
* bluetooth/uuid.h
### 主要使用的function：
* **bt_enable**  
啟動藍芽功能

* **settings_load**  
讀取設定

* **bt_le_adv_start**  
利用兩個bt_data廣播資訊-ad和掃描回應資訊-sd，開始藍芽廣播

* **bt_gatt_cb_register**  
利用bt_gatt_cb設定GATT的回呼函式，在不同的GATT事件下會觸發

* **bt_gatt_notify**  
發送notification

## BLE服務設定
* **BT_GATT_SERVICE_DEFINE**  
定義GATT服務內容

* **BT_GATT_PRIMARY_SERVICE**  
利用UUID設定GATT主要服務

* **BT_GATT_CHARACTERISTIC**  
設定GATT特徵，參數順序為
> 1. UUID
> 2. 功能 (變數開頭為 **BT_GATT_CHRC**)
> 3. 權限 (變數開頭為 **BT_GATT_PERM**)
> 4. 讀取功能回呼函式
> 5. 寫入功能回呼函式
> 6. 數值位址

* **BT_GATT_CCC**  
宣告Client Characteristic Configuration

可以使用Nordic開發的手機APP [nRF Connect](https://play.google.com/store/apps/details?id=no.nordicsemi.android.mcp&hl=zh_TW&gl=US) 觀看開發結果  
詳細資料可以參考[Zephyr GAP](https://docs.zephyrproject.org/latest/doxygen/html/group__bt__gap.html)和[Zephyr GATT](https://docs.zephyrproject.org/latest/doxygen/html/group__bt__gatt.html)

# 實用函式庫
[Zephyr 時間函式庫](https://docs.zephyrproject.org/latest/doxygen/html/time__units_8h.html)  
[Zephyr 信號量函式庫](https://docs.zephyrproject.org/latest/doxygen/html/group__semaphore__apis.html)  
[Zephyr 執行緒函式庫](https://docs.zephyrproject.org/latest/doxygen/html/group__thread__apis.html)  

# 參考資料
Nordic 官方文件 - [連結](https://infocenter.nordicsemi.com/index.jsp)  
Zephyr 官方文件 - [連結](https://docs.zephyrproject.org/latest/)