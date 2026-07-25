v0.0.4:
- made `kernel_panic` work via `kprintf`
- `vga_change_text_color`
- `%k` fmt specifier (changes color to specified in va args)
- `include/utils/vga_colors.h` header with vga colors
- `kvprintf` (`kprintf` but with va_list as second param)
- `KPANIC_DBG` macro is now do-while.
- basic logging (`klog`)
- fixed timer impl
