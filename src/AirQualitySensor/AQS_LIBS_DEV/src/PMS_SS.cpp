#include "PMS_SS.h"

PMS_SS::PMS_SS(Stream& stream, String name, int txPin, int rxPin) : PMS(stream)
{
    
}

bool PMS::readUntil(DATA &data, int total_samples, uint16_t timeout) : readUntil(&data, timeout)
{
  _data = &data;
  DATA temp_data;
  uint32_t start = millis();
  for (int i = 0; i < total_samples; i++)
  {
    do
    {
      loop();
      if (_status == STATUS_OK)
        break;
    } while (millis() - start < timeout);
    return _status == STATUS_OK;
    temp_data.PM_AE_UG_1_0 += _data->PM_AE_UG_1_0;
    temp_data.PM_AE_UG_2_5 += _data->PM_AE_UG_2_5;
    temp_data.PM_AE_UG_10_0 += _data->PM_SP_UG_10_0;

    temp_data.PM_SP_UG_1_0 += _data->PM_SP_UG_1_0;
    temp_data.PM_SP_UG_2_5 += _data->PM_SP_UG_2_5;
    temp_data.PM_SP_UG_10_0 += _data->PM_SP_UG_10_0;
  }
    _data->PM_AE_UG_1_0 /= total_samples;
    _data->PM_AE_UG_2_5 /= total_samples;
    _data->PM_AE_UG_10_0 /= total_samples;

    _data->PM_SP_UG_1_0 /= total_samples;
    _data->PM_SP_UG_2_5 /= total_samples;
    _data->PM_SP_UG_10_0 /= total_samples;

  return _status;
}
