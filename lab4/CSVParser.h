#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <string>
#include <stdexcept>
#include <vector>
#include <iterator>


struct CSVParseError : public std::runtime_error {
    size_t line_number;
    size_t column_number;
    CSVParseError(const std::string& message, size_t line, size_t column)
        : std::runtime_error(message + " at line " + std::to_string(line) + 
                           ", column " + std::to_string(column)), 
          line_number(line), column_number(column) {}
};

struct CSVFormatError : public std::runtime_error {
    size_t line_number;
    char expected_separator;
    size_t expected_columns;
    size_t actual_columns;
    
    CSVFormatError(size_t line, char exp_sep, size_t exp_cols, size_t act_cols)
        : std::runtime_error(
            "CSV format error at line " + std::to_string(line) + 
            ": expected " + std::to_string(exp_cols) + 
            " columns with separator '" + std::string(1, exp_sep) + 
            "', but found " + std::to_string(act_cols) + " columns"
        ),
        line_number(line),
        expected_separator(exp_sep),
        expected_columns(exp_cols),
        actual_columns(act_cols) {}
};

class CSVRow {
public:
    std::vector<std::string> cells;
};

class CSVReader {
protected:
    std::istream* input_stream;
    char column_separator;
    char quote_character;
    char line_separator;
    size_t current_line = 0;

    std::string ParseQuotedCell(const std::string& line, size_t& position) {
        std::string result;
        size_t column_start = position + 1;
        position++;
        while (position < line.size()) {
            if (line[position] == quote_character) {
                if (position + 1 < line.size() && line[position + 1] == quote_character) {
                    result.push_back(quote_character);
                    position += 2;
                } else {
                    position++;
                    return result;
                }
            } else {
                result.push_back(line[position++]);
            }
        }
        throw CSVParseError("Unclosed quotation mark", current_line, column_start);
    }

    std::string ParseRegularCell(const std::string& line, size_t& position) {
        if (line[position] == quote_character)
            return ParseQuotedCell(line, position);

        std::string result;
        while (position < line.size() && line[position] != column_separator)
            result.push_back(line[position++]);
        return result;
    }

    CSVRow SplitLineIntoCells(const std::string& line) {
        CSVRow row;
        size_t position = 0;
        while (position < line.size()) {
            row.cells.push_back(ParseRegularCell(line, position));
            if (position < line.size() && line[position] == column_separator) position++;
        }
        return row;
    }

public:
    CSVReader(std::istream& stream, char column_sep = ',', char line_sep = '\n', char quote = '"')
        : input_stream(&stream), column_separator(column_sep), 
          quote_character(quote), line_separator(line_sep) {}

    bool ReadNextRow(CSVRow& row) {
        std::string line;
        if (!std::getline(*input_stream, line, line_separator)) return false;
        current_line++;
        row = SplitLineIntoCells(line);
        return true;
    }
    
    char GetColumnSeparator() const { return column_separator; }

    size_t GetCurrentLine() const { return current_line; }
};

template<typename... ColumnTypes>
class CSVParser {
    CSVReader csv_reader;
    size_t lines_to_skip;

public:
    CSVParser(std::istream& stream, size_t skip_lines = 0,
              char column_sep = ',', char line_sep = '\n', char quote = '"')
        : csv_reader(stream, column_sep, line_sep, quote), lines_to_skip(skip_lines) {}

    class Iterator {
        CSVParser* parent_parser;
        std::tuple<ColumnTypes...> current_data;
        bool is_end_iterator = false;

        template<typename ValueType>
        void ParseCellValue(const std::string& text, ValueType& destination, size_t column) {
            std::istringstream stream(text);
            if (!(stream >> destination))
                throw CSVParseError("Failed to parse value: '" + text + "'", 
                                  parent_parser->csv_reader.GetCurrentLine(), column);
        }

        void ParseCellValue(const std::string& text, std::string& destination, size_t column) {
            destination = text;
        }

        template<size_t Index = 0>
        void ParseRowCells(const CSVRow& row, size_t current_column = 1) {
            if (row.cells.size() != sizeof...(ColumnTypes)) {
                throw CSVFormatError(
                    parent_parser->csv_reader.GetCurrentLine(),
                    parent_parser->csv_reader.GetColumnSeparator(),
                    sizeof...(ColumnTypes),
                    row.cells.size()
                );
            }
            
            if constexpr (Index < sizeof...(ColumnTypes)) {
                ParseCellValue(row.cells[Index], std::get<Index>(current_data), current_column);
                ParseRowCells<Index + 1>(row, current_column + 1);
            }
        }
        

        void LoadNextRow() {
            CSVRow row;
            if (!parent_parser->csv_reader.ReadNextRow(row)) {
                is_end_iterator = true;
                return;
            }
            ParseRowCells(row);
        }

    public:
        Iterator(CSVParser* parser, bool is_end = false) : parent_parser(parser), is_end_iterator(is_end) {
            if (!is_end) LoadNextRow();
        }

        Iterator& operator++() {
            LoadNextRow();
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);  // используем префиксный инкремент
            return temp;
        }
        auto operator*() const { return current_data; }

        bool operator==(const Iterator& other) const {
            return is_end_iterator == other.is_end_iterator;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    Iterator begin() {
        for (size_t i = 0; i < lines_to_skip; i++) {
            CSVRow dummy_row;
            csv_reader.ReadNextRow(dummy_row);
        }
        return Iterator(this);
    }

    Iterator end() {
        return Iterator(this, true);
    }
};
