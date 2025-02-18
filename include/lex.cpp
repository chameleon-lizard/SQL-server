#include <iostream>
#include <sstream>
#include <string>
#include <vector>

void replace_all(std::string &str, const std::string &from, const std::string &to)
{
    if (from.empty())
    {
        return;
    }
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

namespace Lex
{

    enum token
    {
        OR,
        IN,
        NOT,
        ALL,
        AND,
        SET,
        DIV,
        END,
        TEXT,
        LONG,
        LIKE,
        MULT,
        PLUS,
        OPEN,
        INTO,
        FROM,
        DROP,
        WHERE,
        TABLE,
        MINUS,
        EQUAL,
        IDENT,
        COMMA,
        CLOSE,
        CARET,
        ERROR,
        SELECT,
        INSERT,
        UPDATE,
        DELETE,
        CREATE,
        LESSER,
        NUMBER,
        STRING,
        PERCENT,
        GREATER,
        NOT_EQUAL,
        UNDEFINED,
        UNDERSCORE,
        SOFT_LESSER,
        OPEN_SQUARE,
        SOFT_GREATER,
        CLOSE_SQUARE
    };

    token parse_word(std::string str);

    class Lexem
    {
    private:
        enum token t;
        std::string s;

    public:
        Lexem();
        Lexem(std::string str);
        Lexem(const Lexem &other);
        Lexem &operator=(const Lexem &other);
        enum token get_token();
        std::string get_string();
        void set_token(token tok);
        void set_string(std::string str);
    };

    void Lexem::set_token(token tok)
    {
        // If we explicitly need to have different token (i.e. string in WHERE)
        t = tok;
    }

    Lexem::Lexem()
    {
        t = UNDEFINED;
        s = "";
    }

    Lexem::Lexem(const Lexem &other)
    {
        t = other.t;
        s = other.s;
    }

    Lexem &Lexem::operator=(const Lexem &other)
    {
        t = other.t;
        s = other.s;

        return *this;
    }

    void Lexem::set_string(std::string str)
    {
        t = parse_word(str);
        s = str;
    }

    int print(enum token t)
    {
        switch (t)
        {
        case OR:
            std::cout << "OR ";
            break;
        case IN:
            std::cout << "IN ";
            break;
        case NOT:
            std::cout << "NOT ";
            break;
        case ALL:
            std::cout << "ALL ";
            break;
        case AND:
            std::cout << "AND ";
            break;
        case SET:
            std::cout << "SET ";
            break;
        case DIV:
            std::cout << "DIV ";
            break;
        case END:
            std::cout << "END ";
            break;
        case TEXT:
            std::cout << "TEXT ";
            break;
        case LONG:
            std::cout << "LONG ";
            break;
        case LIKE:
            std::cout << "LIKE ";
            break;
        case MULT:
            std::cout << "MULT ";
            break;
        case PLUS:
            std::cout << "PLUS ";
            break;
        case OPEN:
            std::cout << "OPEN ";
            break;
        case INTO:
            std::cout << "INTO ";
            break;
        case FROM:
            std::cout << "FROM ";
            break;
        case DROP:
            std::cout << "DROP ";
            break;
        case WHERE:
            std::cout << "WHERE ";
            break;
        case TABLE:
            std::cout << "TABLE ";
            break;
        case MINUS:
            std::cout << "MINUS ";
            break;
        case EQUAL:
            std::cout << "EQUAL ";
            break;
        case IDENT:
            std::cout << "IDENT ";
            break;
        case COMMA:
            std::cout << "COMMA ";
            break;
        case CLOSE:
            std::cout << "CLOSE ";
            break;
        case CARET:
            std::cout << "CARET ";
            break;
        case ERROR:
            std::cout << "ERROR ";
            break;
        case SELECT:
            std::cout << "SELECT ";
            break;
        case INSERT:
            std::cout << "INSERT ";
            break;
        case UPDATE:
            std::cout << "UPDATE ";
            break;
        case DELETE:
            std::cout << "DELETE ";
            break;
        case CREATE:
            std::cout << "CREATE ";
            break;
        case LESSER:
            std::cout << "LESSER ";
            break;
        case NUMBER:
            std::cout << "NUMBER ";
            break;
        case STRING:
            std::cout << "STRING ";
            break;
        case PERCENT:
            std::cout << "PERCENT ";
            break;
        case GREATER:
            std::cout << "GREATER ";
            break;
        case NOT_EQUAL:
            std::cout << "NOT_EQUAL ";
            break;
        case UNDERSCORE:
            std::cout << "UNDERSCORE ";
            break;
        case SOFT_LESSER:
            std::cout << "SOFT_LESSER ";
            break;
        case OPEN_SQUARE:
            std::cout << "OPEN_SQUARE ";
            break;
        case SOFT_GREATER:
            std::cout << "SOFT_GREATER ";
            break;
        case CLOSE_SQUARE:
            std::cout << "CLOSE_SQUARE ";
            break;
        default:
            std::cout << "Error: couldn't recognize the token ";
            return 1;
            break;
        }

        return 0;
    }

    Lexem::Lexem(std::string str)
    {
        t = parse_word(str);
        s = str;
    }

    token Lexem::get_token()
    {
        return t;
    }

    std::string Lexem::get_string()
    {
        return s;
    }

    token parse_word(std::string str)
    {
        enum state
        {
            A_STATE,
            AN_STATE,
            AL_STATE,
            AND_STATE,
            ALL_STATE,
            C_STATE,
            CR_STATE,
            CRE_STATE,
            CREA_STATE,
            CREAT_STATE,
            CREATE_STATE,
            D_STATE,
            DR_STATE,
            DE_STATE,
            DRO_STATE,
            DEL_STATE,
            DROP_STATE,
            DELE_STATE,
            DELET_STATE,
            DELETE_STATE,
            F_STATE,
            FR_STATE,
            FRO_STATE,
            FROM_STATE,
            I_STATE,
            IN_STATE,
            INT_STATE,
            INS_STATE,
            INTO_STATE,
            INSE_STATE,
            INSER_STATE,
            INSERT_STATE,
            L_STATE,
            LI_STATE,
            LO_STATE,
            LIK_STATE,
            LON_STATE,
            LIKE_STATE,
            LONG_STATE,
            N_STATE,
            NO_STATE,
            NOT_STATE,
            O_STATE,
            OR_STATE,
            S_STATE,
            SE_STATE,
            SET_STATE,
            SEL_STATE,
            SELE_STATE,
            SELEC_STATE,
            SELECT_STATE,
            T_STATE,
            TA_STATE,
            TE_STATE,
            TEX_STATE,
            TAB_STATE,
            TEXT_STATE,
            TABL_STATE,
            TABLE_STATE,
            U_STATE,
            UP_STATE,
            UPD_STATE,
            UPDA_STATE,
            UPDAT_STATE,
            UPDATE_STATE,
            W_STATE,
            WH_STATE,
            WHE_STATE,
            WHER_STATE,
            WHERE_STATE,
            OK_STATE,
            DIV_STATE,
            MULT_STATE,
            PLUS_STATE,
            OPEN_STATE,
            CARET_STATE,
            EQUAL_STATE,
            CLOSE_STATE,
            MINUS_STATE,
            COMMA_STATE,
            START_STATE,
            IDENT_STATE,
            NUMBER_STATE,
            LESSER_STATE,
            STRING_STATE,
            GREATER_STATE,
            PERCENT_STATE,
            NOT_EQUAL_STATE,
            UNDERSCORE_STATE,
            EXCLAMATION_STATE,
            OPEN_SQUARE_STATE,
            SOFT_LESSER_STATE,
            CLOSE_SQUARE_STATE,
            SOFT_GREATER_STATE
        };

        int o = 1;
        enum token t;
        int current_line = 0;
        int current_position = 0;
        enum state s = START_STATE;
        int current_char = str[0];
        int current_end_position = 0;
        std::string current_text = "";

        for (int i = 1; s != OK_STATE; current_char = str[i++])
        {
            switch (s)
            {
            case START_STATE:
                if (std::isspace(current_char))
                {
                }
                else if (current_char == 'A')
                {
                    s = A_STATE;
                }
                else if (current_char == 'C')
                {
                    s = C_STATE;
                }
                else if (current_char == 'D')
                {
                    s = D_STATE;
                }
                else if (current_char == 'F')
                {
                    s = F_STATE;
                }
                else if (current_char == 'I')
                {
                    s = I_STATE;
                }
                else if (current_char == 'L')
                {
                    s = L_STATE;
                }
                else if (current_char == 'N')
                {
                    s = N_STATE;
                }
                else if (current_char == 'O')
                {
                    s = O_STATE;
                }
                else if (current_char == 'S')
                {
                    s = S_STATE;
                }
                else if (current_char == 'T')
                {
                    s = T_STATE;
                }
                else if (current_char == 'U')
                {
                    s = U_STATE;
                }
                else if (current_char == 'W')
                {
                    s = W_STATE;
                }

                else if (current_char == '+')
                {
                    s = PLUS_STATE;
                }
                else if (current_char == '-')
                {
                    s = MINUS_STATE;
                }
                else if (current_char == '*')
                {
                    s = MULT_STATE;
                }
                else if (current_char == '/')
                {
                    s = DIV_STATE;
                }
                else if (current_char == '%')
                {
                    s = PERCENT_STATE;
                }
                else if (current_char == ',')
                {
                    s = COMMA_STATE;
                }
                else if (current_char == '(')
                {
                    s = OPEN_STATE;
                }
                else if (current_char == ')')
                {
                    s = CLOSE_STATE;
                }
                else if (current_char == '[')
                {
                    s = OPEN_SQUARE_STATE;
                }
                else if (current_char == ']')
                {
                    s = CLOSE_SQUARE_STATE;
                }
                else if (current_char == '_')
                {
                    s = UNDERSCORE_STATE;
                }
                else if (current_char == '^')
                {
                    s = CARET_STATE;
                }
                else if (current_char == '=')
                {
                    s = EQUAL_STATE;
                }
                else if (current_char == '!')
                {
                    s = EXCLAMATION_STATE;
                }
                else if (current_char == '>')
                {
                    s = GREATER_STATE;
                }
                else if (current_char == '<')
                {
                    s = LESSER_STATE;
                }
                else if (current_char == "'"[0])
                {
                    s = STRING_STATE;
                }
                else if (std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else if (std::isdigit(current_char))
                {
                    s = NUMBER_STATE;
                }
                break;
            case NUMBER_STATE:
                if (std::isdigit(current_char))
                {
                    s = NUMBER_STATE;
                }
                else if (std::isalpha(current_char))
                {
                    o = 0;
                    t = ERROR;
                    s = OK_STATE;
                }
                else
                {
                    t = NUMBER;
                    s = OK_STATE;
                }
                break;
            case STRING_STATE:
                if (current_char == "'"[0])
                {
                    t = STRING;
                    s = OK_STATE;
                }
                else
                {
                    t = STRING;
                    s = STRING_STATE;
                }
                break;
            case IDENT_STATE:
                if ((std::isdigit(current_char)) || (std::isalpha(current_char)))
                {
                    s = TEXT_STATE;
                }
                else
                {
                    t = IDENT;
                    s = OK_STATE;
                }

                break;
            case A_STATE:
                if (current_char == 'N')
                {
                    s = AN_STATE;
                }
                else if (current_char == 'L')
                {
                    s = AL_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case AN_STATE:
                if (current_char == 'D')
                {
                    s = AND_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case AL_STATE:
                if (current_char == 'L')
                {
                    s = ALL_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case AND_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = AND;
                }
                break;
            case ALL_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = ALL;
                }
                break;
            case C_STATE:
                if (current_char == 'R')
                {
                    s = CR_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case CR_STATE:
                if (current_char == 'E')
                {
                    s = CRE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case CRE_STATE:
                if (current_char == 'A')
                {
                    s = CREA_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case CREA_STATE:
                if (current_char == 'T')
                {
                    s = CREAT_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case CREAT_STATE:
                if (current_char == 'E')
                {
                    s = CREATE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case CREATE_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = CREATE;
                }
                break;
            case D_STATE:
                if (current_char == 'R')
                {
                    s = DR_STATE;
                }
                else if (current_char == 'E')
                {
                    s = DE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case DR_STATE:
                if (current_char == 'O')
                {
                    s = DRO_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case DE_STATE:
                if (current_char == 'L')
                {
                    s = DEL_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case DRO_STATE:
                if (current_char == 'P')
                {
                    s = DROP_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case DEL_STATE:
                if (current_char == 'E')
                {
                    s = DELE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case DROP_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = DROP;
                }
                break;
            case DELE_STATE:
                if (current_char == 'T')
                {
                    s = DELET_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case DELET_STATE:
                if (current_char == 'E')
                {
                    s = DELETE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case DELETE_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = DELETE;
                }
                break;
            case F_STATE:
                if (current_char == 'R')
                {
                    s = FR_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case FR_STATE:
                if (current_char == 'O')
                {
                    s = FRO_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case FRO_STATE:
                if (current_char == 'M')
                {
                    s = FROM_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case FROM_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = FROM;
                }
                break;
            case I_STATE:
                if (current_char == 'N')
                {
                    s = IN_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case IN_STATE:
                if (current_char == 'T')
                {
                    s = INT_STATE;
                }
                else if (current_char == 'S')
                {
                    s = INS_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IN;
                }
                break;
            case INT_STATE:
                if (current_char == 'O')
                {
                    s = INTO_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case INS_STATE:
                if (current_char == 'E')
                {
                    s = INSE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case INTO_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = INTO;
                }
                break;
            case INSE_STATE:
                if (current_char == 'R')
                {
                    s = INSER_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case INSER_STATE:
                if (current_char == 'T')
                {
                    s = INSERT_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case INSERT_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = INSERT;
                }
                break;
            case L_STATE:
                if (current_char == 'I')
                {
                    s = LI_STATE;
                }
                else if (current_char == 'O')
                {
                    s = LO_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case LI_STATE:
                if (current_char == 'K')
                {
                    s = LIK_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case LO_STATE:
                if (current_char == 'N')
                {
                    s = LON_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case LIK_STATE:
                if (current_char == 'E')
                {
                    s = LIKE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case LON_STATE:
                if (current_char == 'G')
                {
                    s = LONG_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case LIKE_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = LIKE;
                }
                break;
            case LONG_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = LONG;
                }
                break;
            case N_STATE:
                if (current_char == 'O')
                {
                    s = NO_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case NO_STATE:
                if (current_char == 'T')
                {
                    s = NOT_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case NOT_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = NOT;
                }
                break;
            case O_STATE:
                if (current_char == 'R')
                {
                    s = OR_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case OR_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = OR;
                }
                break;
            case S_STATE:
                if (current_char == 'E')
                {
                    s = SE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case SE_STATE:
                if (current_char == 'T')
                {
                    s = SET_STATE;
                }
                else if (current_char == 'L')
                {
                    s = SEL_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case SET_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = SET;
                }
                break;
            case SEL_STATE:
                if (current_char == 'E')
                {
                    s = SELE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case SELE_STATE:
                if (current_char == 'C')
                {
                    s = SELEC_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case SELEC_STATE:
                if (current_char == 'T')
                {
                    s = SELECT_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case SELECT_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = SELECT;
                }
                break;
            case T_STATE:
                if (current_char == 'A')
                {
                    s = TA_STATE;
                }
                else if (current_char == 'E')
                {
                    s = TE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case TA_STATE:
                if (current_char == 'B')
                {
                    s = TAB_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case TE_STATE:
                if (current_char == 'X')
                {
                    s = TEX_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case TAB_STATE:
                if (current_char == 'L')
                {
                    s = TABL_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case TEX_STATE:
                if (current_char == 'T')
                {
                    s = TEXT_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case TABL_STATE:
                if (current_char == 'E')
                {
                    s = TABLE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case TEXT_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = TEXT;
                }
                break;
            case TABLE_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = TABLE;
                }
                break;
            case U_STATE:
                if (current_char == 'P')
                {
                    s = UP_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case UP_STATE:
                if (current_char == 'D')
                {
                    s = UPD_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case UPD_STATE:
                if (current_char == 'A')
                {
                    s = UPDA_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case UPDA_STATE:
                if (current_char == 'T')
                {
                    s = UPDAT_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case UPDAT_STATE:
                if (current_char == 'E')
                {
                    s = UPDATE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case UPDATE_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = UPDATE;
                }
                break;
            case W_STATE:
                if (current_char == 'H')
                {
                    s = WH_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case WH_STATE:
                if (current_char == 'E')
                {
                    s = WHE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case WHE_STATE:
                if (current_char == 'R')
                {
                    s = WHER_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case WHER_STATE:
                if (current_char == 'E')
                {
                    s = WHERE_STATE;
                }
                else if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = IDENT;
                }
                break;
            case WHERE_STATE:
                if (std::isdigit(current_char) || std::isalpha(current_char))
                {
                    s = IDENT_STATE;
                }
                else
                {
                    s = OK_STATE;
                    t = WHERE;
                }
                break;
            case OK_STATE:
                break;
            case PLUS_STATE:
                t = PLUS;
                s = OK_STATE;
                break;
            case MINUS_STATE:
                t = MINUS;
                s = OK_STATE;
                break;
            case MULT_STATE:
                t = MULT;
                s = OK_STATE;
                break;
            case DIV_STATE:
                t = DIV;
                s = OK_STATE;
                break;
            case PERCENT_STATE:
                t = PERCENT;
                s = OK_STATE;
                break;
            case COMMA_STATE:
                t = COMMA;
                s = OK_STATE;
                break;
            case OPEN_STATE:
                t = OPEN;
                s = OK_STATE;
                break;
            case CLOSE_STATE:
                t = CLOSE;
                s = OK_STATE;
                break;
            case OPEN_SQUARE_STATE:
                t = OPEN_SQUARE;
                s = OK_STATE;
                break;
            case CLOSE_SQUARE_STATE:
                t = CLOSE_SQUARE;
                s = OK_STATE;
                break;
            case UNDERSCORE_STATE:
                t = UNDERSCORE;
                s = OK_STATE;
                break;
            case CARET_STATE:
                t = CARET;
                s = OK_STATE;
                break;
            case EQUAL_STATE:
                t = EQUAL;
                s = OK_STATE;
                break;
            case EXCLAMATION_STATE:
                if (current_char == '=')
                {
                    s = NOT_EQUAL_STATE;
                }
                else
                {
                    o = false;
                    s = OK_STATE;
                    t = ERROR;
                }
                s = OK_STATE;
                break;
            case NOT_EQUAL_STATE:
                t = NOT_EQUAL;
                s = OK_STATE;
                break;
            case GREATER_STATE:
                t = GREATER;
                s = OK_STATE;
                break;
            case LESSER_STATE:
                t = LESSER;
                s = OK_STATE;
                break;
            case SOFT_GREATER_STATE:
                t = SOFT_GREATER;
                s = OK_STATE;
                break;
            case SOFT_LESSER_STATE:
                t = SOFT_LESSER;
                s = OK_STATE;
                break;
            }

            if (s != OK_STATE)
            {
                if (current_char == '\n')
                {
                    current_line++;
                    current_position = 0;
                    current_end_position = 0;
                }
                else if (std::isspace(current_char))
                {
                    current_position++;
                    current_end_position++;
                }
                else
                {
                    current_end_position++;
                }

                if (t != END && !std::isspace(current_char))
                {
                    current_text.push_back(current_char);
                }
            }

            if (s == OK_STATE)
            {
                return t;
            }
        }
        return ERROR;
    }

    std::vector<Lexem> parse(std::string str)
    {
        replace_all(str, "(", " ( ");
        replace_all(str, ")", " ) ");
        replace_all(str, ",", " , ");

        std::vector<std::string> strings;
        std::istringstream f(str);
        std::string s;

        while (getline(f, s, ' '))
        {
            if (s.length())
            {
                strings.push_back(s);
            }
        }

        std::vector<Lexem> lexems;

        for (auto i = strings.begin(); i != strings.end(); i++)
        {
            lexems.push_back(Lexem(*i));
        }

        return lexems;
    }
} // namespace Lex