int USB_read_current_file(uint8_t *buffer, uint32_t buffer_size);
void USB_open_next_file(void);
void USB_rewind_files(void);
void USB_handle_state(uint8_t state);
uint8_t USB_mounted(void);
void USB_create_wav_file_for_writing(void);
void USB_write_current_file(uint8_t *buffer, uint32_t buffer_size);
void USB_close_wav_file_for_writing(void);
