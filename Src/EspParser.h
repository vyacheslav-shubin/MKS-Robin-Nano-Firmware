//
// Created by shubin on 02.07.2020.
//

#ifndef WORK_ESPPARSER_H
#define WORK_ESPPARSER_H


class EspParser {
private:
    unsigned short buffer_fill;
    unsigned short tail_expected;
    unsigned char buffer[1024];
    void clear() {
        buffer_fill = 0;
        tail_expected = 0;
        //memset(buffer_fill, 0, sizeof(buffer_fill));
    }
    unsigned short calc_tail_expected();
    unsigned char validate_size();
    unsigned char validate_frame();
    unsigned char validate_tail();
    unsigned char shift_buffer();
    void do_message();
public:
    void parse(unsigned char * data, unsigned short len);
    EspParser() {
        this->clear();
    }
};

extern EspParser espParser;

#endif //WORK_ESPPARSER_H
