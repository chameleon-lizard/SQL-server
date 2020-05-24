#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdio>
#include <regex>
#include <stack>

#include "table.cpp"
#include "lex.cpp"

namespace RPN
{
    enum RPNType
    {
        NUMERIC,
        LOGIC
    };

    struct RPNExpr
    {
        std::string field_name;
        std::vector<Lex::Lexem> rpn;
    };

    bool calculate(std::vector<Lex::Lexem> expr, std::string &result)
    {
        std::stack<std::string> opndStack;

        for (auto i = expr.begin(); i != expr.end(); i++)
        {
            int res;
            std::string s_res;
            switch (i->get_token())
            {
            case Lex::TEXT:
            case Lex::IDENT:
            case Lex::STRING:
                opndStack.push(i->get_string());
                break;
            case Lex::NUMBER:
                opndStack.push(i->get_string());
                break;
            case Lex::PLUS:
                res = std::stoi(opndStack.top());
                opndStack.pop();
                res += std::stoi(opndStack.top());
                opndStack.pop();
                opndStack.push(std::to_string(res));
                break;
            case Lex::MINUS:
                res = std::stoi(opndStack.top());
                opndStack.pop();
                res = std::stoi(opndStack.top()) - res;
                opndStack.pop();
                opndStack.push(std::to_string(res));
                break;
            case Lex::MULT:
                res = std::stoi(opndStack.top());
                opndStack.pop();
                res *= std::stoi(opndStack.top());
                opndStack.pop();
                opndStack.push(std::to_string(res));
                break;
            case Lex::DIV:
                res = std::stoi(opndStack.top());
                opndStack.pop();
                res = std::stoi(opndStack.top()) / res;
                opndStack.pop();
                opndStack.push(std::to_string(res));
                break;
            case Lex::PERCENT:
                res = std::stoi(opndStack.top());
                opndStack.pop();
                res = std::stoi(opndStack.top()) % res;
                opndStack.pop();
                opndStack.push(std::to_string(res));
                break;
            case Lex::AND:
                res = std::stoi(opndStack.top());
                opndStack.pop();
                res = std::stoi(opndStack.top()) && res;
                opndStack.pop();
                opndStack.push(std::to_string(res));
                break;
            case Lex::OR:
                res = std::stoi(opndStack.top());
                opndStack.pop();
                res = std::stoi(opndStack.top()) && res;
                opndStack.pop();
                opndStack.push(std::to_string(res));
                break;
            case Lex::CARET:
                res = std::stoi(opndStack.top());
                opndStack.pop();
                res = std::stoi(opndStack.top()) ^ res;
                opndStack.pop();
                opndStack.push(std::to_string(res));
                break;
            case Lex::NOT_EQUAL:
                s_res = opndStack.top();
                opndStack.pop();
                s_res = opndStack.top().compare(s_res) ? "1" : "0";
                opndStack.pop();
                opndStack.push(s_res);
                break;
            case Lex::EQUAL:
                s_res = opndStack.top();
                opndStack.pop();
                s_res = opndStack.top().compare(s_res) ? "0" : "1";
                opndStack.pop();
                opndStack.push(s_res);
                break;
            case Lex::SOFT_GREATER:
                s_res = opndStack.top();
                opndStack.pop();
                s_res = opndStack.top().compare(s_res) >= 0 ? "1" : "0";
                opndStack.pop();
                opndStack.push(s_res);
                break;
            case Lex::SOFT_LESSER:
                s_res = opndStack.top();
                opndStack.pop();
                s_res = opndStack.top().compare(s_res) <= 0 ? "1" : "0";
                opndStack.pop();
                opndStack.push(s_res);
                break;
            case Lex::GREATER:
                s_res = opndStack.top();
                opndStack.pop();
                s_res = opndStack.top().compare(s_res) > 0 ? "1" : "0";
                opndStack.pop();
                opndStack.push(s_res);
                break;
            case Lex::LESSER:
                s_res = opndStack.top();
                opndStack.pop();
                s_res = opndStack.top().compare(s_res) < 0 ? "1" : "0";
                opndStack.pop();
                opndStack.push(s_res);
                break;
            case Lex::NOT:
                res = std::stoi(opndStack.top());
                opndStack.pop();
                res = !res;
                opndStack.push(std::to_string(res));
                break;
            default:
                result = "Unknown operator";
                return false;
            }
        }

        result = opndStack.top();
        return true;
    }

    void trypop(std::stack<Lex::Lexem> opstack)
    {
        if (opstack.empty())
        {
            std::__throw_logic_error("stack underflow, caret not closed");
        }
    }

    std::vector<Lex::Lexem> rpn(std::vector<Lex::Lexem> expr)
    {
        std::vector<Lex::Lexem> out;
        std::stack<Lex::Lexem> opstack;

        for (auto i = expr.begin(); i != expr.end(); i++)
        {
            Lex::token t = i->get_token();
            switch (t)
            {
            case Lex::NUMBER:
            case Lex::IDENT:
            case Lex::TEXT:
            case Lex::STRING:
                out.push_back(*i);
                break;
            case Lex::MINUS:
            case Lex::PLUS:
                while (!opstack.empty() && opstack.top().get_token() != Lex::OPEN)
                {
                    out.push_back(opstack.top());
                    trypop(opstack);
                    opstack.pop();
                }
                opstack.push(*i);
                break;
            case Lex::MULT:
                while (!opstack.empty() && opstack.top().get_token() == Lex::MULT)
                {
                    out.push_back(opstack.top());
                    trypop(opstack);
                    opstack.pop();
                }
                opstack.push(*i);
                break;
            case Lex::CARET:
            case Lex::AND:
            case Lex::OR:
                while (
                    !opstack.empty() && opstack.top().get_token() == Lex::AND ||
                    !opstack.empty() && opstack.top().get_token() == Lex::OR)
                {
                    out.push_back(opstack.top());
                    trypop(opstack);
                    opstack.pop();
                }
                opstack.push(*i);
                break;
            case Lex::NOT_EQUAL:
            case Lex::EQUAL:
                while (
                    !opstack.empty() && opstack.top().get_token() == Lex::NOT_EQUAL ||
                    !opstack.empty() && opstack.top().get_token() == Lex::EQUAL)
                {
                    out.push_back(opstack.top());
                    trypop(opstack);
                    opstack.pop();
                }
                opstack.push(*i);
                break;
            case Lex::SOFT_GREATER:
            case Lex::SOFT_LESSER:
            case Lex::GREATER:
            case Lex::LESSER:
                while (
                    !opstack.empty() && opstack.top().get_token() == Lex::MULT ||
                    !opstack.empty() && opstack.top().get_token() == Lex::SOFT_GREATER ||
                    !opstack.empty() && opstack.top().get_token() == Lex::SOFT_LESSER ||
                    !opstack.empty() && opstack.top().get_token() == Lex::GREATER ||
                    !opstack.empty() && opstack.top().get_token() == Lex::LESSER)
                {
                    out.push_back(opstack.top());
                    trypop(opstack);
                    opstack.pop();
                }
                opstack.push(*i);
                break;
            case Lex::NOT:
                while (!opstack.empty() && opstack.top().get_token() != Lex::OPEN)
                {
                    out.push_back(opstack.top());
                    trypop(opstack);
                    opstack.pop();
                }
                opstack.push(*i);
                break;
            case Lex::CLOSE:
                while (opstack.top().get_token() != Lex::OPEN)
                {
                    out.push_back(opstack.top());
                    trypop(opstack);
                    opstack.pop();
                }
                trypop(opstack);
                opstack.pop();
                break;
            case Lex::OPEN:
                opstack.push(*i);
                break;
            default:
                break;
            }
        }

        while (!opstack.empty())
        {
            out.push_back(opstack.top());
            opstack.pop();
        }
        return out;
    }

} // namespace RPN

struct InsertField
{
    FieldType type;
    std::string str;
    unsigned int num;
};

enum UpdateExprType
{
    FIELD_NAME,
    OPERATOR,
    NUMBER
};

struct WhereCondition
{
    Lex::token type;
    std::vector<Lex::Lexem> v_lex_1;
    std::vector<Lex::Lexem> v_lex_2;
    Lex::Lexem l_lex_1;
    Lex::Lexem l_lex_2;
    bool Not;
};

bool preprocess(THandle hTable, Lex::Lexem &lexem, std::string &result_or_error)
{
    // Nothing to preprocess if this is not a field name
    if (
        (Lex::IDENT != lexem.get_token()) &&
        (Lex::TEXT != lexem.get_token()))
    {
        return 1;
    }

    // Getting field type
    FieldType typeField;
    Errors errResult = getFieldType(hTable, lexem.get_string().data(), &typeField);
    if (Errors::OK != errResult)
    {
        std::stringstream roerr;
        roerr << "Failed to identify the type of the \"" << lexem.get_string() << "\" field from the database: " << getErrorString(errResult) << std::endl;
        result_or_error.clear();
        result_or_error = roerr.str();
        closeTable(hTable);
        return 0;
    }

    // Replacing field name with the field value withing the lexem
    char *szValue = 0;
    long lValue = 0;
    switch (typeField)
    {
    case FieldType::Text:
        errResult = getText(hTable, lexem.get_string().data(), &szValue);
        if (Errors::OK == errResult)
        {
            lexem.set_string(szValue);
        }
        break;
    case FieldType::Long:
        errResult = getLong(hTable, lexem.get_string().data(), &lValue);
        if (Errors::OK == errResult)
        {
            lexem.set_string(std::to_string(lValue));
        }
        break;
    default:
        errResult = Errors::BadFieldType;
        break;
    }

    // Processing error result
    if (Errors::OK != errResult)
    {
        std::stringstream roerr;
        roerr << "Failed to retrieve database field data for the \"" << lexem.get_string().data() << "\" field: " << getErrorString(errResult) << std::endl;
        result_or_error.clear();
        result_or_error = roerr.str();
        closeTable(hTable);
        return 0;
    }

    // Successfully processed if reached here
    return 1;
}

bool preprocess(THandle hTable, std::vector<Lex::Lexem> &rpn, std::string &result_or_error)
{
    for (auto i = rpn.begin(); i != rpn.end(); i++)
    {
        if (!preprocess(hTable, *i, result_or_error))
        {
            return 0;
        }
    }
    return 1;
}

bool calculate_in(std::string value, std::vector<Lex::Lexem> values, std::string &result_or_error)
{
    // Checking if "value" exists in the "values" list
    for (auto i = values.begin(); i != values.end(); i++)
    {
        switch (i->get_token())
        {
        case Lex::STRING:
        case Lex::NUMBER:
            if (value == i->get_string())
            {
                result_or_error = "1";
                return 1;
            }
            else
            {
                break;
            }
        default:
            result_or_error = "Bad value type in the list of possible values in the IN clause";
            return 0;
        }
    }

    // Nothing found if reached here
    result_or_error = "0";
    return 1;
}

bool calculate_like(std::string value, std::string condition, std::string &result_or_error)
{
    std::size_t occurance = 0;

    replace_all(condition, "'", "");
    replace_all(condition, "\\", "\\\\");
    replace_all(condition, "$", "\\$");
    replace_all(condition, ".", "\\.");
    replace_all(condition, "*", "\\*");
    replace_all(condition, "+", "\\+");
    replace_all(condition, "?", "\\?");
    replace_all(condition, "{", "\\{");
    replace_all(condition, "}", "\\}");
    replace_all(condition, "|", "\\|");
    replace_all(condition, "(", "\\(");
    replace_all(condition, ")", "\\)");
    replace_all(condition, "%", ".*");
    replace_all(condition, "_", ".");

    result_or_error = std::to_string(std::regex_match(value, std::regex(condition)));
    return 1;
}

bool pipeline(THandle hTable, std::vector<Lex::Lexem> rpn, std::string &result_or_error)
{
    rpn = RPN::rpn(rpn);
    std::string strError;
    if (!preprocess(hTable, rpn, strError))
    {
        result_or_error = strError;
        closeTable(hTable);
        hTable = 0;
        return 0;
    }

    // Calculating the resulting RPN
    if (!RPN::calculate(rpn, result_or_error))
    {
        closeTable(hTable);
        hTable = 0;
        return 0;
    }

    return 1;
}

bool where_pipeline(THandle hTable, WhereCondition condition, std::string &result_or_error)
{
    // Processing trivial case
    if (Lex::ALL == condition.type)
    {
        result_or_error = "1";
        return 1;
    }

    // Checking if the table is open
    if (0 == hTable)
    {
        result_or_error = "Process function failed: NULL pointer to the database table provided";
        return 0;
    }

    // Rearranging lexems according to the operations priorities
    if (
        (Lex::WHERE == condition.type) ||
        (Lex::IN == condition.type))
    {
        condition.v_lex_1 = RPN::rpn(condition.v_lex_1);
    }

    // Replacing table field names with the corresponding table values in the 1rst lexem vector
    switch (condition.type)
    {
    case Lex::WHERE:
    case Lex::IN:
        if (!preprocess(hTable, condition.v_lex_1, result_or_error))
        {
            return 0;
        }
        break;
    case Lex::LIKE:
        if (!preprocess(hTable, condition.l_lex_1, result_or_error))
        {
            return 0;
        }
        break;
    case Lex::ALL:
        break;
    default:
        result_or_error = "Bad WhereResult.type value";
        return 0;
    }

    // Calculating condition value
    switch (condition.type)
    {
    case Lex::WHERE:
        if (!RPN::calculate(condition.v_lex_1, result_or_error))
        {
            return 0;
        }
        break;
    case Lex::IN:
        if (!RPN::calculate(condition.v_lex_1, result_or_error))
        {
            return 0;
        }
        if (!calculate_in(result_or_error, condition.v_lex_2, result_or_error))
        {
            return 0;
        }
        break;
    case Lex::LIKE:
        if (!calculate_like(condition.l_lex_1.get_string(), condition.l_lex_2.get_string(), result_or_error))
        {
            return 0;
        }
        break;
    case Lex::ALL:
        result_or_error = "1";
        break;
    default:
        result_or_error = "Bad WhereResult.type value";
        return 0;
    }

    // Applying the NOT condition if necessary
    if (condition.Not)
        switch (condition.type)
        {
        case Lex::IN:
        case Lex::LIKE:
            if ("0" == result_or_error)
            {
                result_or_error = "1";
            }
            else if ("1" == result_or_error)
            {
                result_or_error = "0";
            }
            else
            {
                return 0;
            }
            break;
        }

    // Everything is OK if reached here
    return 1;
}

bool get_table_field_names(THandle hTable, std::vector<std::string> &field_names, std::string &error)
{
    // Resetting field names array
    field_names.clear();

    // Getting table fields count
    unsigned count = 0;
    Errors errResult = getFieldsNum(hTable, &count);
    if (Errors::OK != errResult)
    {
        std::stringstream err;
        err << "Failed to get the number of table fields: " << getErrorString(errResult);
        error.clear();
        error = err.str();
        return 0;
    }
    else if (0 == count)
    {
        return 1;
    }

    // Listing tabel field names
    char *szFieldName = 0;
    for (int i = 0; i < count; i++)
    {
        errResult = getFieldName(hTable, i, &szFieldName);
        if (Errors::OK == errResult)
        {
            field_names.push_back(szFieldName);
        }
        else
        {
            std::stringstream err;
            err << "Failed to get the name of the table field #" << std::to_string(i) << ": " << getErrorString(errResult);
            error.clear();
            error = err.str();

            field_names.clear();
            return 0;
        }
    }

    // Successfully exiting
    return 1;
}

bool execute_create(std::string table_name, std::vector<struct FieldDef> fields, std::string &error)
{
    // Checking if we have table structure
    if (0 == fields.size())
    {
        std::stringstream err;
        err << "Failed to CREATE table " << table_name.data() << ": No table fields specified";
        error.clear();
        error = err.str();
        return 0;
    }

    // Assembling table header structure
    TableStruct struTable;
    struTable.numOfFields = fields.size();
    struTable.fieldsDef = &fields[0];

    // Actually, creating the table
    Errors errResult = createTable(table_name.data(), &struTable);
    if (Errors::OK == errResult)
    {
        std::stringstream res;
        res << "Created table " << table_name.data();
        error.clear();
        error = res.str();
        return 1;
    }
    else
    {
        std::stringstream err;
        err << "Failed to CREATE TABLE " << table_name.data() << ": " << getErrorString(errResult);
        error.clear();
        error = err.str();
        return 0;
    }
}

bool execute_drop(std::string table_name, std::string &error)
{
    Errors errResult = deleteTable(table_name.data());
    if (Errors::OK == errResult)
    {
        std::stringstream res;
        res << "Dropped table " << table_name.data();
        error.clear();
        error = res.str();
        return 1;
    }
    else
    {
        std::stringstream roerr;
        std::cout << "DROP" << std::endl;
        roerr << "Failed to DROP the " << table_name.data() << " table file: " << getErrorString(errResult) << std::endl;
        error.clear();
        error = roerr.str();
        return 0;
    }
}

bool execute_insert(std::string table_name, std::vector<struct InsertField> fields, std::string &result_or_error)
{
    // Opening the table created
    THandle hTable = 0;
    Errors errResult = openTable(table_name.data(), &hTable);
    if (Errors::OK != errResult)
    {
        std::stringstream roerr;
        roerr << "Failed to open the " << table_name.data() << " table file: " << getErrorString(errResult) << std::endl;
        result_or_error.clear();
        result_or_error = roerr.str();
        return 0;
    }

    // Creating a new record
    errResult = createNew(hTable);
    if (Errors::OK != errResult)
    {
        std::stringstream roerr;
        roerr << "Failed to create new record: " << getErrorString(errResult) << std::endl;
        result_or_error.clear();
        result_or_error = roerr.str();
        closeTable(hTable);
        return 0;
    }

    // Looping through the fields array
    unsigned number = 0;
    char *szFieldName = 0;
    for (auto i = fields.begin(); i != fields.end(); i++)
    {
        // Getting the name of the field
        errResult = getFieldName(hTable, number, &szFieldName);
        if (Errors::OK != errResult)
        {
            std::stringstream roerr;
            roerr << "Failed to get the name of the \"" << number << "\" field from the database: " << getErrorString(errResult) << std::endl;
            result_or_error.clear();
            result_or_error = roerr.str();
            closeTable(hTable);
            return 0;
        }
        else
            number++;

        // Sending field data to the database
        switch (i->type)
        {
        case FieldType::Text:
            errResult = putTextNew(hTable, szFieldName, i->str.data());
            break;
        case FieldType::Long:
            errResult = putLongNew(hTable, szFieldName, i->num);
            break;
        default:
            errResult = Errors::BadFieldType;
            break;
        }

        // Analyzing data send result
        if (Errors::OK != errResult)
        {
            std::stringstream roerr;
            roerr << "Failed to send the \"" << i->str << "\" field data to the database: " << getErrorString(errResult) << std::endl;
            result_or_error.clear();
            result_or_error = roerr.str();
            closeTable(hTable);
            return 0;
        }
    }

    // Finally, submitting the entire record to the database
    errResult = insertzNew(hTable);
    if (Errors::OK != errResult)
    {
        std::stringstream roerr;
        roerr << "Failed to submit inserted record to the database table: " << getErrorString(errResult) << std::endl;
        result_or_error.clear();
        result_or_error = roerr.str();
        closeTable(hTable);
        return 0;
    }

    // Closing the table and exiting
    closeTable(hTable);
    std::stringstream res;
    res << "Inserted into table " << table_name.data();
    result_or_error.clear();
    result_or_error = res.str();
    return 1;
}

bool execute_delete(std::string table_name, WhereCondition where_condition, std::string &result_or_error)
{
    // Opening the table created
    THandle hTable = 0;
    Errors errResult = openTable(table_name.data(), &hTable);
    if (Errors::OK != errResult)
    {
        std::stringstream roerr;
        roerr << "Failed to open the " << table_name.data() << " table file: " << getErrorString(errResult) << std::endl;
        result_or_error.clear();
        result_or_error = roerr.str();
        return 0;
    }

    // Moving cursor to the 1rst table record
    errResult = moveFirst(hTable);
    if (Errors::OK != errResult)
    {
        closeTable(hTable);
        hTable = 0;
        std::stringstream roerr;
        roerr << "Failed to open the " << table_name.data() << " table file: " << getErrorString(errResult) << std::endl;
        result_or_error.clear();
        result_or_error = roerr.str();

        return 0;
    }

    // Looping through records
    while (!afterLast(hTable))
    {
        // Checking if the current record meets the "where" condition
        std::string where_result;
        if (!where_pipeline(hTable, where_condition, where_result))
        {
            std::cout << hTable << std::endl;
            closeTable(hTable);
            hTable = 0;
            return 0;
        }
        else if ("1" != where_result)
        {
            moveNext(hTable);
            continue;
        }

        // Deleting the record
        errResult = deleteRec(hTable);
        if (Errors::OK != errResult)
        {
            closeTable(hTable);
            hTable = 0;
            std::stringstream roerr;
            roerr << "Failed to delete record: " << getErrorString(errResult) << std::endl;
            result_or_error.clear();
            result_or_error = roerr.str();
            return 0;
        }

        // Moving to the next record
        moveNext(hTable);
        continue;
    }

    // Closing the table and exiting
    closeTable(hTable);
    std::stringstream res;
    res << "Deleted from table " << table_name.data();
    result_or_error.clear();
    result_or_error = res.str();
    return 1;
}

bool execute_update(std::string table_name, std::vector<RPN::RPNExpr> rpns, WhereCondition where_condition, std::string &result_or_error)
{
    // Opening the table created
    THandle hTable = 0;
    Errors errResult = openTable(table_name.data(), &hTable);
    if (Errors::OK != errResult)
    {
        std::stringstream roerr;
        roerr << "Failed to open the " << table_name.data() << " table file: " << getErrorString(errResult) << std::endl;
        result_or_error.clear();
        result_or_error = roerr.str();
        return 0;
    }

    // Moving cursor to the 1rst table record
    errResult = moveFirst(hTable);
    if (Errors::OK != errResult)
    {
        closeTable(hTable);
        hTable = 0;
        std::stringstream roerr;
        roerr << "Failed to open the " << table_name.data() << " table file: " << getErrorString(errResult) << std::endl;
        result_or_error.clear();
        result_or_error = roerr.str();

        return 0;
    }

    // Looping through records
    while (!afterLast(hTable))
    {
        // Checking if the current record meets the "where" condition
        std::string where_result;
        if (!where_pipeline(hTable, where_condition, where_result))
        {
            std::cout << hTable << std::endl;
            closeTable(hTable);
            hTable = 0;
            return 0;
        }
        else if ("1" != where_result)
        {
            moveNext(hTable);
            continue;
        }

        // Opening the record for editing
        errResult = startEdit(hTable);
        if (Errors::OK != errResult)
        {
            closeTable(hTable);
            hTable = 0;
            std::stringstream roerr;
            roerr << "Failed to open the table record for editing: " << getErrorString(errResult) << std::endl;
            result_or_error.clear();
            result_or_error = roerr.str();
            return 0;
        }

        // Looping through the fields array
        for (auto i = rpns.begin(); i != rpns.end(); i++)
        {
            // Calculating the RPN to put result to the database table
            if (!pipeline(hTable, i->rpn, result_or_error))
            {
                closeTable(hTable);
                hTable = 0;
                std::stringstream roerr;
                roerr << "Failed to pipeline an RPN for the: " << i->field_name << "field of the " << table_name << " table: " << result_or_error;
                result_or_error.clear();
                result_or_error = roerr.str();
                return 0;
            }

            // Getting corresponding field type
            FieldType typeField;
            errResult = getFieldType(hTable, i->field_name.data(), &typeField);
            if (Errors::OK != errResult)
            {
                closeTable(hTable);
                hTable = 0;
                std::stringstream roerr;
                roerr << "Failed to read the " << i->field_name.data() << " field type from the " << table_name.data() << " table: " << getErrorString(errResult) << std::endl;
                result_or_error.clear();
                result_or_error = roerr.str();
                return 0;
            }

            // Sending field data to the database
            switch (typeField)
            {
            case FieldType::Text:
                errResult = putText(hTable, i->field_name.data(), result_or_error.data());
                break;
            case FieldType::Long:
                errResult = putLong(hTable, i->field_name.data(), atol(result_or_error.data()));
                break;
            default:
                errResult = Errors::BadFieldType;
                break;
            }

            // Analyzing data send result
            if (Errors::OK != errResult)
            {
                std::stringstream roerr;
                roerr << "Failed to send the \"" << i->field_name << "\" field data to the database: " << getErrorString(errResult) << std::endl;
                result_or_error.clear();
                result_or_error = roerr.str();
                closeTable(hTable);
                return 0;
            }
        }

        // Finally, submitting the entire record to the database
        errResult = finishEdit(hTable);
        if (Errors::OK != errResult)
        {
            std::stringstream roerr;
            roerr << "Failed to submit updated record to the database table: " << getErrorString(errResult) << std::endl;
            result_or_error.clear();
            result_or_error = roerr.str();
            closeTable(hTable);
            return 0;
        }

        // Moving to the next table record
        moveNext(hTable);
    }

    // Closing the table and exiting
    closeTable(hTable);
    std::stringstream res;
    res << "Updated table " << table_name.data();
    result_or_error.clear();
    result_or_error = res.str();
    return 1;
}

bool execute_select(std::vector<std::string> field_names, std::string table_name, WhereCondition where_condition, std::string &result_or_error)
{
    // Opening the table created
    THandle hTable = 0;
    Errors errResult = openTable(table_name.data(), &hTable);
    if (Errors::OK != errResult)
    {
        std::stringstream roerr;
        std::cout << "Where" << std::endl;
        roerr << "Failed to open the " << table_name.data() << " table file: " << getErrorString(errResult) << std::endl;
        result_or_error.clear();
        result_or_error = roerr.str();
        return 0;
    }

    // Getting table field names if necessary
    std::string error;
    if (
        (field_names.size() == 0) &&
        (!get_table_field_names(hTable, field_names, error)))
    {
        closeTable(hTable);
        hTable = 0;
        result_or_error = error;
        return 0;
    }
    if (field_names.size() == 0)
    {
        closeTable(hTable);
        hTable = 0;

        std::stringstream roerr;
        roerr << "The " << table_name.data() << " table does not have fields" << std::endl;
        result_or_error.clear();
        result_or_error = roerr.str();

        return 0;
    }

    // Moving cursor to the 1rst table record
    errResult = moveFirst(hTable);
    if (Errors::OK != errResult)
    {
        closeTable(hTable);
        hTable = 0;
        std::stringstream roerr;
        roerr << "Failed to open the " << table_name.data() << " table file: " << getErrorString(errResult) << std::endl;
        result_or_error.clear();
        result_or_error = roerr.str();

        return 0;
    }

    // Looping through records
    char *szFieldValue = 0;
    long lFieldValue = 0;
    while (!afterLast(hTable))
    {
        // Checking if the current record meets the "where" condition
        std::string where_result;
        if (!where_pipeline(hTable, where_condition, where_result))
        {
            std::cout << hTable << std::endl;
            closeTable(hTable);
            hTable = 0;
            return 0;
        }
        else if ("1" != where_result)
        {
            moveNext(hTable);
            continue;
        }

        // Getting current record data
        for (auto i = field_names.begin(); i != field_names.end(); i++)
        {
            // Getting corresponding field type
            FieldType typeField;
            errResult = getFieldType(hTable, i->data(), &typeField);
            if (Errors::OK != errResult)
            {
                closeTable(hTable);
                hTable = 0;

                std::stringstream roerr;
                roerr << "Failed to read the " << i->data() << " field type from the " << table_name.data() << " table: " << getErrorString(errResult) << std::endl;
                result_or_error.clear();
                result_or_error = roerr.str();

                return 0;
            }

            // Getting field value
            std::string value;
            switch (typeField)
            {
            case FieldType::Text:
                errResult = getText(hTable, i->data(), &szFieldValue);
                value = szFieldValue;
                break;
            case FieldType::Long:
                errResult = getLong(hTable, i->data(), &lFieldValue);
                value = std::to_string(lFieldValue);
                break;
            default:
                errResult = Errors::BadFieldType;
                break;
            }

            // adding field value retrieved to the result string
            if (Errors::OK == errResult)
            {
                // adding field or record separator to the result string
                if (i != field_names.begin())
                {
                    result_or_error = result_or_error + "\t";
                }
                result_or_error = result_or_error + value;
            }
            else
            {
                closeTable(hTable);
                hTable = 0;

                std::stringstream roerr;
                roerr << "Failed to read the " << i->data() << " field value from the " << table_name.data() << " table: " << getErrorString(errResult) << std::endl;
                result_or_error.clear();
                result_or_error = roerr.str();

                return 0;
            }
        }
        result_or_error = result_or_error + "\n";
        moveNext(hTable);
    }

    // Closing the table, cleaning up and successfully exiting
    closeTable(hTable);
    hTable = NULL;
    return 1;
}