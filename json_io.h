
// Copyright (c) 2011 Ger Hobbelt
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef JSON_INPUT_OUTPUT_STUFF
#define JSON_INPUT_OUTPUT_STUFF


#include "tws_data_structures.h"



class json_output
{
protected:
    struct mg_connection *m_conn;
    int m_indent_level;
    int m_sibling_counter;
    std::vector<int> m_counter_stack;

public:
    json_output(struct mg_connection *conn);
    virtual ~json_output();

protected:
    void json_output_indent(void);

public:
    virtual void add_sibling(const char *name, const char *value);
    virtual void add_sibling(const char *name, const ib_date_t &value);
    virtual void add_sibling(const char *name, const ib_string_t &value);
    virtual void add_sibling(const char *name, int value);
    virtual void add_sibling(const char *name, double value);

    virtual void push_level(const char *name);
    virtual void pop_level(void);

    virtual void begin_object(void);
    virtual void end_object(void);
    virtual void begin_array(void);
    virtual void end_array(void);

    virtual void start(void);
    virtual void finish(void);
};





#endif // JSON_INPUT_OUTPUT_STUFF
