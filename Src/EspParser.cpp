//
// Created by shubin on 02.07.2020.
//

#include "EspParser.h"
#include "wifi_module.h"
#include "string.h"
#include "serial.h"

EspParser espParser;

#define FRAME_SIZE() (((unsigned short)(this->buffer[2])) | ((unsigned short)(this->buffer[3])<<8))

static short find_char(const unsigned char  *_array, unsigned short len, unsigned char c) {
    for(unsigned short i = 0; i < len; i++)
        if (_array[i] == c)
            return i;
    return -1;
}


unsigned char EspParser::shift_buffer() {
    if (this->buffer_fill > 1) {
        unsigned short new_head = find_char(&this->buffer[1], this->buffer_fill - 1, ESP_PROTOC_HEAD);
        if (new_head>=0) {
            new_head++;
            memcpy(this->buffer, &this->buffer[new_head], this->buffer_fill - new_head);
            this->buffer_fill-=new_head;
            if (this->buffer_fill>4) {
                this->calc_tail_expected();

            }
            return 1;
        }
    }
    return 0;
}

unsigned char EspParser::validate_size() {
    return FRAME_SIZE() <= 1019;
}

unsigned char EspParser::validate_tail() {
    return this->buffer[FRAME_SIZE() + 3] == ESP_PROTOC_TAIL;
}

unsigned short EspParser::calc_tail_expected() {
    return this->tail_expected = FRAME_SIZE() + 4;
}

void EspParser::do_message() {
    execute_wifi_frame(this->buffer[1], &this->buffer[4], FRAME_SIZE());
}

unsigned char EspParser::validate_frame() {
    return message_type_is_walid(this->buffer[1])  && validate_size();
}

void EspParser::parse(unsigned char * data, unsigned short len) {
    unsigned short cursor = 0;
    main_loop: while (cursor < len) {
        //Поиск заголовка
        if (this->buffer_fill == 0) {
            short head = 0;
            head = find_char((unsigned char *)&data[cursor], len-cursor, ESP_PROTOC_HEAD);
            if (head == -1) {
                this->clear();
                return;
            } else {
                cursor += head;
                this->buffer[0] = ESP_PROTOC_HEAD;
                this->buffer_fill++;
                SERIAL_ECHOLN("HEAD FOUND");
                cursor++;
            }
        }
        if (this->buffer_fill != 0) {
            unsigned char c;
            while ((this->buffer_fill<sizeof(this->buffer)) && (cursor<len)) {
                c = data[cursor++];
                unsigned short current_index = this->buffer_fill++;
                this->buffer[current_index] = c;
                if (this->buffer_fill==4) {
                    if (!this->validate_frame()) {
                        SERIAL_ECHOLNPAIR("FRAME SIZE ERROR", FRAME_SIZE());
                        this->clear();
                        goto main_loop;
                    } else {
                        this->calc_tail_expected();
                    }
                } else if (this->buffer_fill>4) {
                    if (c == ESP_PROTOC_TAIL) {
                        if (current_index == this->tail_expected) {
                            this->do_message();
                            this->clear();
                            goto main_loop;
                        } else if (this->tail_expected<current_index) {
                            SERIAL_ECHOPAIR("BROKEN FRAME, NO EXPECTED TAIL AT : ", this->tail_expected);
                            if (!this->shift_buffer()) {
                                this->clear();
                                goto main_loop;
                            }
                        }
                    } else if ((this->buffer_fill >= sizeof(this->buffer)) || (this->tail_expected==current_index)) {
                        SERIAL_ECHOPAIR("BROKEN FRAME, BUFFER FULL OR NO EXPECTED TAIL AT : ", this->tail_expected);
                        if (!this->shift_buffer()) {
                            this->clear();
                            goto main_loop;
                        }
                    }
                }
            }
        }

    }
}
