/**
 * @file json_helper.c
 *
 * @brief JSON Helper Functions Implementation
 */

/* Includes ------------------------------------------------------------------*/

#include "json_helper.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include <string.h>
#include <time.h>

/* Private defines -----------------------------------------------------------*/

// JSON response status strings
#define JSON_STATUS_SUCCESS "success"
#define JSON_STATUS_ERROR "error"

/* Private variables ---------------------------------------------------------*/

static const char *TAG = "JSON_HELPER";

/* Exported functions --------------------------------------------------------*/

/**
 * @brief Create sensor data JSON string
 */
char *json_helper_create_sensor_data(uint32_t timestamp, float temperature, float humidity, int light, int interval)
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE(TAG, "Failed to create JSON object");
        return NULL;
    }

    cJSON_AddNumberToObject(root, "timestamp", timestamp);
    cJSON_AddNumberToObject(root, "temperature", temperature);
    cJSON_AddNumberToObject(root, "humidity", humidity);
    cJSON_AddNumberToObject(root, "light", light);
    cJSON_AddNumberToObject(root, "interval", interval);

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (json_str == NULL)
    {
        ESP_LOGE(TAG, "Failed to print JSON");
        return NULL;
    }

    return json_str;
}

/**
 * @brief Create device status JSON string
 */
char *json_helper_create_status(uint32_t timestamp, bool online, const char *wifi_ssid, int8_t wifi_rssi,
                                const char *ip_address, uint32_t uptime,
                                uint32_t heap_free, const char *firmware_version)
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE(TAG, "Failed to create JSON object");
        return NULL;
    }

    cJSON_AddNumberToObject(root, "timestamp", timestamp);
    cJSON_AddBoolToObject(root, "online", online);

    if (wifi_ssid != NULL)
    {
        cJSON_AddStringToObject(root, "wifi_ssid", wifi_ssid);
    }

    cJSON_AddNumberToObject(root, "wifi_rssi", wifi_rssi);

    if (ip_address != NULL)
    {
        cJSON_AddStringToObject(root, "ip", ip_address);
    }

    cJSON_AddNumberToObject(root, "uptime", uptime);
    cJSON_AddNumberToObject(root, "heap_free", heap_free);

    if (firmware_version != NULL)
    {
        cJSON_AddStringToObject(root, "firmware_version", firmware_version);
    }

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (json_str == NULL)
    {
        ESP_LOGE(TAG, "Failed to print JSON");
        return NULL;
    }

    return json_str;
}

/**
 * @brief Create command response JSON string
 */
char *json_helper_create_response(uint32_t timestamp, const char *cmd, bool success, const char *message)
{
    if (cmd == NULL || message == NULL)
    {
        ESP_LOGE(TAG, "Invalid parameters: cmd or message is NULL");
        return NULL;
    }

    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE(TAG, "Failed to create JSON object");
        return NULL;
    }

    cJSON_AddNumberToObject(root, "timestamp", timestamp);
    cJSON_AddStringToObject(root, "cmd", cmd);
    cJSON_AddStringToObject(root, "status", success ? JSON_STATUS_SUCCESS : JSON_STATUS_ERROR);
    cJSON_AddStringToObject(root, "message", message);

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (json_str == NULL)
    {
        ESP_LOGE(TAG, "Failed to print JSON");
        return NULL;
    }

    return json_str;
}

/**
 * @brief Parse command from JSON string
 */
cJSON *json_helper_parse_command(const char *json_str, char *cmd, size_t cmd_len)
{
    if (json_str == NULL || cmd == NULL || cmd_len == 0)
    {
        ESP_LOGE(TAG, "Invalid parameters");
        return NULL;
    }

    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        ESP_LOGE(TAG, "JSON parse error: %s", error_ptr ? error_ptr : "unknown");
        return NULL;
    }

    cJSON *cmd_item = cJSON_GetObjectItem(root, "cmd");
    if (!cJSON_IsString(cmd_item))
    {
        ESP_LOGE(TAG, "Command field not found or not a string");
        cJSON_Delete(root);
        return NULL;
    }

    strncpy(cmd, cmd_item->valuestring, cmd_len - 1);
    cmd[cmd_len - 1] = '\0';

    return root;
}

/**
 * @brief Get string value from JSON object safely
 */
const char *json_helper_get_string(cJSON *object, const char *key, const char *default_val)
{
    if (object == NULL || key == NULL)
    {
        return default_val;
    }

    cJSON *item = cJSON_GetObjectItem(object, key);
    if (cJSON_IsString(item))
    {
        return item->valuestring;
    }

    return default_val;
}

/**
 * @brief Get number value from JSON object safely
 */
double json_helper_get_number(cJSON *object, const char *key, double default_val)
{
    if (object == NULL || key == NULL)
    {
        return default_val;
    }

    cJSON *item = cJSON_GetObjectItem(object, key);
    if (cJSON_IsNumber(item))
    {
        return item->valuedouble;
    }

    return default_val;
}

/**
 * @brief Get integer value from JSON object safely
 */
int json_helper_get_int(cJSON *object, const char *key, int default_val)
{
    if (object == NULL || key == NULL)
    {
        return default_val;
    }

    cJSON *item = cJSON_GetObjectItem(object, key);
    if (cJSON_IsNumber(item))
    {
        return item->valueint;
    }

    return default_val;
}

/**
 * @brief Get bool value from JSON object safely
 */
bool json_helper_get_bool(cJSON *object, const char *key, bool default_val)
{
    if (object == NULL || key == NULL)
    {
        return default_val;
    }

    cJSON *item = cJSON_GetObjectItem(object, key);
    if (cJSON_IsBool(item))
    {
        return cJSON_IsTrue(item);
    }

    return default_val;
}

/**
 * @brief Create WiFi scan result JSON array
 */
char *json_helper_create_wifi_scan_result(const void *ap_list, uint16_t ap_count)
{
    if (ap_list == NULL)
    {
        ESP_LOGE(TAG, "Invalid parameter: ap_list is NULL");
        return NULL;
    }

    const wifi_ap_record_t *ap_records = (const wifi_ap_record_t *)ap_list;

    cJSON *root = cJSON_CreateArray();
    if (root == NULL)
    {
        ESP_LOGE(TAG, "Failed to create JSON array");
        return NULL;
    }

    for (int i = 0; i < ap_count; i++)
    {
        cJSON *item = cJSON_CreateObject();
        if (item == NULL)
        {
            ESP_LOGE(TAG, "Failed to create JSON object for AP %d", i);
            cJSON_Delete(root);
            return NULL;
        }

        // Ensure SSID is null-terminated (max 32 bytes + null)
        char ssid_str[33];
        memcpy(ssid_str, ap_records[i].ssid, 32);
        ssid_str[32] = '\0';

        cJSON_AddStringToObject(item, "ssid", ssid_str);
        cJSON_AddNumberToObject(item, "rssi", ap_records[i].rssi);
        cJSON_AddNumberToObject(item, "auth", ap_records[i].authmode);
        cJSON_AddItemToArray(root, item);
    }

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (json_str == NULL)
    {
        ESP_LOGE(TAG, "Failed to print JSON");
        return NULL;
    }

    return json_str;
}

/**
 * @brief Parse WiFi credentials from JSON string
 */
esp_err_t json_helper_parse_wifi_credentials(const char *json_str,
                                             char *ssid_out, size_t ssid_len,
                                             char *password_out, size_t password_len)
{
    if (json_str == NULL || ssid_out == NULL || password_out == NULL)
    {
        ESP_LOGE(TAG, "Invalid parameters");
        return ESP_ERR_INVALID_ARG;
    }

    cJSON *root = cJSON_Parse(json_str);
    if (root == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        ESP_LOGE(TAG, "JSON parse error: %s", error_ptr ? error_ptr : "unknown");
        return ESP_ERR_INVALID_ARG;
    }

    cJSON *ssid_json = cJSON_GetObjectItem(root, "ssid");
    if (!cJSON_IsString(ssid_json))
    {
        ESP_LOGE(TAG, "SSID field not found or not a string");
        cJSON_Delete(root);
        return ESP_ERR_INVALID_ARG;
    }

    const char *ssid = ssid_json->valuestring;

    // Validate SSID length (WiFi SSID max 32 bytes, need null terminator)
    if (strlen(ssid) == 0 || strlen(ssid) > 31)
    {
        ESP_LOGE(TAG, "Invalid SSID length: %d", strlen(ssid));
        cJSON_Delete(root);
        return ESP_ERR_INVALID_ARG;
    }

    // Copy SSID
    strncpy(ssid_out, ssid, ssid_len - 1);
    ssid_out[ssid_len - 1] = '\0';

    // Get password (optional)
    cJSON *pass_json = cJSON_GetObjectItem(root, "password");
    if (cJSON_IsString(pass_json))
    {
        const char *password = pass_json->valuestring;

        // Validate password length (WiFi password max 63 characters)
        if (strlen(password) > 63)
        {
            ESP_LOGE(TAG, "Password too long: %d (max 63)", strlen(password));
            cJSON_Delete(root);
            return ESP_ERR_INVALID_ARG;
        }

        strncpy(password_out, password, password_len - 1);
        password_out[password_len - 1] = '\0';
    }
    else
    {
        password_out[0] = '\0';
    }

    cJSON_Delete(root);
    return ESP_OK;
}

/**
 * @brief Create WiFi status JSON string
 */
char *json_helper_create_wifi_status(bool connected, bool provisioned,
                                     const char *ip_address, int8_t rssi)
{
    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE(TAG, "Failed to create JSON object");
        return NULL;
    }

    cJSON_AddBoolToObject(root, "connected", connected);
    cJSON_AddBoolToObject(root, "provisioned", provisioned);

    if (connected)
    {
        if (ip_address != NULL && strlen(ip_address) > 0)
        {
            cJSON_AddStringToObject(root, "ip", ip_address);
        }
        cJSON_AddNumberToObject(root, "rssi", rssi);
    }

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (json_str == NULL)
    {
        ESP_LOGE(TAG, "Failed to print JSON");
        return NULL;
    }

    return json_str;
}

/**
 * @brief Create simple JSON response with status and message
 */
char *json_helper_create_simple_response(const char *status, const char *message)
{
    if (status == NULL || message == NULL)
    {
        ESP_LOGE(TAG, "Invalid parameters: status or message is NULL");
        return NULL;
    }

    cJSON *root = cJSON_CreateObject();
    if (root == NULL)
    {
        ESP_LOGE(TAG, "Failed to create JSON object");
        return NULL;
    }

    cJSON_AddStringToObject(root, "status", status);
    cJSON_AddStringToObject(root, "message", message);

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    if (json_str == NULL)
    {
        ESP_LOGE(TAG, "Failed to print JSON");
        return NULL;
    }

    return json_str;
}