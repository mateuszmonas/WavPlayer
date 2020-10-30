int USB_read_current_file(uint8_t *buffer, uint32_t buffer_size);
void USB_open_next_file(void);
void USB_rewind_files(void);
void USB_handle_state(uint8_t state);
uint8_t USB_mounted(void);
