char dtoa(char digit) {
    if (digit <= 9)
        return '0' + digit;
    if (digit <= 15)
        return 'A' + (digit - 10);
    return '\0';
}
