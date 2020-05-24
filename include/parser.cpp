#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "db.cpp"

/*
* S -> SELECT_SENTENCE | INSERT_SENTENCE | UPDATE_SENTENCE |
* 	 DELETE_SENTENCE | CREATE_SENTENCE | DROP_SENTENCE
* 	 
* SELECT_SENTENCE -> select FIELD_LIST from TABLE_NAME WHERE_CLAUSE
* FIELD_LIST -> FIELD_NAME {, FIELD_NAME } | *
* TABLE_NAME -> NAME
* FIELD_NAME -> NAME
* NAME -> ident
* 
* 
* INSERT_SENTENCE -> insert into TABLE_NAME (FIELD_VALUE {, FIELD_VALUE})
* FIELD_VALUE -> string | number
* 
* 
* UPDATE_SENTENCE -> update TABLE_NAME set FIELD_NAME = EXPRESSION WHERE_CLAUSE
* 
* DELETE_SENTENCE -> delete from TABLE_NAME WHERE_CLAUSE
* 
* CREATE_SENTENCE -> create table TABLE_NAME (FIELD_DESCRIPTION_LIST)
* FIELD_DESCRIPTION_LIST -> FIELD_DESCRIPTION {, FIELD_DESCRIPTION}
* FIELD_DESCRIPTION -> FIELD_NAME FIELD_TYPE
* FIELD_TYPE -> text (long) | long
* 
* DROP_SENTENCE -> drop table TABLE_NAME
* 
* WHERE_CLAUSE -> where TEXT_FIELD_NAME [not] like STRING_SAMPLE |
* 				where EXPRESSION [not] in (VALUE_LIST) |
* 				where BOOL_EXPRESSION |
* 				where all
* STRING_SAMPLE -> string 
* EXPRESSION -> LONG_EXPRESSION | TEXT_EXPRESSION
* 
* TEXT_FIELD_NAME	-> ident	
* VALUE_LIST -> string {, string} | number {, number}
* 
* LONG_EXPRESSION -> LONG_ADD {SIGN_ADD LONG_ADD}
* SIGN_ADD -> + | -
* LONG_ADD -> LONG_MULT {SIGN_MULT LONG_MULT}
* SIGN_MULT -> * | / | %
* LONG_MULT -> LONG_VALUE | (LONG_EXPRESSION)
* LONG_VALUE -> LONG_FIELD_NAME | number
* LONG_FIELD_NAME -> ident
* 
* TEXT_EXPRESSION -> TEXT_FIELD_NAME | string
* TEXT_FIELD_NAME -> ident
* 
* BOOL_EXPRESSION -> BOOL_ADD {or BOOL_ADD}
* BOOL_ADD -> BOOL_MULT {and BOOL_MULT}
* BOOL_MULT -> not BOOL_MULT | (BOOL_EXPRESSION) | (COMPARISON)
* COMPARISON -> TEXT_COMPARISON | LONG_COMPARISON
* TEXT_COMPARISON -> TEXT_EXPRESSION SIGN_COMP TEXT_EXPRESSION
* LONG_COMPARISON -> LONG_EXPRESSION SIGN_COMP LONG_EXPRESSION
* SIGN_COMP -> = | != | < | > | <= | >=

* SELECT_SENTENCE -> select FIELD_LIST from TABLE_NAME WHERE_CLAUSE
* FIELD_LIST -> FIELD_NAME {, FIELD_NAME } | *
* TABLE_NAME -> NAME
* FIELD_NAME -> NAME
* NAME -> ident

*/

namespace Parser
{
    WhereCondition
    Where(std::vector<Lex::Lexem>::iterator lexem_iter, std::vector<Lex::Lexem>::iterator end_iter)
    {
        struct WhereCondition result;

        // We need to know which alternative we have in the query. Firstly, we check if
        // it is the simplest one.
        if (lexem_iter->get_token() == Lex::ALL)
        {
            result.type = Lex::ALL;
            return result;
        }

        Lex::Lexem smth = *lexem_iter;
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }

        // At this point we know that 'smth' is not ALL lexem, but we still don't know
        // what it really is. It can be name of the field in LIKE-alternative, can be
        // a part of expression in IN alternative or in default WHERE clause. If the
        // next lexem is either NOT or LIKE, we have LIKE alternative. Let's check it.
        if (lexem_iter->get_token() == Lex::LIKE || (lexem_iter != end_iter && (lexem_iter->get_token() == Lex::NOT && (lexem_iter + 1)->get_token() == Lex::LIKE)))
        {
            // Now we need to save the field name that 'smth' happens to be and then get the expression string.
            result.type = Lex::LIKE;
            result.l_lex_1 = smth;
            if (lexem_iter->get_token() == Lex::NOT)
            {
                result.Not = true;

                // Now passing NOT and LIKE
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }

                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }
            }
            else if (lexem_iter->get_token() == Lex::NOT && lexem_iter == end_iter)
            {
                std::__throw_logic_error("'NOT' keyword in the end of query is not allowed");
            }
            else
            {
                result.Not = false;
                // Going past LIKE keyword
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }
            }

            // Checking if we have a string.
            if (lexem_iter->get_token() == Lex::STRING)
            {
                result.l_lex_2 = *lexem_iter;
            }
            else
            {
                std::__throw_logic_error("expected string");
            }

            return result;
        }

        // Now we don't know which alternative this is, but we need to get the expression
        // either way.
        std::vector<Lex::Lexem> lexems;
        lexems.push_back(smth);
        Lex::token type = Lex::WHERE;
        bool isNot = false;
        while (lexem_iter != end_iter)
        {
            Lex::token t = lexem_iter->get_token();

            // Checking if we need to end the loop and which alternative we have.
            if (lexem_iter->get_token() == Lex::IN || (lexem_iter != end_iter && (lexem_iter->get_token() == Lex::NOT && (lexem_iter + 1)->get_token() == Lex::IN)))
            {
                type = Lex::IN;
                break;
            }
            else if (lexem_iter->get_token() == Lex::NOT && lexem_iter == end_iter)
            {
                std::__throw_logic_error("'NOT' keyword in the end of query is not allowed");
            }

            if (t == Lex::TEXT || t == Lex::IDENT)
            {
                Lex::token lexems_end_token = !lexems.empty() ? (lexem_iter - 1)->get_token() : Lex::NOT;
                if (!lexems.empty() &&
                    (lexems_end_token == Lex::IDENT ||
                     lexems_end_token == Lex::TEXT))
                {
                    throw std::logic_error("two field names in a row are not allowed");
                }
                else if (!lexems.empty() && lexems_end_token == Lex::NUMBER)
                {
                    throw std::logic_error("number and a field name in a row is not allowed");
                }
                lexems.push_back(*lexem_iter);
            }
            else if (
                t == Lex::SOFT_GREATER ||
                t == Lex::SOFT_LESSER ||
                t == Lex::NOT_EQUAL ||
                t == Lex::PERCENT ||
                t == Lex::GREATER ||
                t == Lex::LESSER ||
                t == Lex::MINUS ||
                t == Lex::CARET ||
                t == Lex::EQUAL ||
                t == Lex::MULT ||
                t == Lex::PLUS ||
                t == Lex::MULT ||
                t == Lex::MULT ||
                t == Lex::DIV ||
                t == Lex::NOT ||
                t == Lex::AND ||
                t == Lex::OR)
            {
                Lex::token lexems_end_token = !lexems.empty() ? (lexem_iter - 1)->get_token() : Lex::NOT;
                if (!lexems.empty() &&
                    (lexems_end_token == Lex::SOFT_GREATER ||
                     lexems_end_token == Lex::SOFT_LESSER ||
                     lexems_end_token == Lex::NOT_EQUAL ||
                     lexems_end_token == Lex::PERCENT ||
                     lexems_end_token == Lex::GREATER ||
                     lexems_end_token == Lex::LESSER ||
                     lexems_end_token == Lex::MINUS ||
                     lexems_end_token == Lex::CARET ||
                     lexems_end_token == Lex::EQUAL ||
                     lexems_end_token == Lex::MULT ||
                     lexems_end_token == Lex::PLUS ||
                     lexems_end_token == Lex::MULT ||
                     lexems_end_token == Lex::MULT ||
                     lexems_end_token == Lex::DIV ||
                     lexems_end_token == Lex::NOT ||
                     lexems_end_token == Lex::AND ||
                     lexems_end_token == Lex::OR))
                {
                    throw std::logic_error("two operators in a row are not allowed");
                }
                lexems.push_back(*lexem_iter);
            }
            else if (t == Lex::NUMBER)
            {
                Lex::token lexems_end_token = !lexems.empty() ? (lexem_iter - 1)->get_token() : Lex::NOT;
                if (!lexems.empty() && lexems_end_token == Lex::NUMBER)
                {
                    throw std::logic_error("two numbers in a row are not allowed");
                }
                else if (!lexems.empty() &&
                         (lexems_end_token == Lex::IDENT ||
                          lexems_end_token == Lex::TEXT))
                {
                    throw std::logic_error("field name and a number in a row is not allowed");
                }
                lexems.push_back(*lexem_iter);
            }
            else if (t == Lex::OPEN)
            {
                lexems.push_back(*lexem_iter);
            }
            else if (t == Lex::CLOSE)
            {
                lexems.push_back(*lexem_iter);
            }
            else if (t == Lex::STRING)
            {
                std::string s = lexem_iter->get_string();
                replace_all(s, "'", "");
                lexem_iter->set_string(s);
                lexem_iter->set_token(Lex::STRING);
                lexems.push_back(*lexem_iter);
            }

            if (lexem_iter == end_iter)
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }
        }

        // Now we know what it is. Let's call the appropriate function.
        if (type == Lex::WHERE)
        {
            // Simple WHERE clause, returning.
            result.type = Lex::WHERE;
            result.v_lex_1 = lexems;
            return result;
        }
        else
        {
            // IN alternative.
            if (lexem_iter == end_iter)
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }

            if (lexem_iter->get_token() == Lex::OPEN)
            {
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }

                std::vector<Lex::Lexem> consts;
                while (lexem_iter->get_token() != Lex::CLOSE)
                {
                    Lex::token t = lexem_iter->get_token();
                    if (t != Lex::STRING && t != Lex::LONG && t != Lex::NUMBER)
                    {
                        std::__throw_logic_error("expected string or long constant");
                    }
                    consts.push_back(*lexem_iter);

                    if (lexem_iter == end_iter)
                    {
                        std::__throw_logic_error("unexpected EOS");
                    }
                    else
                    {
                        lexem_iter++;
                    }

                    while (lexem_iter->get_token() == Lex::COMMA)
                    {
                        if (lexem_iter == end_iter)
                        {
                            std::__throw_logic_error("unexpected EOS");
                        }
                        else
                        {
                            lexem_iter++;
                        }
                    }
                }

                // Returning.
                result.type = Lex::IN;
                result.Not = isNot;
                result.v_lex_1 = lexems;
                result.v_lex_2 = consts;
            }
        }

        return result;
    }

    std::string Select(std::vector<Lex::Lexem>::iterator lexem_iter, std::vector<Lex::Lexem>::iterator end_iter)
    {
        // Select
        bool star = false;
        WhereCondition r;

        // Fields
        std::vector<std::string> field_names;
        field_names.clear();
        if (lexem_iter->get_token() == Lex::MULT)
        {
            star = true;
            if (lexem_iter == end_iter)
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }
        }
        else
        {
            while (lexem_iter->get_token() == Lex::IDENT || lexem_iter->get_token() == Lex::TEXT)
            {
                field_names.push_back(lexem_iter->get_string());
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }

                if (lexem_iter->get_token() == Lex::COMMA)
                {
                    if (lexem_iter == end_iter)
                    {
                        std::__throw_logic_error("unexpected EOS");
                    }
                    else
                    {
                        lexem_iter++;
                    }
                }
            }
        }

        // From
        if (lexem_iter->get_token() != Lex::FROM)
        {
            throw std::logic_error("expected 'FROM' keyword");
        }

        // Table name
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }
        if (lexem_iter->get_token() != Lex::TEXT && lexem_iter->get_token() != Lex::IDENT)
        {
            throw std::logic_error("expected table name");
        }
        std::string table_name = lexem_iter->get_string();

        // Where
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }
        if (lexem_iter->get_token() == Lex::WHERE)
        {
            if (lexem_iter == end_iter)
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }
            r = Where(lexem_iter, end_iter);
        }

        std::string result;
        execute_select(field_names, table_name, r, result);

        return result;
    }

    std::string Drop(std::vector<Lex::Lexem>::iterator lexem_iter, std::vector<Lex::Lexem>::iterator end_iter)
    {
        if (lexem_iter->get_token() != Lex::TABLE)
        {
            throw std::logic_error("expected 'TABLE' keyword");
        }

        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }
        if (lexem_iter->get_token() != Lex::TEXT && lexem_iter->get_token() != Lex::IDENT)
        {
            throw std::logic_error("expected table name");
        }
        else
        {
            std::string table_name = lexem_iter->get_string();

            // Calling drop function
            std::string result;
            execute_drop(table_name, result);
            return result;
        }
        return "Error: invalid input in create";
    }

    std::string Create(std::vector<Lex::Lexem>::iterator lexem_iter, std::vector<Lex::Lexem>::iterator end_iter)
    {
        if (lexem_iter->get_token() != Lex::TABLE)
        {
            throw std::logic_error("expected 'TABLE' keyword");
        }

        // Table name
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }
        std::string table_name;
        if (lexem_iter->get_token() != Lex::TEXT && lexem_iter->get_token() != Lex::IDENT)
        {
            throw std::logic_error("expected table name");
        }
        else
        {
            table_name = lexem_iter->get_string();
        }

        // Opening bracket
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }
        if (lexem_iter->get_token() != Lex::OPEN)
        {
            throw std::logic_error("expected opening bracket");
        }

        // Table fields
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }
        std::vector<struct FieldDef> fields;
        fields.clear();

        while (lexem_iter->get_token() != Lex::CLOSE)
        {
            FieldDef current_field = {"0", Logic, 0};
            strncpy(current_field.name, lexem_iter->get_string().data(), MaxFieldNameLen);

            if (lexem_iter == end_iter)
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }
            if (lexem_iter->get_token() == Lex::TEXT || lexem_iter->get_token() == Lex::IDENT)
            {
                current_field.type = Text;
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }

                if (lexem_iter->get_token() != Lex::OPEN)
                {
                    throw std::logic_error("expected 'OPEN' keyword");
                }
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }

                if (lexem_iter->get_token() != Lex::NUMBER)
                {
                    throw std::logic_error("expected number");
                }
                else
                {
                    current_field.len = stoi(lexem_iter->get_string());
                }
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }

                if (lexem_iter->get_token() != Lex::CLOSE)
                {
                    throw std::logic_error("expected 'CLOSE' keyword");
                }
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }
            }
            else if (lexem_iter->get_token() == Lex::LONG)
            {
                current_field.type = Long;
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }
            }
            else
            {
                throw std::logic_error("expected either 'LONG' or 'TEXT' keyword");
            }

            if (lexem_iter->get_token() == Lex::COMMA)
            {
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }
            }

            fields.push_back(current_field);
        }

        // Calling create function
        std::string result;
        execute_create(table_name, fields, result);
        return result;
    }

    std::string Delete(std::vector<Lex::Lexem>::iterator lexem_iter, std::vector<Lex::Lexem>::iterator end_iter)
    {
        if (lexem_iter->get_token() != Lex::FROM)
        {
            throw std::logic_error("expected 'FROM' keyword");
        }
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }

        if (lexem_iter->get_token() != Lex::TEXT && lexem_iter->get_token() != Lex::IDENT)
        {
            throw std::logic_error("expected table name");
        }
        std::string table_name = lexem_iter->get_string();
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }

        if (lexem_iter->get_token() != Lex::WHERE)
        {
            throw std::logic_error("expected 'WHERE' keyword");
        }

        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }

        WhereCondition r = Where(lexem_iter, end_iter);

        // Calling delete function
        std::string result;
        execute_delete(table_name, r, result);
        return result;
    }

    std::string Insert(std::vector<Lex::Lexem>::iterator lexem_iter, std::vector<Lex::Lexem>::iterator end_iter)
    {
        if (lexem_iter->get_token() != Lex::INTO)
        {
            throw std::logic_error("expected 'INTO' keyword");
        }
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }

        if (lexem_iter->get_token() != Lex::TEXT && lexem_iter->get_token() != Lex::IDENT)
        {
            throw std::logic_error("expected table name");
        }
        std::string table_name = lexem_iter->get_string();
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }

        if (lexem_iter->get_token() != Lex::OPEN)
        {
            throw std::logic_error("expected 'OPEN' keyword");
        }
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }

        std::vector<struct InsertField> fields;
        while (lexem_iter->get_token() != Lex::CLOSE)
        {
            if (lexem_iter->get_token() == Lex::STRING)
            {
                struct InsertField current_field = {Text, lexem_iter->get_string().substr(1, lexem_iter->get_string().size() - 2), 0};
                fields.push_back(current_field);
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }
            }
            else if (lexem_iter->get_token() == Lex::NUMBER)
            {
                struct InsertField current_field = {Long, "", (unsigned int)std::stol(lexem_iter->get_string())};
                fields.push_back(current_field);
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }
            }
            else if (lexem_iter->get_token() == Lex::COMMA)
            {
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }
            }
            else
            {
                throw std::logic_error("expected string or long typed fieldname or comma");
            }
        }

        // Calling insert function
        std::string result;
        execute_insert(table_name, fields, result);
        return result;
    }

    std::string Update(std::vector<Lex::Lexem>::iterator lexem_iter, std::vector<Lex::Lexem>::iterator end_iter)
    {
        if (lexem_iter->get_token() != Lex::TEXT && lexem_iter->get_token() != Lex::IDENT)
        {
            throw std::logic_error("expected table name");
        }
        std::string table_name = lexem_iter->get_string();
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }

        if (lexem_iter->get_token() != Lex::SET)
        {
            throw std::logic_error("expected 'SET' keyword");
        }
        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }

        std::vector<RPN::RPNExpr> expr;

        while (lexem_iter->get_token() != Lex::WHERE)
        {
            if (lexem_iter->get_token() != Lex::TEXT && lexem_iter->get_token() != Lex::IDENT)
            {
                throw std::logic_error("expected typename");
            }
            std::string field_name = lexem_iter->get_string();
            if (lexem_iter == end_iter)
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }

            if (lexem_iter->get_token() != Lex::EQUAL)
            {
                throw std::logic_error("expected '='");
            }
            if (lexem_iter == end_iter)
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }

            std::vector<Lex::Lexem> lexems;
            while (lexem_iter->get_token() != Lex::WHERE && lexem_iter->get_token() != Lex::COMMA)
            {
                Lex::token t = lexem_iter->get_token();
                if (t == Lex::TEXT || t == Lex::IDENT)
                {
                    Lex::token lexems_end_token = !lexems.empty() ? (lexem_iter - 1)->get_token() : Lex::NOT;
                    if (!lexems.empty() &&
                        (lexems_end_token == Lex::IDENT ||
                         lexems_end_token == Lex::TEXT))
                    {
                        throw std::logic_error("two field names in a row are not allowed");
                    }
                    else if (!lexems.empty() && lexems_end_token == Lex::NUMBER)
                    {
                        throw std::logic_error("number and a field name in a row is not allowed");
                    }
                    lexems.push_back(*lexem_iter);
                }
                else if (t == Lex::DIV || t == Lex::MULT || t == Lex::PLUS || t == Lex::MULT || t == Lex::MULT || t == Lex::MINUS || t == Lex::PERCENT)
                {
                    Lex::token lexems_end_token = !lexems.empty() ? (lexem_iter - 1)->get_token() : Lex::NOT;
                    if (!lexems.empty() &&
                        (lexems_end_token == Lex::DIV ||
                         lexems_end_token == Lex::MULT ||
                         lexems_end_token == Lex::PLUS ||
                         lexems_end_token == Lex::MULT ||
                         lexems_end_token == Lex::MULT ||
                         lexems_end_token == Lex::MINUS ||
                         lexems_end_token == Lex::PERCENT))
                    {
                        throw std::logic_error("two operators in a row are not allowed");
                    }
                    lexems.push_back(*lexem_iter);
                }
                else if (t == Lex::NUMBER)
                {
                    Lex::token lexems_end_token = !lexems.empty() ? (lexem_iter - 1)->get_token() : Lex::NOT;
                    if (!lexems.empty() && lexems_end_token == Lex::NUMBER)
                    {
                        throw std::logic_error("two numbers in a row are not allowed");
                    }
                    else if (!lexems.empty() &&
                             (lexems_end_token == Lex::IDENT ||
                              lexems_end_token == Lex::TEXT))
                    {
                        throw std::logic_error("field name and a number in a row is not allowed");
                    }
                    lexems.push_back(*lexem_iter);
                }
                if (lexem_iter == end_iter)
                {
                    std::__throw_logic_error("unexpected EOS");
                }
                else
                {
                    lexem_iter++;
                }
            }
            RPN::RPNExpr current;
            current.field_name = field_name;
            current.rpn = lexems;
            expr.push_back(current);
        }

        if (lexem_iter == end_iter)
        {
            std::__throw_logic_error("unexpected EOS");
        }
        else
        {
            lexem_iter++;
        }

        WhereCondition r = Where(lexem_iter, end_iter);

        // Calling update function
        std::string result;
        execute_update(table_name, expr, r, result);
        return result;
    }

    std::string analyze(std::vector<Lex::Lexem> query)
    {
        auto lexem_iter = query.begin();
        switch (lexem_iter->get_token())
        {
        case Lex::SELECT:
            if (lexem_iter == query.end())
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }
            return Select(lexem_iter, query.end());
        case Lex::DROP:
            if (lexem_iter == query.end())
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }
            return Drop(lexem_iter, query.end());
        case Lex::CREATE:
            if (lexem_iter == query.end())
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }
            return Create(lexem_iter, query.end());
        case Lex::DELETE:
            if (lexem_iter == query.end())
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }
            return Delete(lexem_iter, query.end());
        case Lex::INSERT:
            if (lexem_iter == query.end())
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }
            return Insert(lexem_iter, query.end());
        case Lex::UPDATE:
            if (lexem_iter == query.end())
            {
                std::__throw_logic_error("unexpected EOS");
            }
            else
            {
                lexem_iter++;
            }
            return Update(lexem_iter, query.end());
        }
        return "Error: invalid input in analyze";
    }

} // namespace Parser