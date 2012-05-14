
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

#include "system-includes.h"

#include "json_io.h"

#include <mongoose/mongoose_ex.h>



json_output::json_output(struct mg_connection *conn)
	: m_conn(conn)
	, m_indent_level(0)
	, m_sibling_counter(0)
{
}
json_output::~json_output()
{
}


void json_output::json_output_indent(void)
{
	for (int i = m_indent_level; i > 0; i--)
	{
		mg_printf(m_conn, "\t");
	}
}


void json_output::add_sibling(const char *name, const char *value)
{
	assert(m_conn);
	if (m_sibling_counter > 0)
	{
		mg_printf(m_conn, ",\n");
	}
	json_output_indent();
	mg_printf(m_conn, "%s: \"%s\"", name, value);
	m_sibling_counter++;
}
void json_output::add_sibling(const char *name, const ib_date_t &value)
{
	ib_string_t s = ib_string_t(value);
	add_sibling(name, s.c_str());
}
void json_output::add_sibling(const char *name, const ib_string_t &value)
{
	assert(m_conn);
	add_sibling(name, value.c_str());
}
void json_output::add_sibling(const char *name, int value)
{
	assert(m_conn);
	if (m_sibling_counter > 0)
	{
		mg_printf(m_conn, ",\n");
	}
	json_output_indent();
	mg_printf(m_conn, "%s: %d", name, value);
	m_sibling_counter++;
}
void json_output::add_sibling(const char *name, double value)
{
	assert(m_conn);
	if (m_sibling_counter > 0)
	{
		mg_printf(m_conn, ",\n");
	}
	json_output_indent();
	mg_printf(m_conn, "%s: %g", name, value);
	m_sibling_counter++;
}

void json_output::push_level(const char *name)
{
	assert(m_conn);
	if (m_sibling_counter > 0)
	{
		mg_printf(m_conn, ",\n");
	}
	json_output_indent();
	mg_printf(m_conn, "%s: {\n", name);
	m_counter_stack.push_back(m_sibling_counter);
	m_sibling_counter = 0;
	m_indent_level++;
}
void json_output::pop_level(void)
{
	assert(m_conn);
	assert(m_indent_level > 1);
	if (m_sibling_counter > 0)
	{
		mg_printf(m_conn, "\n");
	}
	m_indent_level--;
	assert(m_counter_stack.size() > 0);
	m_sibling_counter = m_counter_stack.back();
	m_counter_stack.pop_back();
	json_output_indent();
	mg_printf(m_conn, "}");
	m_sibling_counter++;
}

void json_output::start(void)
{
	assert(m_conn);
	mg_printf(m_conn, "{\n");
	m_indent_level = 1;
	m_sibling_counter = 0;
	m_counter_stack.clear();
}
void json_output::finish(void)
{
	assert(m_conn);
	if (m_sibling_counter > 0)
	{
		mg_printf(m_conn, "\n");
	}
	m_indent_level--;
	json_output_indent();
	mg_printf(m_conn, "}");
}
