#include <stdexcept>
#include <iostream>
#include "Lexer.h"

Lexer::Lexer(const std::string &input) : input_(input), lineNumber_(1), columnIndex_(0), currentChar_(input[0]) {}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (currentChar_ != '\0') {

        if (isspace(currentChar_)) {
            skipWhitespace();
        } else if (currentChar_ == '/') {
            if (forwardSlash()) {
                tokens.push_back(operatorToken());
            }
        } else if (isalpha(currentChar_) || currentChar_ == '_') {
            tokens.push_back(identifier());
        } else if (isdigit(currentChar_)) {
            tokens.push_back(number());
        } else if (currentChar_ == '"') {
            tokens.push_back(stringLiteral());
        } else {
            tokens.push_back(operatorOrPunctuation());
        }
    }

    return tokens;
}

void Lexer::advance() {
    columnIndex_++;
    if (columnIndex_ >= input_.size()) {
        currentChar_ = '\0';
    } else {
        currentChar_ = input_[columnIndex_];
    }

}

void Lexer::skipWhitespace() {
    while (isspace(currentChar_)) {
        if (currentChar_ == '\n') {
            lineNumber_++; // Increment lineNumber_ when encountering a newline
        }
        advance();
    }
}

Token Lexer::identifier() {
    std::string identifierValue;
    while (isalpha(currentChar_) || isdigit(currentChar_) || currentChar_ == '_') {
        identifierValue += currentChar_;
        advance();
    }

    TokenType type = TokenType::IDENTIFIER;
    if (identifierValue == "CLASS") {
        type = TokenType::CLASS;
    } else if (identifierValue == "ENUM") {
        type = TokenType::ENUM;
    } else if (identifierValue == "CONST") {
        type = TokenType::CONST;
    } else if (identifierValue == "VAL") {
        type = TokenType::VAL;
    }
    // Add more keyword checks as needed

    Token token;
    token.type = type;
    token.value = identifierValue;
    token.line = lineNumber_;
    return token;
}

Token Lexer::number() {
    std::string value;
    while (isdigit(currentChar_)) {
        value += currentChar_;
        advance();
    }
    return createToken(TokenType::NUMBER, value);
}

Token Lexer::operatorToken() {
    std::string value;

    value = input_[columnIndex_ - 1];

    return createToken(TokenType::OPERATOR, value);
}

bool Lexer::forwardSlash() {
    std::string value;
    bool returnValue = false;

    while (currentChar_ == '/') {
        value += currentChar_;
        advance();
    }

    if (value == "//") {
        // single-line comment
        while (currentChar_ != '\n')
            advance();

    } else if (value == "/" && currentChar_ == '*') {
        // multi-line comment
        while (true) {
            advance();
            if (currentChar_ == '*' && input_[columnIndex_ + 1] == '/') {
                advance(); // skip the '*'
                advance(); // skip the '/'
                break;
            }
        }
    } else if (value == "/") {
        returnValue = true;
    } else {
        createToken(TokenType::STRING_LITERAL, value);
    }

    return returnValue;
}

Token Lexer::stringLiteral() {
    std::string value;
    advance(); // skip the opening quote

    while (currentChar_ != '"' && currentChar_ != '\0') {
        value += currentChar_;
        advance();
    }

    if (currentChar_ != '"') {
        const std::string redColor = "\033[31m";
        const std::string resetColor = "\033[0m";

        std::cerr << redColor << "Error: Unclosed string literal [ Line: " << lineNumber_ << " ]" << resetColor
                  << std::endl;

        exit(1);
//        return createToken(TokenType::UNKNOWN, value);
    }

    advance(); // skip the closing quote
    return createToken(TokenType::STRING_LITERAL, value);
}

Token Lexer::operatorOrPunctuation() {
    std::string value;
    value += currentChar_;
    advance();
    return createToken(TokenType::OPERATOR, value);
}

Token Lexer::createToken(TokenType type, const std::string &value) {
    return Token(type, value, lineNumber_);
}