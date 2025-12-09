/**
 * @file json_helper.h
 *
 * @brief JSON Helper Functions API
 */

#ifndef JSON_HELPER_H
#define JSON_HELPER_H

/* Includes ------------------------------------------------------------------*/

#include "cJSON.h"
#include "esp_err.h"
#include <stdbool.h>
#include <stdint.h>

/* Exported functions --------------------------------------------------------*/

/**
 * @brief Create sensor data JSON string
 *
 * Format: {"timestamp": 1701388800, "temperature": 25.6, "humidity": 65.2, "light": 450, "interval": 30}
 *
 * @param[in] timestamp Unix timestamp in seconds
 * @param[in] temperature Temperature in Celsius
 * @param[in] humidity Humidity in percentage
 * @param[in] light Light level (lux)
 * @param[in] interval Measurement interval (seconds)
 *
 * @return JSON string (caller must free) or NULL on error
 */
char *json_helper_create_sensor_data(uint32_t timestamp, float temperature, float humidity, int light, int interval);

/**
 * @brief Create device status JSON string
 *
 * Format: {"timestamp": 1701388800, "online": true, "wifi_ssid": "MyWiFi", "wifi_rssi": -62,
 *          "ip": "192.168.1.100", "uptime": 86400, "heap_free": 245760, "firmware_version": "1.0.2"}
 *
 * @param[in] timestamp Unix timestamp in seconds
 * @param[in] online Device online status
 * @param[in] wifi_ssid WiFi SSID (can be NULL)
 * @param[in] wifi_rssi WiFi signal strength
 * @param[in] ip_address IP address string (can be NULL)
 * @param[in] uptime Uptime in seconds
 * @param[in] heap_free Free heap memory in bytes
 * @param[in] firmware_version Firmware version string
 *
 * @return JSON string (caller must free) or NULL on error
 */
char *json_helper_create_status(uint32_t timestamp, bool online, const char *wifi_ssid, int8_t wifi_rssi,
                                const char *ip_address, uint32_t uptime,
                                uint32_t heap_free, const char *firmware_version);

/**
 * @brief Create command response JSON string
 *
 * Format: {"timestamp": 1701388800, "cmd": "set_led", "status": "success", "message": "LED turned on"}
 *
 * @param[in] timestamp Unix timestamp in seconds
 * @param[in] cmd Command name
 * @param[in] success Success status (true = "success", false = "error")
 * @param[in] message Response message
 *
 * @return JSON string (caller must free) or NULL on error
 */
char *json_helper_create_response(uint32_t timestamp, const char *cmd, bool success, const char *message);

/**
 * @brief Parse command from JSON string
 *
 * Extracts command name and returns the parsed JSON object for further processing
 *
 * @param[in] json_str JSON string to parse
 * @param[out] cmd Output buffer for command name
 * @param[in] cmd_len Size of cmd buffer
 *
 * @return cJSON object (caller must cJSON_Delete) or NULL on error
 */
cJSON *json_helper_parse_command(const char *json_str, char *cmd, size_t cmd_len);

/**
 * @brief Get string value from JSON object safely
 *
 * @param[in] object JSON object
 * @param[in] key Key name
 * @param[in] default_val Default value if not found or not a string
 *
 * @return String value or default
 */
const char *json_helper_get_string(cJSON *object, const char *key, const char *default_val);

/**
 * @brief Get number value from JSON object safely
 *
 * @param[in] object JSON object
 * @param[in] key Key name
 * @param[in] default_val Default value if not found or not a number
 *
 * @return Number value or default
 */
double json_helper_get_number(cJSON *object, const char *key, double default_val);

/**
 * @brief Get integer value from JSON object safely
 *
 * @param[in] object JSON object
 * @param[in] key Key name
 * @param[in] default_val Default value if not found or not a number
 *
 * @return Integer value or default
 */
int json_helper_get_int(cJSON *object, const char *key, int default_val);

/**
 * @brief Get bool value from JSON object safely
 *
 * @param[in] object JSON object
 * @param[in] key Key name
 * @param[in] default_val Default value if not found or not a bool
 *
 * @return Bool value or default
 */
bool json_helper_get_bool(cJSON *object, const char *key, bool default_val);

/**
 * @brief Create WiFi scan result JSON array
 *
 * Format: [{"ssid": "Network1", "rssi": -45, "auth": 3}, ...]
 *
 * @param[in] ap_list Array of WiFi access point records
 * @param[in] ap_count Number of access points
 *
 * @return JSON string (caller must free) or NULL on error
 */
char *json_helper_create_wifi_scan_result(const void *ap_list, uint16_t ap_count);

/**
 * @brief Parse WiFi credentials from JSON
 *
 * Extracts SSID and password from JSON object {"ssid": "...", "password": "..."}
 *
 * @param[in] json_str JSON string to parse
 * @param[out] ssid_out Output buffer for SSID
 * @param[in] ssid_len Size of SSID buffer
 * @param[out] password_out Output buffer for password
 * @param[in] password_len Size of password buffer
 *
 * @return ESP_OK on success, ESP_ERR_INVALID_ARG if invalid
 */
esp_err_t json_helper_parse_wifi_credentials(const char *json_str,
                                             char *ssid_out, size_t ssid_len,
                                             char *password_out, size_t password_len);

/**
 * @brief Create WiFi status JSON
 *
 * Format: {"connected": true, "provisioned": true, "ip": "192.168.1.100", "rssi": -45}
 *
 * @param[in] connected WiFi connection status
 * @param[in] provisioned Provisioning status
 * @param[in] ip_address IP address string (can be NULL)
 * @param[in] rssi Signal strength (ignored if not connected)
 *
 * @return JSON string (caller must free) or NULL on error
 */
char *json_helper_create_wifi_status(bool connected, bool provisioned,
                                     const char *ip_address, int8_t rssi);

/**
 * @brief Create simple response JSON
 *
 * Format: {"status": "ok", "message": "Success"}
 *
 * @param[in] status Status string ("ok", "error", etc.)
 * @param[in] message Response message
 *
 * @return JSON string (caller must free) or NULL on error
 */
char *json_helper_create_simple_response(const char *status, const char *message);

#endif /* JSON_HELPER_H */