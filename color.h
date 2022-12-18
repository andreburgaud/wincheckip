#pragma once

// From https://cplusplus.com/articles/Eyhv0pDG/
// MIT License

/*
Header file to color text and background in windows console applications
Global variables - textcol,backcol,deftextcol,defbackcol,colorprotect
*/

#include<iosfwd>
#include<Windows.h>

namespace color {

    enum concol {
        black = 0,
        dark_blue = 1,
        dark_green = 2,
        dark_aqua, dark_cyan = 3,
        dark_red = 4,
        dark_purple = 5, dark_pink = 5, dark_magenta = 5,
        dark_yellow = 6,
        dark_white = 7,
        gray = 8,
        blue = 9,
        green = 10,
        aqua = 11, cyan = 11,
        red = 12,
        purple = 13, pink = 13, magenta = 13,
        yellow = 14,
        white = 15
    };

    HANDLE std_con_out;

    //Standard Output Handle
    const bool colorprotect = false;

    // If colorprotect is true, background and text colors will never be the same
    concol textcol, backcol, deftextcol, defbackcol;

    /*
    textcol - current text color
    backcol - current back color
    deftextcol - original text color
    defbackcol - original back color
    */

    inline void update_colors() {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(std_con_out, &csbi);
        textcol = concol(csbi.wAttributes & 15);
        backcol = concol((csbi.wAttributes & 0xf0) >> 4);
    }

    inline void setcolor(const concol textcolor, const concol backcolor) {
        if constexpr (colorprotect && textcolor == backcolor) return;
        textcol = textcolor; backcol = backcolor;
        unsigned short wAttributes = ((uint16_t)backcol << 4) | (uint16_t)textcol;
        SetConsoleTextAttribute(std_con_out, wAttributes);
    }

    inline void settextcolor(const concol textcolor) {
        if constexpr (colorprotect && textcolor == backcol) return;
        textcol = textcolor;
        unsigned short wAttributes = ((uint16_t)backcol << 4) | (uint16_t)textcol;
        SetConsoleTextAttribute(std_con_out, wAttributes);
    }

    inline void setbackcolor(const concol backcolor) {
        if constexpr (colorprotect && textcol == backcolor) return;
        backcol = backcolor;
        unsigned short wAttributes = ((uint16_t)backcol << 4) | (uint16_t)textcol;
        SetConsoleTextAttribute(std_con_out, wAttributes);
    }

    inline void concolinit() {
        std_con_out = GetStdHandle(STD_OUTPUT_HANDLE);
        update_colors();
        deftextcol = textcol; defbackcol = backcol;
    }

    template<class elem, class traits>
    inline std::basic_ostream<elem, traits>& operator<<(std::basic_ostream<elem, traits>& os, concol col)
    {
        os.flush(); settextcolor(col); return os;
    }

    template<class elem, class traits>
    inline std::basic_istream<elem, traits>& operator>>(std::basic_istream<elem, traits>& is, concol col) {
        std::basic_ostream<elem, traits>* p = is.tie();
        if (p != NULL)p->flush();
        settextcolor(col);
        return is;
    }
}
