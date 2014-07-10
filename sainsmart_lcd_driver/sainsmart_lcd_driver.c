#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/delay.h>

#define reg_address 0x0A

/* Addresses to scan */
 static const unsigned short normal_i2c[] = { 0x27, I2C_CLIENT_END };

/* possible lcds */
static struct i2c_device_id sainsmart_id[] = {
  { "sainsmart", 0},
  {}
};

/* helper commands for writing (4-bit mode) */

int strobe(struct i2c_client *client, u8 value)
{
  u8 temp;
  temp = (value | 0x0C);
  i2c_smbus_write_byte(client, temp);
  mdelay(1);
  temp = (value & 0xFB);
  i2c_smbus_write_byte(client, temp); 
  mdelay(1);
  return 0;
}

int write_char(struct i2c_client *client, u8 value)
{
  u8 temp_value;
  temp_value = ((value >> 4) << 4) | 0x09;
  i2c_smbus_write_byte(client, value);
  strobe(client, temp_value);
  temp_value = ((value & 0x0F) << 4) | 0x09;
  i2c_smbus_write_byte(client, value);
  strobe(client, temp_value);
  i2c_smbus_write_byte(client, 0x08);  
  return 0;
}

int write_command(struct i2c_client *client, u8 value)
{
  u8 temp_value;
  temp_value = (value & 0xF0) | 0x08;
  i2c_smbus_write_byte(client, value);
  strobe(client, temp_value);
  temp_value = ((value & 0x0F)  << 4) | 0x08;
  i2c_smbus_write_byte(client, value);
  strobe(client, temp_value);
  i2c_smbus_write_byte(client, 0x08);
  return 0;
}


/* initialize lcd to 4-bit mode, required to use i2c backpack */

int init_lcd(struct i2c_client *client)
{
  mdelay(100);
  i2c_smbus_write_byte(client, 0x30);
  strobe(client, 0x30);
  mdelay(5);
  strobe(client, 0x30);
  mdelay(5);
  strobe(client, 0x30);
  mdelay(5);
  i2c_smbus_write_byte(client, 0x20);
  strobe(client, 0x20);
  mdelay(5);


  write_command(client, 0x28);
  write_command(client, 0x08);
  write_command(client, 0x01);
  write_command(client, 0x06);
  write_command(client, 0x0C);
  write_command(client, 0x0F);

  return 0;
}

/* driver initialization/remove functions */

MODULE_DEVICE_TABLE(i2c, sainsmart_id);

static int sainsmart_probe(struct i2c_client *client,
                         const struct i2c_device_id *id)
{
  pr_info("called probe");

  init_lcd(client);
  write_char(client, 'C');
  write_char(client, 'A');
  write_char(client, 'T');
  write_char(client, 'B');
  write_char(client, 'O');
  write_char(client, 'T');
  return 0;
}

static int sainsmart_remove(struct i2c_client *client)
{
  pr_info("called remove");
  return 0;
}


static struct i2c_driver sainsmart_driver = {
  .driver = {
    .name   = "sainsmart",
    .owner  = THIS_MODULE,
  },
  .probe          = sainsmart_probe,
  .remove         = sainsmart_remove,
  .id_table       = sainsmart_id
};

static int __init sainsmart_init(void)
{
  return i2c_add_driver(&sainsmart_driver);
}

module_init(sainsmart_init);

static void __exit sainsmart_exit(void)
{
  i2c_del_driver(&sainsmart_driver);
}

module_exit(sainsmart_exit);

MODULE_LICENSE("GPL");

